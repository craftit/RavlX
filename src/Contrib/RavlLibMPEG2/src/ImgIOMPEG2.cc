// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////
//! lib=RavlLibMPEG2
//! author = "Warren Moore"
//! file="Ravl/Contrib/MPEG/LibMPEG2/ImgIOMPEG2.cc"

#include "Ravl/Image/ImgIOMPEG2.hh"
#include "Ravl/IO.hh"
#include "Ravl/BitStream.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Image/RealYUVValue.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/RGBcYUV.hh"
#include <fstream>
#include "Ravl/DP/AttributeValueTypes.hh"

extern "C"
{
  #include <mpeg2dec/mpeg2convert.h>
}

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN
{
  
  static const char frameTypes[5] = { 'X', 'I', 'P', 'B', 'D' };
  
  static const IntT g_cacheStep = 20;
  
  //: Default constructor.
  
  ImgILibMPEG2BodyC::ImgILibMPEG2BodyC(bool seekable) :
    m_decoder(0),
    m_state(-2),
    m_buffer(4096),
    m_bufStart(0),
    m_bufEnd(0),
    m_frameNo(0),
    m_frameLast(0),
    m_imageCache(g_cacheStep * 2),
    m_lastFrameType(0),
    m_firstIFrame(false),
    m_seekable(seekable),
    m_sequenceInit(false),
    m_lastRead(0),
    m_offsets(true),
    m_gopCount(0),
    m_gopLimit(0),
    m_previousGop(0),
    m_endFound(false)
  {
    m_decoder = mpeg2_init();
    
    BuildAttributes();
  }
  
  //: Destructor.
  
  ImgILibMPEG2BodyC::~ImgILibMPEG2BodyC()
  {
    if(m_decoder != 0)
      mpeg2_close(m_decoder);
  }
  
  //: Get next frame.
  
  bool ImgILibMPEG2BodyC::Get(ImageC<ByteRGBValueC> &img)
  {
    ONDEBUG(cerr << "ImgILibMPEG2BodyC::Get (" << m_frameNo << ")" << endl;)
    
    // If not set, get the initial seek position
    if (m_seekable && m_offsets.IsEmpty() && !InitialSeek())
    {
      cerr << "ImgILibMPEG2BodyC::Get unable to find initial seek position" << endl;
      return false;
    }
    
    bool ok = true;
    Tuple2C< ImageC<ByteRGBValueC>, IntT > dat;
    while (!m_imageCache.LookupR(m_frameNo, dat))
    {
      // If the last read failed, don't bother again
      if (!ok)
      {
        cerr << "ImgILibMPEG2BodyC::Get decoder failed for frame (" << m_frameNo << ")" << endl;
        return false;
      }

      // Keep on truckin' til we find the frame
      ok = DecodeGOP(m_frameNo);

      // If at EOS, stop
      if (IsGetEOS())
      {
        cerr << "ImgILibMPEG2BodyC::Get at end of stream before frame (" << m_frameNo << ")" << endl;
        return false;
      }
    }
    
    // Extract the cached image
    img = dat.Data1();
    m_lastFrameType = dat.Data2();
    
    // Move counter to the next frame
    m_frameNo++;
    
    return true;
  }
  
  //: Seek to location in stream.
  
  bool ImgILibMPEG2BodyC::Seek(UIntT off)
  {
    ONDEBUG(cerr << "ImgILibMPEG2BodyC::Seek (" << off << ")" << endl;)
    if (m_seekable)
    {
      m_frameNo = off;
      return true;
    }
    return false;
  }

  //: Get the size of the file in frames (-1 if not known)

  UIntT ImgILibMPEG2BodyC::Size() const
  {
    return ((UIntT)(-1));
  }
  
  //: Seek to location in stream.
  
  bool ImgILibMPEG2BodyC::Seek64(StreamPosT off)
  {
    ONDEBUG(cerr << "ImgILibMPEG2BodyC::Seek (" << off << ")" << endl;)
    if (m_seekable)
    {
      m_frameNo = off;
      return true;
    }
    return false;
  }
  
  //: Get the size of the file in frames (-1 if not known)

  StreamPosT ImgILibMPEG2BodyC::Size64() const
  {
    return -1;
  }
  
  bool ImgILibMPEG2BodyC::IsGetEOS() const
  {
    // Can't be the end of the stream if we have the current seek image
    if (m_imageCache.Contains(m_frameNo))
      return false;
    
    // We've reached an end tag and haven't looked for an earlier frame, so we must still be at the end
    if (m_endFound)
    {
      ONDEBUG(cerr << "ImgILibMPEG2BodyC::IsGetEOS end found" << endl;)
      return true;
    }
    
    // If the stream is reporting EOS, nothing else we can do
    if (input.IsGetEOS())
    {
      ONDEBUG(cerr << "ImgILibMPEG2BodyC::IsGetEOS input at EOS" << endl;)
      return true;
    }
    
    return false;
  }

  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool ImgILibMPEG2BodyC::GetAttr(const StringC &attrName,StringC &attrValue)
  {
    if(attrName == "frametype")
    {
      attrValue = StringC(frameTypes[m_lastFrameType]);
      return true; 
    }

    return DPPortBodyC::GetAttr(attrName,attrValue);
  }
  
  bool ImgILibMPEG2BodyC::InitialSeek()
  {
    ONDEBUG(cerr << "ImgILibMPEG2BodyC::InitialSeek" << endl;)
    RavlAssertMsg(m_offsets.IsEmpty(), "ImgILibMPEG2BodyC::InitialSeek called with non-empty offsets");

    if (input.IsValid())
    {
      // Create the seek control
      DPSeekCtrlC seek(input);
      if (seek.IsValid())
      {
        // Get the initial seek position
        m_offsets.Insert(0, seek.Tell64(), false);
      }
      
      return true;
    }
    
    return false;
  }

  bool ImgILibMPEG2BodyC::Reset()
  {
    RavlAssertMsg(!m_seekable, "ImgILibMPEG2BodyC::Reset should not be called on a seekable stream");

    ONDEBUG(cerr << "ImgILibMPEG2BodyC::Reset" << endl;)
    
    // Close the decoder
    mpeg2_close(m_decoder);
    
    // Restart the decoder
    m_decoder = mpeg2_init();
    m_sequenceInit = false;
    
    // Clear the counters
    m_gopCount = 0;
    m_gopLimit = 2;
    
    // Clear the read info
    m_endFound = false;
    
    // Make sure the first frame is an I frame
    m_firstIFrame = false;
    
    // Reset the data buffer
    m_bufStart = m_bufEnd = m_buffer.DataStart();
      
    return true;
  }
  
  //: Read data from stream into buffer.
  
  bool ImgILibMPEG2BodyC::ReadInput()
  {
    RavlAssertMsg(input.IsValid(), "ImgILibMPEG2BodyC::ReadInput called with invalid input");

    if (m_seekable)
    {
      // Create the seek control
      DPSeekCtrlC seek(input);
      if (!seek.IsValid())
      {
        ONDEBUG(cerr << "ImgILibMPEG2BodyC::ReadInput unable to seek" << endl;)
        return false;
      }
      
      // Get the next data packet
      m_bufStart = &(m_buffer[0]);
      m_lastRead = seek.Tell64();
      UIntT len = input.GetArray(m_buffer);
      if (len == 0)
      {
        ONDEBUG(cerr << "ImgILibMPEG2BodyC::ReadInput returned no data" << endl;)
        return false;
      }
      
      // Set the end buffer pointer
      m_bufEnd = m_bufStart + len;
    }
    else
    {
      // Get the next data packet
      m_bufStart = &(m_buffer[0]);
      UIntT len = input.GetArray(m_buffer);
      if (len == 0)
      {
        ONDEBUG(cerr << "ImgILibMPEG2BodyC::ReadInput returned no data" << endl;)
        return false;
      }
      m_lastRead += len;
      
      // Set the end buffer pointer
      m_bufEnd = m_bufStart + len;
    }
    
    return true;
  }

  //: Seek to the relevant GOP offset
  
  bool ImgILibMPEG2BodyC::SeekGOP(StreamPosT &firstFrameNo, StreamPosT &localFrameNo)
  {
    // Find the GOP before the required frame...
    StreamPosT at;
    while(!m_offsets.Find(firstFrameNo, at))
    {
      if(firstFrameNo == 0)
        return false;
      firstFrameNo--;
    }
    localFrameNo = firstFrameNo;
    ONDEBUG(cerr << "ImgILibMPEG2BodyC::SeekGOP seeking frame (" << firstFrameNo << ") at (" << at << ")" << endl;)
    
    // Find the gop before the required one
    StreamPosT previousFrameNo = firstFrameNo;
    if (firstFrameNo > 0)
    {
      previousFrameNo = firstFrameNo - 1;
      StreamPosT prevAt;
      while(!m_offsets.Find(previousFrameNo, prevAt))
      {
        if(previousFrameNo == 0)
          return false;
        previousFrameNo--;
      }
      
      // Only step back another GOP if the last GOP read wasn't the previous one, as we need to flush the decoder
      ONDEBUG(cerr << "ImgILibMPEG2BodyC::SeekGOP previous gop frame (" << m_previousGop << ")" << endl;)
      if (m_previousGop != previousFrameNo)
      {
        // Get the previous GOP info
        at = prevAt;
        localFrameNo = previousFrameNo;
        
        if (previousFrameNo == 0)
          // Skip the GOP at the beginning of the sequence, flush the first GOP through the decoder then store the next
          m_gopLimit = 3;
        else
          // Flush the first GOP through the decoder then store the next
          m_gopLimit = 2;
        ONDEBUG(cerr << "ImgILibMPEG2BodyC::SeekGOP seeking previous frame (" << previousFrameNo << ") at (" << at << ")" << endl;)
      }
    }
    else
    {
      // Skip over the GOP at the beginning of the sequence
      m_gopLimit = 2;
    }
    
    // Create the seek control
    DPSeekCtrlC seek(input);
    if (!seek.IsValid())
    {
      cerr << "ImgILibMPEG2BodyC::SeekGOP invalid seek control" << endl;
      return false;
    }
    
    // Reset the decoder if the first frame of the GOP is zero, or an end tag has been encountered
    if (localFrameNo == 0 || m_endFound)
    {
      ONDEBUG(cerr << "ImgILibMPEG2BodyC::SeekGOP decoder reset" << endl;)

      // Close the decoder
      mpeg2_close(m_decoder);
      
      // Restart the decoder
      m_decoder = mpeg2_init();
      m_sequenceInit = false;

      if (m_endFound)
      {
        ONDEBUG(cerr << "ImgILibMPEG2BodyC::SeekGOP sequence tag search" << endl;)
        
        // Get the first frame pos
        StreamPosT startAt;
        if (m_offsets.Find(0, startAt))
        {
          seek.Seek64(startAt);
          
          // Flush the sequence tag and the first two GOPs through the decoder
          UIntT gopCount = 2;
          while (ReadInput() && m_endFound)
          {
            // Submit the data packet to the decoder
            mpeg2_buffer(m_decoder, m_bufStart, m_bufEnd);
            
            do
            {
              // Parse the current data packet
              m_state = mpeg2_parse(m_decoder);
              
              // If we hit a sequence tag and we're uninitialised, decode the tag
              if (m_state == STATE_SEQUENCE && !m_sequenceInit)
              {
                // Get the MPEG info structure
                const mpeg2_info_t *info = mpeg2_info(m_decoder);
                
                // Decode the sequence tag
                StreamPosT startFrame = 0;
                Decode(startFrame, info);
              }
              
              // If we've initialised the sequence then we only need to parse two GOPs before we can continue
              if (m_sequenceInit && m_state == STATE_GOP)
              {
                // Count the GOP
                gopCount--;
                
                // If two GOPs have passed, the deocder is initialised and ready
                if (gopCount == 0)
                {
                  // Clear the end tag indicator
                  m_endFound = false;
                  break;
                }
              }
            } while (m_state != -1);
          }
        }
        else
        {
          cerr << "ImgILibMPEG2BodyC::SeekGOP invalid first frame when searching sequence tag" << endl;
          return false;
        }
      }
    }

    // Seek to the required position
    seek.Seek64(at);
    
    return true;
  }
  
  //: Decode a whole GOP and put it in the image cache.
  
  bool ImgILibMPEG2BodyC::DecodeGOP(StreamPosT firstFrameNo)
  {
    ONDEBUG(cerr << "ImgILibMPEG2BodyC::DecodeGOP first frame(" << firstFrameNo << ")" << endl;)
    
    // Decoder vars
    m_frameLast = firstFrameNo;
    m_gopLimit = 1;

    if (m_seekable)
    {
      // Seek to the relevant GOP position
      if (!SeekGOP(firstFrameNo, m_frameLast))
        return false;
    }
    else
    {
      // Skip the GOP at the beginning
      if (m_frameLast == 0)
        m_gopLimit = 2;
    }

    // Set the state vars
    m_state = -1;
    m_gopCount = 0;
    
    // Decoder loop
    do
    {
      // Read some data into the decoder if necessary
      if (m_state == -1)
      {
        if(ReadInput())
          mpeg2_buffer(m_decoder, m_bufStart, m_bufEnd);
        else
        {
          ONDEBUG(cerr << "ImgILibMPEG2BodyC::DecodeGOP failed to read data." << endl;)
          m_state = -2;
          m_previousGop = firstFrameNo;
          break;
        }
      }

      // Get the MPEG info structure
      const mpeg2_info_t *info = mpeg2_info(m_decoder);
    
      // Decode the buffer
      m_state = mpeg2_parse(m_decoder);
      Decode(m_frameLast, info);

      // Check we've decoded a complete GOP
      if (m_gopCount >= m_gopLimit)
      {
        if (m_seekable)
        {
          // Store the gop frame number
          m_previousGop = firstFrameNo;
  
          // Store the read pos
          if (!m_endFound)
          {
            //StreamPosT parsePos = m_lastRead + (StreamPosT)(m_decoder->buf_start - &(m_buffer[0]));
            StreamPosT parsePos = mpeg2_getpos(m_decoder);
            ONDEBUG(cerr << "ImgILibMPEG2BodyC::DecodeGOP **** Recording frame (" << m_frameLast << ") at (" << parsePos << ")" << endl;)
            m_offsets.Insert(m_frameLast, parsePos, false);
          }
  
          break;
        }
        else
        {
          // Make sure we've emptied the current buffer
          if (m_state == -1)
            break;
        }
      }
    } while(true);

    return true;
  }
  
  bool ImgILibMPEG2BodyC::Decode(StreamPosT &frameNo, const mpeg2_info_t *info)
  {
//    ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode state (" << m_state << ")" << endl;)

    switch(m_state)
    {
      case -1:   // Got to end of buffer ?
//        ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode need input" << endl;)
        break;
        
      case STATE_SEQUENCE_REPEATED:
      case STATE_SEQUENCE:
        ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode got " << (m_state == STATE_SEQUENCE ? "SEQUENCE" : "SEQUENCE_REPEATED" ) << "." << endl;)
        if (!m_sequenceInit)
        {
          m_sequenceInit = true;
          m_imgSize = Index2dC(info->sequence->height,info->sequence->width);
          info = mpeg2_info(m_decoder);
          ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode imageSize(" << m_imgSize << ")" << endl;)
        }
        break;
      
      case STATE_GOP:
        ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode got STATE_GOP count(" << m_gopCount << ") limit(" << m_gopLimit << ") frame(" << frameNo << ")" << endl;)
        m_gopCount++;
        
        // Check it's not a skipped GOP
        if (m_seekable)
        {
          StreamPosT at;
          if (m_offsets.Find(frameNo, at) && at == -1)
          {
            ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode skipped GOP encountered" << endl;)
            m_gopLimit++;
          }
        }
        break;
    
      case STATE_PICTURE:
        ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode got PICTURE." << endl;)
        break;
        
      case STATE_PICTURE_2ND:
        ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode got PICTURE_2ND." << endl;)
        break;
        
      case STATE_END:
        ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode got END." << endl;)
        OnEnd();
        break; 
          
      case STATE_SLICE:
        ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode got SLICE." << endl;)
        // Ignore the slice if it's well before what we need, or no image is ready
        if ((m_gopLimit - m_gopCount < 3) && info->display_fbuf != 0)
        {
          // Only store the image if it's fresh
          if (m_gopLimit - m_gopCount <= 1)
          {
            // Get the frame type
            IntT frameType = info->display_picture->flags & PIC_MASK_CODING_TYPE;
            ONDEBUG( \
              UIntT frameid = (UIntT)info->display_fbuf->id;
              cerr << "ImgILibMPEG2BodyC::Decode frameid(" << frameid << ") temporal ref(" << info->display_picture->temporal_reference << ") type(" << frameTypes[frameType] << ")" << endl; \
            )
            
            // Cache the image
            if (m_firstIFrame || frameType == 1)
            {
              // Convert YUV to RGB
              ImageC<ByteRGBValueC> nimg(m_imgSize[0].V(), m_imgSize[1].V());
              UIntT stride = m_imgSize[1].V();
              if (info->display_fbuf)
              {
                ByteT ** xbuf = (ByteT **) info->display_fbuf->buf;
                UIntT row = 0;
                for(Array2dIterC<ByteRGBValueC> it(nimg);it;row++)
                {
                  ByteT *yd = &(xbuf[0][stride * row]); 
                  IntT crow = ((row >> 1) & ~((UIntT)1)) + row % 2;
              
                  ByteT *ud = &(xbuf[1][(stride/2) * crow]);
                  ByteT *vd = &(xbuf[2][(stride/2) * crow]);
                  do {
                    ByteRGBValueC &p1 = *it;
                    it++;
                    ByteYUV2RGB2(yd[0],yd[1],(*ud + 128),(*vd + 128),p1,*it);
                    yd += 2;
                    ud++;
                    vd++;	      
                  } while(it.Next()) ;
                }
              }
  
              ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode caching frame(" << frameNo << ")" << endl;)
              m_imageCache.Insert(frameNo, Tuple2C<ImageC<ByteRGBValueC>,IntT>(nimg, frameType));
              m_firstIFrame = true;
            }
            else
            {
              ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode skipping partial frame(" << frameNo << ")" << endl;)
            }
          }
          else
          {
            // Flush the image through the decoder
            ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode skipping frame(" << frameNo << ")" << endl;)
          }
          
          if (m_seekable)
          {
            // Can cause problems if a slice is found after a GOP, so skip this one
            if (m_gopLimit - m_gopCount < 1)
            {
              ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode **** replacing GOP as frame (" << frameNo << ") found after" << endl;)
              m_offsets.Insert(frameNo, -1, true);
              m_gopLimit++;
              m_gopDone = false;
            }
    
            // Check the cache size
            StreamPosT cacheSize = (m_gopLimit - 1) * g_cacheStep;
            if (cacheSize > m_imageCache.MaxSize())
            {
              ONDEBUG(cerr << "ImgILibMPEG2BodyC::Decode resized image cache to (" << cacheSize << ")" << endl;)
              m_imageCache.MaxSize(cacheSize);
            }
          }

          // Update the frame count
          if (m_firstIFrame)
            frameNo++;
        }
        break;
        
      case STATE_INVALID:
        cerr << "ImgILibMPEG2BodyC::Decode got INVALID." << endl;
        break;
          
      default:
        cerr << "ImgILibMPEG2BodyC::Decode unknown state(" << m_state << ")" << endl;
        return false;
    }

    return true;
  }

  //: Called when a sequence end tag is encountered

  void ImgILibMPEG2BodyC::OnEnd()
  {
    // Set the end tag indicator
    m_endFound = true;
  }
  
  //: Register stream attributes.

  void ImgILibMPEG2BodyC::BuildAttributes() 
  {
    RegisterAttribute(AttributeTypeStringC("frametype", "MPEG frame type", true, false));
  }
}

