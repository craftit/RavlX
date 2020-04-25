// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! author="Jos Hickson"
//! lib=RavlCrypto
//! date="15/07/2004"
//! file="Ravl/Contrib/Crypto/MCrypt.cc"

#include "Ravl/Crypto/MCrypt.hh"

// Other headers

#define DODEBUG 0

#if DODEBUG
#  define ONDEBUG(x) x
#else
#  define ONDEBUG(x)
#endif

using namespace RavlN;



void MCryptC::Initialise(const void *key, const UIntT keyLength, const void *initialValue) {
  if (dMcryptHandle == NULL) {
    throw ExceptionC("MCryptC::Initialise() - invalid handle!");
  }

  if (dInitialised) {
    Reset();
  }

  // Check the key length and that the initial value is non-NULL if we need an initial value
  if (keyLength > static_cast<UIntT>(mcrypt_enc_get_key_size(dMcryptHandle))) {
    throw ExceptionC("MCryptC::Initialise() - key too long for this algorithm!");
  }

  if (mcrypt_enc_mode_has_iv(dMcryptHandle) == 1 && initialValue == NULL) {
    throw ExceptionC("MCryptC::Initialise() - initial value is missing!");
  }

  // Do the initialisation
  int retval = mcrypt_generic_init(dMcryptHandle, const_cast<void *>(key), keyLength, const_cast<void *>(initialValue));

  if (retval != 0) {
    StringC errStr(mcrypt_strerror(retval));
    errStr.prepend("MCryptC::Reset() - error calling mcrypt_generic_deinit(): ");
    throw ExceptionC(errStr);
  }

  dInitialised = true;
}



void MCryptC::Reset() {
  if (dMcryptHandle == NULL) {
    throw ExceptionC("MCryptC::Reset() - invalid handle!");
  }

  if (dInitialised) {
    int retval = mcrypt_generic_deinit(dMcryptHandle);

    if (retval != 0) {
      StringC errStr(mcrypt_strerror(retval));
      errStr.prepend("MCryptC::Reset() - error calling mcrypt_generic_deinit(): ");
      throw ExceptionC(errStr);
    }

    dInitialised = false;
    dCryptType = CT_None;
  }
}



void MCryptC::Encrypt(void *text, UIntT length) {
  if (dMcryptHandle == NULL) {
    throw ExceptionC("MCryptC::Encrypt() - invalid handle!");
  } else if (!dInitialised) {
    throw ExceptionC("MCryptC::Encrypt() - not initialised (use MCryptC::Initialise())!");
  } else if (dCryptType == CT_Decryption) {
    throw ExceptionC("MCryptC::Encrypt() - already used for decryption - cannot use for encryption too!  Re-initialise first.");
  }

  if (mcrypt_generic(dMcryptHandle, text, length) != 0) {
    throw ExceptionC("MCryptC::Encrypt() - encryption error!");
  }

  // Mark this as being an encryptor
  if (dCryptType == CT_None) {
    dCryptType = CT_Encryption;
  }
}



void MCryptC::Decrypt(void *text, UIntT length) {
  if (dMcryptHandle == NULL) {
    throw ExceptionC("MCryptC::Decrypt() - invalid handle!");
  } else if (!dInitialised) {
    throw ExceptionC("MCryptC::Decrypt() - not initialised (use MCryptC::Initialise())!");
  } else if (dCryptType == CT_Encryption) {
    throw ExceptionC("MCryptC::Decrypt() - already used for encryption - cannot use for decryption too!  Re-initialise first.");
  }

  if (mcrypt_generic(dMcryptHandle, text, length) != 0) {
    throw ExceptionC("MCryptC::Encrypt() - decryption error!");
  }

  // Mark this as being an encryptor
  if (dCryptType == CT_None) {
    dCryptType = CT_Decryption;
  }
}



void MCryptC::Crypt3DESCBC(const ByteT *key, const UIntT keyLength, const ByteT *initialValue,
                           ByteT *text, UIntT numBlocks, const CryptTypeT cryptType)
{
  // Create the encryptor and initialise it with the initial value and the key
  MCryptC tripleDESCBC(MCRYPT_3DES, MCRYPT_CBC);
  tripleDESCBC.Initialise(static_cast<const void *>(key), keyLength, static_cast<const void *>(initialValue));

  if (cryptType == CT_Encryption) {
    tripleDESCBC.Encrypt(static_cast<void *>(text), numBlocks * 8);
  } else if (cryptType == CT_Decryption) {
    tripleDESCBC.Decrypt(static_cast<void *>(text), numBlocks * 8);
  } else {
    throw ExceptionC("MCryptC::Crypt3DESCBC() - invalid crypt type!");
  }
}



void MCryptC::Crypt3DESECB(const ByteT *key, const UIntT keyLength, ByteT *text, UIntT numBlocks, const CryptTypeT cryptType) {
  // Create the encryptor and initialise it with the initial value and the key
  MCryptC tripleDESECB(MCRYPT_3DES, MCRYPT_ECB);
  tripleDESECB.Initialise(static_cast<const void *>(key), keyLength, NULL);

  if (cryptType == CT_Encryption) {
    tripleDESECB.Encrypt(static_cast<void *>(text), numBlocks * 8);
  } else if (cryptType == CT_Decryption) {
    tripleDESECB.Decrypt(static_cast<void *>(text), numBlocks * 8);
  } else {
    throw ExceptionC("MCryptC::Crypt3DESEBC() - invalid crypt type!");
  }
}

