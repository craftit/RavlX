// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace
//! file="Ravl/Contrib/Face/Xml/ClaimSession.cc"

#include "Ravl/Face/ClaimSession.hh"
//#include "Ravl/DLIter.hh"
#include "Ravl/CDLIter.hh"
//#include "Ravl/HashIter.hh"
//#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
//#include "Ravl/Collection.hh"
#include "Ravl/Face/FaceInfoDb.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/Random.hh"
#include "Ravl/SArray1dIter2.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  namespace FaceN {

//------------------------------------------------------------------------------
//! constructor
    ClaimSessionIterC::ClaimSessionIterC(const ClaimSessionC& ClaimSession, const FaceInfoDbC& FaceInfoDb) :
        m_claimSession(ClaimSession), m_faceInfoDb(FaceInfoDb)
    {
      //cout << "Claim session iter constructor\n";
      First();
    }

//------------------------------------------------------------------------------
//! Move current position to the beginning
    void ClaimSessionIterC::First()
    {
//  cout << "ClaimSessionIterC::First\n";
      m_cacheValid = false;
//  cout << "claims num:" << m_claimSession.m_claimsSet.Size() << endl;
      m_it1 = DArray1dIterC < DArray1dC<ClaimSessionRecordC> > (m_claimSession.m_claimsSet);
      if (m_it1) {
//    cout << "claim size:" << m_it1->Size() << endl;
        m_it2 = DArray1dIterC < ClaimSessionRecordC > (*m_it1);
        while (!m_it2) {
          m_it1++;
          if (!m_it1)
            break;
//      cout << "claim size:" << m_it1->Size() << endl;
          m_it2 = DArray1dIterC < ClaimSessionRecordC > (*m_it1);
        }
      }

      m_valid = m_it1 && m_it2;
    }

//------------------------------------------------------------------------------
//! goto next claim returns true if still same claimId
    bool ClaimSessionIterC::Next()
    {
      //cout << "ClaimSessionIterC::Next\n";
      if (!m_valid)
        return false;

      //invalidate cache
      m_cacheValid = false;

      bool sameClaimId = true;
      m_it2++;
      if (!m_it2) {
        sameClaimId = false;
        m_it1++;
        if (m_it1) {
          //cout << "claim size:" << m_it1->Size() << endl;
          m_it2 = DArray1dIterC < ClaimSessionRecordC > (*m_it1);
          while (!m_it2) {
            m_it1++;
            if (!m_it1)
              break;
            //cout << "claim size:" << m_it1->Size() << endl;
            m_it2 = DArray1dIterC < ClaimSessionRecordC > (*m_it1);
          }
          m_valid = m_it2;
        } else
          m_valid = false;
      }
      return sameClaimId;
    }

//------------------------------------------------------------------------------
//! goto next claim
    void ClaimSessionIterC::operator++()
    {
      Next();
    }

//------------------------------------------------------------------------------
//! goto next claim
    void ClaimSessionIterC::operator++(int)
    {
      Next();
    }

//------------------------------------------------------------------------------
    const ClaimC& ClaimSessionIterC::operator*() const
    {
      if (!m_valid)
        throw ExceptionOperationFailedC("ClaimSessionIterC::operator*:invalid iterator");

      if (!m_cacheValid) {
        const ClaimSessionRecordC& rec = *m_it2;
        const UIntT verificationCode = rec.VerificationCode();
        SArray1dIterC < UIntT > faceIdCodesIt(m_claimSession.m_verificationSets[verificationCode]);
        if (!faceIdCodesIt) //should never happen unless empty array added through low level functions
          throw ExceptionOperationFailedC("ClaimSessionIterC::operator*:no faces associated with the claim");

        m_cache = ClaimC(m_claimSession.m_claimedIds.Backward(m_it1.Index().V()),
            m_faceInfoDb[m_claimSession.m_faceIds.Backward(*faceIdCodesIt)].ActualId(),
            0);

        for (; faceIdCodesIt; faceIdCodesIt++) {
          m_cache.AddFaceId(m_claimSession.m_faceIds.Backward(*faceIdCodesIt));
        }
        m_cacheValid = true;
      }
      return m_cache;
    }

//------------------------------------------------------------------------------
    const ClaimC* ClaimSessionIterC::operator->() const
    {
      return &(operator*());
    }

//------------------------------------------------------------------------------
    void ClaimSessionIterC::Get(StringC &ClaimId, SArray1dC<StringC> &FaceIds, bool &TrueClaim) const
    {
      if (!m_valid)
        throw ExceptionOperationFailedC("ClaimSessionIterC::Get:invalid iterator");

      ClaimId = m_claimSession.m_claimedIds.Backward(m_it1.Index().V());

      const ClaimSessionRecordC& rec = *m_it2;
      TrueClaim = rec.TrueClaim();
      const UIntT verificationCode = rec.VerificationCode();
      FaceIds = SArray1dC < StringC > (m_claimSession.m_verificationSets[verificationCode].Size());

      for (SArray1dIter2C < UIntT, StringC > it(m_claimSession.m_verificationSets[verificationCode], FaceIds); it; it++) {
        it.Data2() = m_claimSession.m_faceIds.Backward(it.Data1());
      }
    }

//------------------------------------------------------------------------------
    void ClaimSessionIterC::Get(UIntT &ClaimCode, UIntT &VerCode, bool &TrueClaim) const
    {
      if (!m_valid)
        throw ExceptionOperationFailedC("ClaimSessionIterC::Get:invalid iterator");

      ClaimCode = m_it1.Index().V();

      const ClaimSessionRecordC& rec = *m_it2;
      TrueClaim = rec.TrueClaim();
      VerCode = rec.VerificationCode();
    }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
    ClaimSessionC::ClaimSessionC() :
        size(0)
    {
    }

//------------------------------------------------------------------------------
    ClaimSessionC::ClaimSessionC(XMLIStreamC& Xml) :
        size(0)
    {
      LoadXML(Xml);
    }

//! Load from file
    bool ClaimSessionC::Load(const FilenameC &Filename)
    {
      if (!Filename.IsReadable())
        throw ExceptionOperationFailedC("ClaimSessionC::Load:File is not readable:" + Filename);

      StringC ext = Filename.Extension();
      UIntT curSize = size;
      if (ext == "xml") {
        XMLIStreamC strm(Filename);
        LoadXML(strm);
      } else if (ext == "abs") {
        BinIStreamC strm(Filename);
        if (!strm.Stream().IsOpen())
          throw ExceptionOperationFailedC("ClaimSessionC::Load:Can't open file:" + Filename);
        //skip header
        char c;
        StringC typeName;
        UIntT ver;
        strm >> c >> c >> typeName >> ver;
        //cout << "Type:" << typeName << endl;
        if (ver != 0)
          throw ExceptionOperationFailedC("ClaimSessionC::Load:Not supported version:" + Filename);

        Load(strm);
      } else
        throw ExceptionOperationFailedC("ClaimSessionC::Load:wrong file extension");

      //cout << "Size:" << size << endl;

      return size > curSize;
    }

//------------------------------------------------------------------------------
//! Save to xml stream
    void ClaimSessionC::SaveXML(XMLOStreamC &Xml) const
    {
      // Set auto-indent mode
      Xml.AutoIndent();

      // Start tag
      Xml.StartTag("claimsession");
      Xml.SetAttrib("type", "1");

      // Write child tags
      Xml.StartContents();

      // Write verification face sets
      Xml.StartTag("verificationsets");
      Xml.StartContents();
      for (DArray1dIterC < SArray1dC<UIntT> > it1(m_verificationSets); it1; it1++) {
        Xml.StartTag("set");
        Xml.SetAttrib("id", it1.Index().V());
        for (SArray1dIterC < UIntT > it2(*it1); it2; it2++) {
          Xml << m_faceIds.Backward(*it2) << ' ';
        }
        Xml.EndTag(); //end tag set
      }
      Xml.EndTag(); //end verificationsets tag

      // Loop through claimed ids
      Xml.StartTag("claimstable");
      Xml.StartContents();
      for (DArray1dIterC < DArray1dC<ClaimSessionRecordC> > it1(m_claimsSet); it1; it1++) {
        Xml.StartTag("claim");
        Xml.SetAttrib("claimedId", m_claimedIds.Backward(it1.Index().V()));

        Xml.StartContents();

        // loop through claims records
        for (DArray1dIterC < ClaimSessionRecordC > it2(*it1); it2; it2++) {
          const ClaimSessionRecordC& rec = *it2;

          Xml << rec.VerificationCode() << (rec.TrueClaim() ? 'T' : 'F') << ' ';
        }

        // End claims tag
        Xml.EndTag();
      }
      Xml.EndTag(); //end claimstable tag

      // End claimsession tag
      Xml.EndTag();
    }

//------------------------------------------------------------------------------
//! Save to binary stream
    void ClaimSessionC::Save(BinOStreamC &Strm) const
    {
      //cout << "ClaimSessionC::Save\n";

      //save claimed ids
      UIntT numClaimedIds = m_claimedIds.Size();
      Strm << numClaimedIds;
      for (UIntT i = 0; i < numClaimedIds; i++)
        Strm << m_claimedIds.Backward(i);

      //save face ids
      UIntT numFaceIds = m_faceIds.Size();
      Strm << numFaceIds;
      for (UIntT i = 0; i < numFaceIds; i++)
        Strm << m_faceIds.Backward(i);

      //save verification sets
      Strm << m_verificationSets.Size();
      for (DArray1dIterC < SArray1dC<UIntT> > it1(m_verificationSets); it1; it1++) {
        Strm << *it1;
      }

      // save claims set
      Strm << m_claimsSet.Size();
      for (DArray1dIterC < DArray1dC<ClaimSessionRecordC> > it1(m_claimsSet); it1; it1++) {
        Strm << it1->Size();
        for (DArray1dIterC < ClaimSessionRecordC > it2(*it1); it2; it2++) {
          const ClaimSessionRecordC& rec = *it2;
          Strm << rec.FullVerificationCode();
        }
      }
    }

//------------------------------------------------------------------------------
//! Load from xml stream
    void ClaimSessionC::LoadXML(XMLIStreamC &Xml)
    {
      StringC strType("");

      // Read tags until we get a EnrolSession
      while (Xml && !(Xml.ReadTag(strType) == XMLBeginTag && strType == "claimsession"))
        ;
      if (!Xml)
        return;

      StringC fileType;
      if (Xml.GetAttrib("type", fileType)) //new format
          {
        if (fileType != "1")
          throw ExceptionOperationFailedC("ClaimSessionC::LoadXML:wrong format type");

        //read verification sets
        HashC < UIntT, UIntT > localVerificationCodes; // map verification ids to global ids
        while (Xml && !(Xml.ReadTag(strType) == XMLBeginTag && strType == "verificationsets"))
          ;
        if (!Xml)
          return;
        while (Xml && Xml.PeekTag(strType) != XMLEndTag) {
          if (strType == "set") {
            Xml.ReadTag(strType);
            UIntT localVerId;
            Xml.GetAttrib("id", localVerId);
            StringC list;
            Xml >> list;
            list = list.TopAndTail();
            UIntT numItems = list.freq(' ') + 1;
            SArray1dC < UIntT > localFaceIds(numItems);
            StrIStreamC strm(list);
            UIntT i = 0;
            while (strm && i < numItems) {
              StringC faceId;
              strm >> faceId;
              //cout << "key:" << key << "  faceId:" << faceId << endl;
              if (faceId.IsEmpty())
                break;
              localFaceIds[i++] = GetFaceIdCode(faceId);
            }
            Xml.ReadTag(strType);

            //add verification set
            localVerificationCodes[localVerId] = GetVerificationIdCode(localFaceIds);
          } else {
            //cout << "Skipping:" << strType << endl;
            Xml.ReadTag(strType);
            Xml.ReadTag(strType);
          }
        }
        Xml.ReadTag(strType); // the end verificationsets tag

        while (Xml && !(Xml.ReadTag(strType) == XMLBeginTag && strType == "claimstable"))
          ;
        if (!Xml)
          return;
        while (Xml && Xml.PeekTag(strType) != XMLEndTag) {
          if (strType == "claim") {
            Xml.ReadTag(strType);
            StringC claimedId;
            Xml.GetAttrib("claimedId", claimedId);
            UIntT claimedIdCode = GetClaimedIdCode(claimedId);

            StringC list;
            Xml >> list;
            StrIStreamC strm(list.TopAndTail());
            while (strm) {
              UIntT verificationId = 0xffffffff;
              char trueClaimChar;
              strm >> verificationId >> trueClaimChar;
              //cout << "key:" << key << "  faceId:" << faceId << endl;
              if (verificationId == 0xffffffff)
                break;
              AddClaim(claimedIdCode, localVerificationCodes[verificationId], trueClaimChar == 'T');
            }
            Xml.ReadTag(strType);
          } else {
            //cout << "Skipping:" << strType << endl;
            Xml.ReadTag(strType);
            Xml.ReadTag(strType);
          }
        }
        Xml.ReadTag(strType); // the end claimstable tag
      } else //old format
      {
        // Read features
        while (Xml && Xml.PeekTag(strType) != XMLEndTag) {
          if (strType == "claim") {
            ClaimC claim;
            Xml >> claim;

            AddClaim(claim.ClaimedId(), claim.FaceIds(), claim.ClaimedId() == claim.ActualId());
          } else {
            // Skip this set of tags
            Xml.ReadTag(strType);
            Xml.ReadTag(strType);
          }
        }
        Xml.ReadTag(strType); // the end claimsession tag
      }
    }

//------------------------------------------------------------------------------
//! Load from binary stream
    void ClaimSessionC::Load(BinIStreamC &Strm)
    {
      //cout << "ClaimSessionC::Load\n";

      //load claimed ids
      m_claimedIds = TwoWayMapC<StringC>();
      UIntT numClaimedIds;
      Strm >> numClaimedIds;
      //cout << "NumClaimedIds:" << numClaimedIds << endl;
      for (UIntT i = 0; i < numClaimedIds; i++) {
        StringC claimedId;
        Strm >> claimedId;
        //cout << "i:" << i << "  clId:" << claimedId << endl;
        if (claimedId.IsEmpty())
          throw ExceptionOperationFailedC("ClaimSessionC::Load:Failed to load claim id");
        if (i != m_claimedIds.Forward(claimedId))
          throw ExceptionOperationFailedC("ClaimSessionC::Load:error reading claimed ids:"
              + StringC(i)
              + " != "
              + m_claimedIds.Forward(claimedId));
      }

      //load face ids
      m_faceIds = TwoWayMapC<StringC>();
      unsigned int numFaceIds;
      Strm >> numFaceIds;
      //cout << "NumFaceIds:" << numFaceIds << endl;
      for (unsigned int i = 0; i < numFaceIds; i++) {
        StringC faceId;
        Strm >> faceId;
        if (faceId.IsEmpty())
          throw ExceptionOperationFailedC("ClaimSessionC::Load:Failed to load face id");
        if (i != m_faceIds.Forward(faceId))
          throw ExceptionOperationFailedC("ClaimSessionC::Load:error reading face ids");
      }

      //load verification sets
      UIntT numVerCodes;
      Strm >> numVerCodes;
      //cout << "NumVerCodes:" << numVerCodes << endl;
      m_verificationSets = DArray1dC < SArray1dC<UIntT> > (numVerCodes);
      for (DArray1dIterC < SArray1dC<UIntT> > it1(m_verificationSets); it1; it1++) {
        SArray1dC < UIntT > &verSet = *it1;
        Strm >> verSet;
        if (verSet.Size() == 0)
          throw ExceptionOperationFailedC("ClaimSessionC::Load:Failed to load verification set");
      }

      // load claims set
      unsigned int numClaims;
      Strm >> numClaims;
      m_claimsSet = DArray1dC < DArray1dC<ClaimSessionRecordC> > (numClaims);
      //cout << "size1:" <<  m_claimsSet.Size() << endl;
      for (DArray1dIterC < DArray1dC<ClaimSessionRecordC> > it1(m_claimsSet); it1; it1++) {
        unsigned int numFaceIds;
        Strm >> numFaceIds;
        *it1 = DArray1dC < ClaimSessionRecordC > (numFaceIds);
        //cout << "size2:" <<  it1->Size() << endl;
        for (DArray1dIterC < ClaimSessionRecordC > it2(*it1); it2; it2++) {
          ClaimSessionRecordC& rec = *it2;
          UIntT code = 0xffffffff;
          Strm >> code;
          if (code == 0xffffffff)
            throw ExceptionOperationFailedC("ClaimSessionC::Load:Failed to load verification code");
          rec.FullVerificationCode() = code;
          size++;
        }
      }
    }

//------------------------------------------------------------------------------
    DListC<StringC> ClaimSessionC::UniqueFaceIds() const
    {
      DListC < StringC > ret;
      unsigned int numFaceIds = m_faceIds.Size();
      for (unsigned int i = 0; i < numFaceIds; i++)
        ret.InsLast(m_faceIds.Backward(i));
      return ret;
    }

//------------------------------------------------------------------------------
    DListC<StringC> ClaimSessionC::UniqueClaimedIds() const
    {
      DListC < StringC > ret;
      unsigned int numClaimIds = m_claimedIds.Size();
      for (unsigned int i = 0; i < numClaimIds; i++)
        ret.InsLast(m_claimedIds.Backward(i));
      return ret;
    }

//------------------------------------------------------------------------------
//! Add claim
    void ClaimSessionC::AddClaim(const StringC& ClaimedId, const DListC<StringC> FaceIds, bool TrueClaim)
    {
      //map ids
      unsigned int claimedIdCode = m_claimedIds.Forward(ClaimedId);

      SArray1dC < UIntT > localFaceIds(FaceIds.Size());
      int i = 0;
      for (ConstDLIterC < StringC > it(FaceIds); it; it++, i++) {
        localFaceIds[i] = GetFaceIdCode(*it);
      }

      UIntT localVerId = GetVerificationIdCode(localFaceIds);

      AddClaim(claimedIdCode, localVerId, TrueClaim);
    }

//------------------------------------------------------------------------------
    UIntT ClaimSessionC::NoTrueClaims() const
    {
      UIntT n = 0;
      for (DArray1dIterC < DArray1dC<ClaimSessionRecordC> > it1(m_claimsSet); it1; it1++) {
        for (DArray1dIterC < ClaimSessionRecordC > it2(*it1); it2; it2++) {
          if (it2->TrueClaim())
            n++;
        }
      }
      return n;
    }

//------------------------------------------------------------------------------
    UIntT ClaimSessionC::NoImpostorAttacks() const
    {
      UIntT n = 0;
      for (DArray1dIterC < DArray1dC<ClaimSessionRecordC> > it1(m_claimsSet); it1; it1++) {
        for (DArray1dIterC < ClaimSessionRecordC > it2(*it1); it2; it2++) {
          if (!it2->TrueClaim())
            n++;
        }
      }
      return n;
    }

//------------------------------------------------------------------------------
    void ClaimSessionC::TrueClaims(ClaimSessionC &Result) const
    {
      HashC<UIntT, UIntT> varificationCodeMap, claimCodeMap;
      for (DArray1dIterC < DArray1dC<ClaimSessionRecordC> > it1(m_claimsSet); it1; it1++) {
        for (DArray1dIterC < ClaimSessionRecordC > it2(*it1); it2; it2++) {
          const ClaimSessionRecordC& rec = *it2;
          if (rec.TrueClaim()) {
            UIntT newClaimCode = 0xffffffff, newVerCode = 0xffffffff;
            if (!claimCodeMap.Lookup(it1.Index().V(), newClaimCode)) {
              newClaimCode = Result.GetClaimedIdCode(m_claimedIds.Backward(it1.Index().V()));
              claimCodeMap[it1.Index().V()] = newClaimCode;
            }

            const UIntT verCode = rec.VerificationCode();
            if (!varificationCodeMap.Lookup(verCode, newVerCode)) {
              const SArray1dC<UIntT>& curFaceSet = m_verificationSets[verCode];
              SArray1dC < UIntT > newFaceSet = SArray1dC < UIntT > (curFaceSet.Size());
              for (SArray1dIter2C < UIntT, UIntT > it3(newFaceSet, curFaceSet); it3; it3++) {
                it3.Data1() = Result.GetFaceIdCode(m_faceIds.Backward(it3.Data2()));
              }
              newVerCode = Result.GetVerificationIdCode(newFaceSet);
            }

            Result.AddClaim(newClaimCode, newVerCode, rec.TrueClaim());
          }
        }
      }
    }

//------------------------------------------------------------------------------
    void ClaimSessionC::ImpostorAttacks(ClaimSessionC &Result) const
    {
      HashC<UIntT, UIntT> varificationCodeMap, claimCodeMap;
      for (DArray1dIterC < DArray1dC<ClaimSessionRecordC> > it1(m_claimsSet); it1; it1++) {
        for (DArray1dIterC < ClaimSessionRecordC > it2(*it1); it2; it2++) {
          const ClaimSessionRecordC& rec = *it2;
          if (!rec.TrueClaim()) {
            UIntT newClaimCode = 0xffffffff, newVerCode = 0xffffffff;
            if (!claimCodeMap.Lookup(it1.Index().V(), newClaimCode)) {
              newClaimCode = Result.GetClaimedIdCode(m_claimedIds.Backward(it1.Index().V()));
              claimCodeMap[it1.Index().V()] = newClaimCode;
            }

            const UIntT verCode = rec.VerificationCode();
            if (!varificationCodeMap.Lookup(verCode, newVerCode)) {
              const SArray1dC<UIntT>& curFaceSet = m_verificationSets[verCode];
              SArray1dC < UIntT > newFaceSet = SArray1dC < UIntT > (curFaceSet.Size());
              for (SArray1dIter2C < UIntT, UIntT > it3(newFaceSet, curFaceSet); it3; it3++) {
                it3.Data1() = Result.GetFaceIdCode(m_faceIds.Backward(it3.Data2()));
              }
              newVerCode = Result.GetVerificationIdCode(newFaceSet);
            }

            Result.AddClaim(newClaimCode, newVerCode, rec.TrueClaim());
          }
        }
      }
    }

//------------------------------------------------------------------------------
    /*void ClaimSessionC::Pick(ClaimSessionC &Result, UIntT n) const
     {
     //    CollectionC<ClaimC>c(*this);
     ClaimSessionC s;
     if(n>Size()) n = Size();
     for(UIntT i=0;i<n;i++) s.InsLast(c.Pick());
     return s;
     }*/

//------------------------------------------------------------------------------
    /*void ClaimSessionC::Pick(ClaimSessionC &Result,
     UIntT TrueClaims, UIntT ImpostorAttacks) const
     {
     unsigned int totalCli = NoTrueClaims();
     unsigned int totalImp = NoImpostorAttacks();

     if(TrueClaims > totalCli)
     TrueClaims = totalCli;

     if(ImpostorAttacks > totalImp)
     ImpostorAttacks = totalImp;

     unsigned int skipCli = (unsigned int)(Random1() * totalCli / TrueClaims);
     unsigned int skipImp = (unsigned int)(Random1() * totalImp / ImpostorAttacks);

     for(DArray1dIterC<DArray1dC<ClaimSessionRecordC> > it1(m_claimsSet); it1; it1++)
     {
     StringC claimedId = m_claimedIds.Backward(it1.Index().V());
     for(DArray1dIterC<ClaimSessionRecordC> it2(*it1); it2; it2++)
     {
     const ClaimSessionRecordC& rec = *it2;
     if(rec.TrueClaim())
     {
     if(skipCli != 0)
     skipCli--;
     else
     {
     Result.AddClaim(claimedId, m_faceIds.Backward(rec.FaceIdCode()),
     true, rec.Key());
     totalCli--;
     TrueClaims--;
     skipCli = TrueClaims != 0 ?
     (unsigned int)(Random1() * totalCli / TrueClaims) : 0xffffffff;
     }
     }
     else
     {
     if(skipImp != 0)
     skipImp--;
     else
     {
     Result.AddClaim(claimedId, m_faceIds.Backward(rec.FaceIdCode()),
     false, rec.Key());
     totalImp--;
     ImpostorAttacks--;
     skipImp = ImpostorAttacks != 0 ?
     (unsigned int)(Random1() * totalImp / ImpostorAttacks) : 0xffffffff;
     }
     }
     }
     }
     }
     */
//----------------------------------------------------------------------------
//! Get claimed id code
    UIntT ClaimSessionC::GetClaimedIdCode(const StringC& ClaimedId)
    {
      return m_claimedIds.Forward(ClaimedId);
    }

//----------------------------------------------------------------------------
//! Get face id code
    UIntT ClaimSessionC::GetFaceIdCode(const StringC& FaceId)
    {
      return m_faceIds.Forward(FaceId);
    }

//----------------------------------------------------------------------------
//! Get face id from code
    const StringC& ClaimSessionC::GetFaceId(UIntT FaceCode) const
    {
      return m_faceIds.Backward(FaceCode);
    }

//----------------------------------------------------------------------------
//! Get verification id code
    UIntT ClaimSessionC::GetVerificationIdCode(const SArray1dC<UIntT> &FaceIdCodes)
    {
      UIntT verId;
      if (FindVerificationCode(verId, FaceIdCodes))
        return verId;

      return AddVerificationCode(FaceIdCodes);
    }

//----------------------------------------------------------------------------
//! Get set of image codes from verification id code
    const SArray1dC<UIntT>& ClaimSessionC::GetVerificationSet(UIntT VerificationCode) const
    {
      return m_verificationSets[VerificationCode];
    }

//----------------------------------------------------------------------------
//! Add claim
    void ClaimSessionC::AddClaim(UIntT ClaimedIdCode, UIntT VerificationIdCode, bool TrueClaim)
    {
      //check if we need to extend claims array
      while (ClaimedIdCode >= m_claimsSet.Size()) {
        m_claimsSet.Append(DArray1dC<ClaimSessionRecordC>());
      }

      //get set of records for current claimed Id
      DArray1dC < ClaimSessionRecordC > &curClaims = m_claimsSet[ClaimedIdCode];

      //append claim
      curClaims.Append(ClaimSessionRecordC(VerificationIdCode, TrueClaim));
      size++;
    }

//----------------------------------------------------------------------------
//! Find verification code for the set of faceIds
    bool ClaimSessionC::FindVerificationCode(UIntT &VerId, const SArray1dC<UIntT> &FaceIdCodes) const
    {
      for (DArray1dIterC < SArray1dC<UIntT> > it1(m_verificationSets); it1; it1++) {
        if (it1->Size() != FaceIdCodes.Size())
          continue;
        bool match = true;
        for (SArray1dIter2C < UIntT, UIntT > it2(*it1, FaceIdCodes); it2; it2++)
          if (it2.Data1() != it2.Data2()) {
            match = false;
            break;
          }

        if (match) {
          VerId = it1.Index().V();
          return true;
        }
      }
      return false;
    }

//----------------------------------------------------------------------------
    UIntT ClaimSessionC::AddVerificationCode(const SArray1dC<UIntT> &FaceIdCodes)
    {
      m_verificationSets.Append(FaceIdCodes.Copy());
      return m_verificationSets.Size() - 1;
    }

//----------------------------------------------------------------------------
    XMLIStreamC &operator>>(XMLIStreamC &Xml, ClaimSessionC &Data)
    {
      Data.LoadXML(Xml);
      return Xml;
    }

//----------------------------------------------------------------------------
    XMLOStreamC &operator<<(XMLOStreamC &Xml, const ClaimSessionC& Data)
    {
      Data.SaveXML(Xml);
      return Xml;
    }

//----------------------------------------------------------------------------
    BinIStreamC &operator>>(BinIStreamC &Strm, ClaimSessionC &Data)
    {
      Data.Load(Strm);
      return Strm;
    }

//----------------------------------------------------------------------------
    BinOStreamC &operator<<(BinOStreamC &Strm, const ClaimSessionC& Data)
    {
      Data.Save(Strm);
      return Strm;
    }
//----------------------------------------------------------------------------

    void InitClaimSessionFormat()
    {
    }
    static TypeNameC typeNameClaimSessionC(typeid(ClaimSessionC), "FaceN::ClaimSessionC");
    FileFormatXMLStreamC<ClaimSessionC> FileFormatXMLStream_ClaimSessionC;

//RAVL_INITVIRTUALCONSTRUCTOR_FULL_NAMED(niformLBPNormaliseBodyC,UniformLBPNormaliseC,FaceNormAbstractC,"UniformLBPNormalise");
//FileFormatStreamC <UniformLBPNormaliseC> FileFormatStream_UniformLBPNormaliseC;
    FileFormatBinStreamC<ClaimSessionC> FileFormatBinStream_ClaimSessionC;

  }
}

