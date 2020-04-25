// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/DVDRead/DVDRead.cc"
//! lib=RavlDVDRead
//! author = "Warren Moore"

#include <stdint.h>
#include "Ravl/DVDRead.hh"
#include "dvdread/ifo_read.h"
#include "dvdread/ifo_print.h"
#include "dvdread/nav_read.h"
#include "Ravl/DList.hh"
#include "Ravl/DP/AttributeValueTypes.hh"
#include <string.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{
  
  static const RealT g_timeStep[] = { 120, 60, 30, 10, 7.5, 7.0, 6.5, 6, 5.5, 5, 4.5, 4, 3.5, 3, 2.5, 2, 1.5, 1, 0.5 };
  
  DVDReadBodyC::DVDReadBodyC(const UIntT title, const StringC device) :
    m_device(device),
    m_title(title - 1),
    m_numFps(0.0),
    m_dvdReader(NULL),
    m_dvdVmgFile(NULL),
    m_dvdVtsFile(NULL),
    m_dvdPgc(NULL),
    m_dvdFile(NULL),
    m_curCell(0),
    m_curSector(0),
    m_curBlock(0),
    m_curByte(0),
    m_signalFlush(true),
    m_needFlush(true)
  {
    // Try to open the DVD device
    m_dvdReader = DVDOpen(m_device);
    if (!m_dvdReader)
    {
      cerr << "DVDReadBodyC::DVDReadBodyC unable to open DVD device (" << m_device << ")" << endl;
      Close();
      return;
    }
    
    // Get the VMG IFO
    m_dvdVmgFile = ifoOpen(m_dvdReader, 0);
    if(!m_dvdVmgFile)
    {
      cerr << "DVDReadBodyC::DVDReadBodyC can't open VMG info" << endl;
      Close();
      return;
    }
    tt_srpt_t *tt_srpt = m_dvdVmgFile->tt_srpt;
    
    // Check the title is valid
    if (m_title > tt_srpt->nr_of_srpts)
    {
      cerr << "DVDReadBodyC::DVDReadBodyC invalid title set" << endl;
      Close();
      return;
    }
  
    // Check the angles
    if (tt_srpt->title[m_title].nr_of_angles > 1)
    {
      cerr << "DVDReadBodyC::DVDReadBodyC multiple angle DVDs not supported" << endl;
      Close();
      return;
    }
      
    // Get the title info
    UIntT vtsnum = tt_srpt->title[m_title].title_set_nr;
    UIntT ttnnum = tt_srpt->title[m_title].vts_ttn;
    ONDEBUG(cerr << "DVDReadBodyC::DVDReadBodyC VTS(" << vtsnum << ") title(" << ttnnum << ")" << endl;)

    // Get the VTS IFO
    m_dvdVtsFile = ifoOpen(m_dvdReader, vtsnum);
    if(!m_dvdVtsFile)
    {
      cerr << "DVDReadBodyC::DVDReadBodyC unable to load IFO for VTS (" << vtsnum << ")" << endl;
      Close();
      return;
    }
    
    // Get the program chain info
    m_dvdPgc = m_dvdVtsFile->vts_pgcit->pgci_srp[ttnnum-1].pgc;
    UIntT numCells = m_dvdPgc->nr_of_cells;
    if (numCells == 0)
    {
      cerr << "DVDReadBodyC::DVDReadBodyC no cells found for title (" << ttnnum << ")" << endl;
      Close();
      return;
    }
    ONDEBUG(cerr << "DVDReadBodyC::DVDReadBodyC cells(" << numCells << ")" << endl;)

    // Get the FPS
    m_numFps = 25.0;
    dvd_time_t titleTime = m_dvdPgc->playback_time;
    if (titleTime.hour != 0 &&  titleTime.minute != 0 && titleTime.second != 0)
    {
//      if ((titleTime.frame_u & 0xc0) >> 6 == 1)
//        m_numFps = 25.0;
      if ((titleTime.frame_u & 0xc0) >> 6 == 3)
        m_numFps = 29.97;
    }
    ONDEBUG(cerr << "DVDReadBodyC::DVDReadBodyC fps(" << m_numFps << ")" << endl;)

    // Create the cell data table
    RealT playTime = 0.0;
    m_cellTable = SArray1dC< Tuple3C< StreamPosT, UIntT, bool> >(numCells);
    for (UIntT i = 0; i < numCells; i++)
    {
      m_cellTable[i].Data1() = Floor(playTime * m_numFps);
      m_cellTable[i].Data2() = m_dvdPgc->cell_playback[i].first_sector;
      m_cellTable[i].Data3() = m_dvdPgc->cell_playback[i].stc_discontinuity;
      ONDEBUG(cerr << "DVDReadBodyC::DVDReadBodyC cell(" << i << ")\tframe(" << m_cellTable[i].Data1() << ")\tsector(" << hex << m_cellTable[i].Data2() << dec << ")\tdiscontinuity(" << (m_cellTable[i].Data3() ? "y" : "n") << ")" << endl;)

      // Get the cell payback info
      dvd_time_t cellTime = m_dvdPgc->cell_playback[i].playback_time;
      playTime += GetTime(cellTime);
    }

    // Open the title
    m_dvdFile = DVDOpenFile(m_dvdReader, vtsnum, DVD_READ_TITLE_VOBS);
    if (!m_dvdFile)
    {
      cerr << "DVDReadBodyC::DVDReadBodyC unable to open title VOB VTS(" << vtsnum << ") for title (" << ttnnum << ")" << endl;
      Close();
      return;
    }

    // Start at the first frame
    m_curSector = m_cellTable[m_curCell].Data2();
    
    // Read NAV packet
    UIntT len = DVDReadBlocks(m_dvdFile, m_curSector, 1, m_curNav);
    RavlAssertMsg(len == 1,  "DVDReadBodyC::DVDReadBodyC unable to read NAV block");
    navRead_DSI(&m_dsiPack, &(m_curNav[DSI_START_BYTE]));
    RavlAssertMsg(m_curSector == m_dsiPack.dsi_gi.nv_pck_lbn, "DVDReadBodyC::DVDReadBodyC DSI sector does not match cell sector");

    BuildAttributes();
  }
  
  DVDReadBodyC::~DVDReadBodyC()
  {
    Close();
  }
  
  void DVDReadBodyC::Close()
  {
    // Reset the params
    m_device = "";
    m_title = 0;
    m_numFps = 0.0;
    
    // Free all open handles
    if (m_dvdFile)
    {
      DVDCloseFile(m_dvdFile);
      m_dvdFile = NULL;
    }
    
    if (m_dvdVtsFile)
    {
      ifoClose(m_dvdVtsFile);
      m_dvdVtsFile = NULL;
    }
    
    if (m_dvdVmgFile)
    {
      ifoClose(m_dvdVmgFile);
      m_dvdVmgFile = NULL;
    }

    if (m_dvdReader)
    {
      DVDClose(m_dvdReader);
      m_dvdReader = NULL;
    }
  }
  
  ByteT DVDReadBodyC::Get()
  {
    SArray1dC<ByteT> data(1);
    GetArray(data);
    return data[0];
  }

  bool DVDReadBodyC::Get(ByteT &data)
  {
    // Check we're not at the end of the stream
    if (IsGetEOS())
      return false;
    
    SArray1dC<ByteT> dataChar(1);
    bool read = GetArray(dataChar) > 0;
    if (read)
      data = dataChar[0];
    return read;
  }

  IntT DVDReadBodyC::GetArray(SArray1dC<ByteT> &data)
  {
    RavlAssertMsg(m_curSector == m_dsiPack.dsi_gi.nv_pck_lbn, "DVDReadBodyC::GetArray DSI sector does not match cell sector");

    // Get the data size
    RavlAssertMsg(m_curBlock < m_dsiPack.dsi_gi.vobu_ea, "DVDReadBodyC::GetArray current block out of DSI range");
    
    UIntT dataRead = 0;
    ByteT dataBuf[g_blockSize];
    while (dataRead < data.Size())
    {
      // If required, flush the data pipeline
      if (m_needFlush)
      {
        m_signalFlush();
        m_needFlush = false;
      }
      
      // Write out the part of the current pack
      if (m_curByte != 0 || (data.Size() - dataRead < g_blockSize))
      {
        // Calculate how many bytes to read
        UIntT byteCount = g_blockSize - m_curByte;
        if (byteCount > (data.Size() - dataRead))
          byteCount = data.Size() - dataRead;
        
        // Read the block into the buffer
        UIntT len = DVDReadBlocks(m_dvdFile, m_curSector + m_curBlock + 1, 1, dataBuf);
        if (len != 1)
          cerr << "DVDReadBodyC::GetArray unable to block (" << hex << m_curSector << ":" << m_curBlock << dec << ")" << endl;
        
        // Copy the data from the buffer to the buffer
        memcpy(data.DataStart() + dataRead, dataBuf + m_curByte, byteCount);
        
        // Update the counters
        dataRead += byteCount;
        m_curByte += byteCount;
        
        // Have we stepped over the end of the block?
        RavlAssertMsg(m_curByte <= g_blockSize, "DVDReadBodyC::GetArray byte count exceeded maximum");
        if (m_curByte == g_blockSize)
        {
          m_curByte = 0;
          m_curBlock++;
        }
      }
      else
      {
        // Write out as many 2k blocks as we can
        UIntT blockCount = (data.Size() - dataRead) / g_blockSize;
        if (blockCount > 0)
        {
          if (m_curBlock < m_dsiPack.dsi_gi.vobu_ea)
          {
            // Limit the block size
            if (blockCount > (m_dsiPack.dsi_gi.vobu_ea - m_curBlock))
              blockCount = (m_dsiPack.dsi_gi.vobu_ea - m_curBlock);
            
            // Read the blocks
            UIntT len = DVDReadBlocks(m_dvdFile, m_curSector + m_curBlock + 1, blockCount, data.DataStart() + dataRead);
            if (len != blockCount)
              cerr << "DVDReadBodyC::GetArray unable to read (" << blockCount << ") blocks from (" << hex << m_curSector << ":" << m_curBlock << dec << ")" << endl;
            
            // Update the counters
            dataRead += blockCount * g_blockSize;
            m_curBlock += blockCount;
          }
        }
      }
      
      // Do we need to go to the next VOBU?
      if (m_curBlock == m_dsiPack.dsi_gi.vobu_ea)
      {
        if (m_dsiPack.vobu_sri.next_vobu != SRI_END_OF_CELL)
        {
          // Get the next VOBU sector
          m_curSector += (m_dsiPack.vobu_sri.next_vobu & 0x7fffffff);

          // Read NAV packet
          UIntT len = DVDReadBlocks(m_dvdFile, m_curSector, 1, m_curNav);
          RavlAssertMsg(len == 1,  "DVDReadBodyC::GetArray unable to read NAV block");
          navRead_DSI(&m_dsiPack, &(m_curNav[DSI_START_BYTE]));
          RavlAssertMsg(m_curSector == m_dsiPack.dsi_gi.nv_pck_lbn, "DVDReadBodyC::GetArray DSI sector does not match cell sector");
      
          // Reset the counters
          m_curBlock = 0;
          m_curByte = 0;
          m_endFound = false;

          // Display the VOBU time
          ONDEBUG(cerr << "DVDReadBodyC::GetArray vobu frame(" << Floor(((RealT)GetTime(m_dsiPack.dsi_gi.c_eltm) * m_numFps) + m_cellTable[m_curCell].Data1()) << ")" << endl;)
        }
        else
        {
          // Check if we have another VOBU to move on to
          if (OnEnd())
          {
            // Have we read any data?
            if (dataRead > 0)
              // If so, stop here so the data can be processed before the pipeline is flushed
              break;
          }
        }
      }
    }

    return dataRead;
  }

  bool DVDReadBodyC::Seek(UIntT off)
  {
    return false;
  }

  UIntT DVDReadBodyC::Tell() const
  {
    return ((UIntT)(-1));
  }

  UIntT DVDReadBodyC::Size() const
  {
    return ((UIntT)(-1));
  }

  bool DVDReadBodyC::Seek64(StreamPosT off)
  {
    return false;
  }
  
  StreamPosT DVDReadBodyC::Tell64() const
  {
    return -1;
  }
  
  StreamPosT DVDReadBodyC::Size64() const
  {
    return -1;
  }

  bool DVDReadBodyC::IsGetEOS() const
  {
    // EOS if no file open
    if (!m_dvdFile)
    {
//      ONDEBUG(cerr << "DVDReadBodyC::IsGetEOS no DVD file open" << endl;)
      return true;
    }
    
//    ONDEBUG(cerr << "DVDReadBodyC::IsGetEOS (" << (m_endFound ? "Y" : "N") << ")" << endl;)
    return m_endFound;
  }

  bool DVDReadBodyC::Discard()
  {
    return false;
  }

  bool DVDReadBodyC::SeekFrame(const StreamPosT frame, StreamPosT &lastFrame)
  {
    ONDEBUG(cerr << "DVDReadBodyC::SeekFrame frame(" << frame << ")" << endl;)

    // Find the closest sector
    UIntT cellCount = 0;
    while (cellCount < m_cellTable.Size() - 1 && frame > m_cellTable[cellCount + 1].Data1())
    {
      cellCount++;
    }
    
    // If we have changed cell, get the new cell start VOBU info
    if (m_curCell != cellCount)
    {
      // Store the sector position
      m_curCell = cellCount;
      m_curSector = m_cellTable[cellCount].Data2();
      ONDEBUG(cerr << "DVDReadBodyC::SeekFrame cell(" << m_curCell << ") sector(" << hex << m_curSector << dec << ") frame(" << m_cellTable[cellCount].Data1() << ")" << endl;)
      
      // Read NAV packet
      UIntT len = DVDReadBlocks(m_dvdFile, m_curSector, 1, m_curNav);
      RavlAssertMsg(len == 1,  "DVDReadBodyC::SeekFrame unable to read NAV block");
      navRead_DSI(&m_dsiPack, &(m_curNav[DSI_START_BYTE]));
      RavlAssertMsg(m_curSector == m_dsiPack.dsi_gi.nv_pck_lbn, "DVDReadBodyC::SeekFrame DSI sector does not match cell sector");
  
      // Reset the counters
      m_curBlock = 0;
      m_curByte = 0;
      m_endFound = false;
      
      // Flush the pipeline
      m_needFlush = true;
      
      // Store the new frame number
      lastFrame = m_cellTable[cellCount].Data1();
    }
    
    // Create a table of frame offsets
    RealT frameStep[19];
    for (UIntT i = 0; i < 19; i++)
      frameStep[i] = m_numFps * g_timeStep[i];

    // Keep moving the sector until we're as close as possible to the next frame
    bool skipped = false;
    while (true)
    {
      // Display the VOBU time and current offset
      StreamPosT vobuFrame = Floor(((RealT)GetTime(m_dsiPack.dsi_gi.c_eltm)) * m_numFps) + m_cellTable[cellCount].Data1();
      RealT frameOffset = frame - vobuFrame;
      ONDEBUG(cerr << "DVDReadBodyC::SeekFrame vobu frame(" << vobuFrame << ") current offset(" << frameOffset << ")" << endl;)
      
      // Leave now if we're bang on
      if (frameOffset == 0)
        break;
      
      // Update the start frame if we've skipped
      if (skipped)
        lastFrame = vobuFrame;

      // Set the seek table
      UIntT *seekTable = m_dsiPack.vobu_sri.fwda;
      
      // Positive offset (seeking forward)
      bool checkAgain = false;
      if (frameOffset > 0)
      {
        // Can we get closer within the VOBU?
        UIntT curSector = m_curSector;
        ByteT curNav[g_blockSize];
        dsi_t dsiPack;
        for (UIntT i = 0; i < 19; i++)
        {
          ONDEBUG(cerr << "DVDReadBodyC::SeekFrame offset entry(" << g_timeStep[i] << ") frame step(" << frameStep[i] << ") ";)

          // Valid DSI table entry
          if ((seekTable[i] & 0x80000000) > 0 && (seekTable[i] & 0x3fffffff) != 0x3fffffff)
          {
            ONDEBUG(cerr << "sector(" << hex << (seekTable[i] & 0x3fffffff) << dec << ")" << endl;)
            
            // Select the closest offset
            if (Ceil(frameStep[i]) < frameOffset)
            {
              // Make sure it's not referring to itself
              if ((seekTable[i] & 0x3fffffff) != 0)
              {
                ONDEBUG(cerr << "DVDReadBodyC::SeekFrame seeking to offset entry(" << g_timeStep[i] << ") frame step(" << frameStep[i] << ") sector(" << hex << (seekTable[i] & 0x3fffffff) << dec << ")" << endl;)

                // Set the new sector offset
                curSector += seekTable[i] & 0x3fffffff;
                
                // Read NAV packet
                UIntT len = DVDReadBlocks(m_dvdFile, curSector, 1, curNav);
                RavlAssertMsg(len == 1,  "DVDReadBodyC::SeekFrame unable to read NAV block");
                navRead_DSI(&dsiPack, &(curNav[DSI_START_BYTE]));
                RavlAssertMsg(curSector == dsiPack.dsi_gi.nv_pck_lbn, "DVDReadBodyC::SeekFrame DSI sector does not match cell sector");
                
                // Get the new frame offset
                StreamPosT nextVobuFrame = Floor(((RealT)GetTime(dsiPack.dsi_gi.c_eltm)) * m_numFps) + m_cellTable[cellCount].Data1();
                RealT nextFrameOffset = frame - nextVobuFrame;

                // Check it's got a different time block
                if ((nextFrameOffset >= 0) && (GetTime(dsiPack.dsi_gi.c_eltm) != GetTime(m_dsiPack.dsi_gi.c_eltm)))
                {
                  // Copy over the nav block and store the details
                  m_curSector = curSector;
                  memcpy(m_curNav, curNav, g_blockSize);
                  memcpy(&m_dsiPack, &dsiPack, sizeof(dsi_t));
                  
                  // Reset the counters
                  m_curBlock = 0;
                  m_curByte = 0;
                  m_endFound = false;
                  
                  // Update the decoder frame
                  skipped = true;
                  
                  // Flush the pipeline
                  m_needFlush = true;
                  
                  // Show we got one this time round
                  checkAgain = true;
                }
                else
                {
                  ONDEBUG(cerr << "DVDReadBodyC::SeekFrame ignoring unsuitable vobu frame(" << nextVobuFrame << ")" << endl);
                }
                
                break;
              }
            }
          }
          else
          {
            ONDEBUG(cerr << "invalid" << endl;)
          }
        }
      }
      else
      {
        ONDEBUG(cerr << "DVDReadBodyC::SeekFrame seeking to beginning of cell(" << m_curCell << ")  sector(" << hex << m_cellTable[m_curCell].Data2() << dec << ")" << endl;)

        // Set the new sector offset
        m_curSector = m_cellTable[m_curCell].Data2();
        
        // Read NAV packet
        UIntT len = DVDReadBlocks(m_dvdFile, m_curSector, 1, m_curNav);
        RavlAssertMsg(len == 1,  "DVDReadBodyC::SeekFrame unable to read NAV block");
        navRead_DSI(&m_dsiPack, &(m_curNav[DSI_START_BYTE]));
        RavlAssertMsg(m_curSector == m_dsiPack.dsi_gi.nv_pck_lbn, "DVDReadBodyC::SeekFrame DSI sector does not match cell sector");
        
        // Reset the counters
        m_curBlock = 0;
        m_curByte = 0;
        m_endFound = false;
        
        // Update the decoder frame
        lastFrame = m_cellTable[m_curCell].Data1();
        
        // Flush the pipeline
        m_needFlush = true;
        
        // Show we got one this time round
        checkAgain = true;
      }
      
      if (!checkAgain)
        break;
    }
    
    return true;
  }
  
  Int64T DVDReadBodyC::GetTime(dvd_time_t time)
  {
    Int64T s = 0;
    s += ((time.hour >>   4) & 0x0f) * 36000;
    s += ((time.hour       ) & 0x0f) *  3600;
    s += ((time.minute >> 4) & 0x0f) *   600;
    s += ((time.minute     ) & 0x0f) *    60;
    s += ((time.second >> 4) & 0x0f) *    10;
    s += ((time.second     ) & 0x0f)        ;
    return s;
  }
  
  bool DVDReadBodyC::OnEnd()
  {
    ONDEBUG(cerr << "DVDReadBodyC::OnEnd reached end of vobu" << endl;)
    
    // Have we reached the last cell?
    if (m_curCell < m_cellTable.Size() - 1)
    {
      // Move to the next cell
      m_curCell++;
      m_curSector = m_cellTable[m_curCell].Data2();
      ONDEBUG(cerr << "DVDReadBodyC::OnEnd cell(" << m_curCell << ") sector(" << m_curSector << ")" << endl;)
      
      // Read NAV packet
      UIntT len = DVDReadBlocks(m_dvdFile, m_curSector, 1, m_curNav);
      RavlAssertMsg(len == 1,  "DVDReadBodyC::OnEnd unable to read NAV block");
      navRead_DSI(&m_dsiPack, &(m_curNav[DSI_START_BYTE]));
      RavlAssertMsg(m_curSector == m_dsiPack.dsi_gi.nv_pck_lbn, "DVDReadBodyC::OnEnd DSI sector does not match cell sector");
  
      // Reset the counters
      m_curBlock = 0;
      m_curByte = 0;
      
      // Signal a pipeline flush
      m_needFlush = true;
      
      return false;
    }
    
    // Mark the end
    m_endFound = true;
    
    return true;
  }
  
  bool DVDReadBodyC::GetAttr(const StringC &attrName, StringC &attrValue)
  {
    RavlAssertMsg(m_dvdVtsFile != NULL, "DVDReadBodyC::GetAttr requires a valid VTS file");
    
    if (m_dvdVtsFile != NULL)
    {
      vtsi_mat_t *vtsi_mat = m_dvdVtsFile->vtsi_mat;
      video_attr_t &video_attr = vtsi_mat->vts_video_attr;
      
      if (attrName == "mpegversion")
      {
        attrValue = (video_attr.mpeg_version == 1 ? "MPEG2" : "MPEG1");
        return true;
      }

      if (attrName == "videoformat")
      {
        attrValue = (video_attr.video_format == 1 ? "PAL" : "NTSC");
        return true;
      }

      if (attrName == "aspectratio")
      {
        attrValue = (video_attr.display_aspect_ratio == 3 ? "16:9" : "4:3");
        return true;
      }

      if (attrName == "framesize")
      {
        IntT height = 480;
        if(video_attr.video_format != 0) 
          height = 576;
        
        StringC width;
        switch(video_attr.picture_size)
        {
          case 0:
            width = "720x";
            break;
            
          case 1:
            width = "704x";
            break;

          case 2:
            width = "352x";
            break;

          case 3:
            width = "352x";
            height /= 2;
            break;
          
          default:
            height = 0;
            break;
        if (height != 0)
        {
          attrValue = width + StringC(height);
          return true;
        }
      }
      
      if (attrName == "duration")
      {
        RavlAssertMsg(m_dvdPgc != NULL, "DVDReadBodyC::GetAttr requires a valid PGC");
        
        dvd_time_t time = m_dvdPgc->playback_time;
        if (time.hour != 0 &&  time.minute != 0 && time.second != 0)
        {
          attrValue.form("%02x:%02x:%02x", time.hour, time.minute, time.second);
          return true;
        }
      }

      if (attrName == "framerate")
      {
        RavlAssertMsg(m_numFps != 0, "DVDReadBodyC::GetAttr FPS not valid");
        
        attrValue.form("%2.2f", m_numFps);
        return true;
      }

      if (attrName == "frames")
      {
        RavlAssertMsg(m_numFps != 0, "DVDReadBodyC::GetAttr FPS not valid");
        
        dvd_time_t time = m_dvdPgc->playback_time;
        StreamPosT frames = Floor(((RealT)GetTime(time)) * m_numFps);
        attrValue = StringC(frames);
        return true;
      }
    }

    return DPPortBodyC::GetAttr(attrName, attrValue);
  }

  //: Register stream attributes.

  void DVDReadBodyC::BuildAttributes()
  {
    RegisterAttribute(AttributeTypeStringC("mpegversion", "MPEG version", true, false));
    RegisterAttribute(AttributeTypeStringC("videoformat", "Video format", true, false));
    RegisterAttribute(AttributeTypeStringC("aspectratio", "Aspect ratio", true, false));
    RegisterAttribute(AttributeTypeStringC("framesize",   "Frame size",   true, false));
    RegisterAttribute(AttributeTypeStringC("duration",    "Duration",     true, false));
    RegisterAttribute(AttributeTypeStringC("framerate",   "Frame rate",   true, false));
    RegisterAttribute(AttributeTypeStringC("frames",      "Total frames", true, false));
  }

}
