// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_CLAIMSESSION_HEADER
#define RAVLN_FACEN_CLAIMSESSION_HEADER 1
//! rcsid="$Id$"
//! lib="RavlFace"
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/ClaimSession.hh"
//! author="Kieron J Messer"

#include "Ravl/TwoWayMap.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/DList.hh"
#include "Ravl/Face/Claim.hh"
#include "Ravl/DArray1d.hh"
#include "Ravl/DArray1dIter.hh"
#include "Ravl/OS/Filename.hh"

namespace RavlN {
  namespace FaceN {

    class FaceInfoDbC;

    using namespace RavlN;

    //----------------------------------------------------------------------------
    class ClaimSessionRecordC
    {
    public:
      ClaimSessionRecordC() :
          verificationCode(0)
      {
      }

      ClaimSessionRecordC(unsigned int VerificationCode, bool TrueClaim) :
          verificationCode(VerificationCode | (TrueClaim ? 0x80000000 : 0x0))
      {
      }

      bool TrueClaim() const
      {
        return (verificationCode & 0x80000000) != 0;
      }

      unsigned int VerificationCode() const
      {
        return verificationCode & 0x7fffffff;
      }

      //get full code including true claim bit
      unsigned int& FullVerificationCode()
      {
        return verificationCode;
      }

      //get full code including true claim bit
      const unsigned int& FullVerificationCode() const
      {
        return verificationCode;
      }
    private:
      unsigned int verificationCode; //highest bit used to mark true claim
    };

    class ClaimSessionIterC;

    //----------------------------------------------------------------------------
    //! A set of ClaimC with XML streams
    class ClaimSessionC
    {
    public:
      //! Constructor
      ClaimSessionC();

      //! Constructor from XML stream
      ClaimSessionC(XMLIStreamC& xml);

      //! Load from file
      bool Load(const FilenameC &Filename);

      //! Save to xml stream
      void SaveXML(XMLOStreamC &Xml) const;

      //! Save to binary stream
      void Save(BinOStreamC &Stream) const;

      //! Load from xml stream (adds loaded claims to the set)
      void LoadXML(XMLIStreamC &Xml);

      //! Load from binary stream (adds loaded claims to the set)FIXME does not add
      void Load(BinIStreamC &Stream);

      //! Add claim
      void AddClaim(const StringC& ClaimedId, const DListC<StringC> FaceIds, bool TrueClaim);

      //! Get number of claimed ids
      UIntT GetNumberOfClaimedIds() const
      {
        return m_claimsSet.Size();
      }

      //! Get number of claims for specific claimed Id
      UIntT GetNumberOfClaims(const StringC& ClaimedId) const;

      //! Get number of verification ids
      UIntT GetNumberOfVerificationIds() const
      {
        return m_verificationSets.Size();
      }

      //! Get a list of all the unique faces in session
      DListC<StringC> UniqueFaceIds() const;

      //! Get a list of all the unique claimed ids
      DListC<StringC> UniqueClaimedIds() const;

      //! The number of true claims in the claim session
      UIntT NoTrueClaims() const;

      //! The number of impostor attacks in the claim session
      UIntT NoImpostorAttacks() const;

      //! Get number of verification ids
      UIntT NoVerificationIds() const
      {
        return m_verificationSets.Size();
      }

      //Tuple2C<RealT, RealT> FaFr() const;
      //: Work out number of false acceptances and number of false rejections

      // Get just the true claims
      void TrueClaims(ClaimSessionC &Result) const;

      // Get just the impostor attacks
      void ImpostorAttacks(ClaimSessionC &Result) const;

      // Pick a random sample of n claims
      //void Pick(ClaimSessionC &Result, UIntT n) const;

      //! Pick a random sample of claims
      //void Pick(ClaimSessionC &Result, UIntT TrueClaims, UIntT ImpostorAttacks) const;

      UIntT Size() const
      {
        return size;
      }

      //  ************************ low level functions ***************************
      //! Get claimed id code
      UIntT GetClaimedIdCode(const StringC& ClaimedId);

      //! Get face id code
      UIntT GetFaceIdCode(const StringC& FaceId);

      //! Get face id from code
      const StringC& GetFaceId(UIntT FaceCode) const;

      //! Get verification id code
      UIntT GetVerificationIdCode(const SArray1dC<UIntT> &FaceIdCodes);

      //! Get set of image codes from verification id code
      const SArray1dC<UIntT>& GetVerificationSet(UIntT VerificationCode) const;

      //! Add claim
      void AddClaim(UIntT ClaimedIdCode, UIntT VerificationIdCode, bool TrueClaim);

      friend class ClaimSessionIterC;
    protected:
      //! Find verification code for the set of faceIds
      bool FindVerificationCode(UIntT &VerId, const SArray1dC<UIntT> &FaceIdCodes) const;

      //! Add verification code for set of face ids
      UIntT AddVerificationCode(const SArray1dC<UIntT> &FaceIdCodes);

      RavlN::TwoWayMapC<StringC> m_claimedIds; //!< map claimed ids to codes
      RavlN::TwoWayMapC<StringC> m_faceIds; //!< map faceIds to codes

      //! map set of verification faceIds to verification code
      DArray1dC<SArray1dC<UIntT> > m_verificationSets;

      //! claims table (first index  is claim code, second is verification code)
      DArray1dC<DArray1dC<ClaimSessionRecordC> > m_claimsSet;

      UIntT size;
    };

    //----------------------------------------------------------------------------
    class ClaimSessionIterC
    {
    public:
      //! constructor
      ClaimSessionIterC(const ClaimSessionC& ClaimSession, const FaceInfoDbC& FaceInfoDb);

      //! is pointing to a valid element
      operator bool() const
      {
        return m_valid;
      }

      //! Move current position to the beginning
      void First();

      //! goto next claim returns true if still same claimId
      bool Next();

      //! goto next claim
      void operator++();

      //! goto next claim
      void operator++(int);

      const ClaimC& operator*() const;

      const ClaimC* operator->() const;

      void Get(StringC &ClaimId, SArray1dC<StringC> &FaceIds, bool &TrueClaim) const;

      void Get(UIntT &ClaimCode, UIntT &VerCode, bool &TrueClaim) const;

    protected:
      const ClaimSessionC& m_claimSession;
      const FaceInfoDbC& m_faceInfoDb;

      //position in claimSet
      DArray1dIterC<DArray1dC<ClaimSessionRecordC> > m_it1;
      DArray1dIterC<ClaimSessionRecordC> m_it2;
      bool m_valid; //valid pointer

      //cache
      mutable ClaimC m_cache;
      mutable bool m_cacheValid;
    };

    XMLIStreamC &operator>>(XMLIStreamC &Xml, ClaimSessionC &Data);
    XMLOStreamC &operator<<(XMLOStreamC &Xml, const ClaimSessionC &Data);

    BinIStreamC &operator>>(BinIStreamC &Xml, ClaimSessionC &Data);
    BinOStreamC &operator<<(BinOStreamC &Xml, const ClaimSessionC &Data);
  //----------------------------------------------------------------------------

  }
}

#endif
