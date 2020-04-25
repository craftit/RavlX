// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MCRYPT_HEADER
#define RAVL_MCRYPT_HEADER 1
//! rcsid="$Id$"
//! author="Jos Hickson"
//! lib=RavlCrypto
//! date="15/07/2004"
//! file="Ravl/Contrib/Crypto/MCrypt.hh"
//! docentry="Ravl.API.Core.Cryptography"
//! userlevel=Normal

#include <mcrypt.h>
#include "Ravl/Exception.hh"
#include "Ravl/String.hh"

namespace RavlN {

  //! userlevel=Normal
  //: This class is essentially a simplifying wrapper around the <i>libmcrypt</i> encryption/decryption
  //: library API.  For details on how <i>libmcrypt</i> works see the mcrypt(3) man page.<p>
  //
  //  This class does not support the use of algorithms not provided by the libmcrypt package - that is
  //  it does not support dynamic module loading.  To use that you will need to use the <i>libmcrypt</i>
  //  library directly, assuming that supports dynamic module loading.<p>
  //
  //  The general usage pattern would be:
  //  <ul>
  //  <li> Create an MCryptC object.
  //  <li> Initialise the object with a key and initial value (IV), if necessary, using Initialise().
  //  <li> Use Encrypt() or Decrypt().  These can be called repeatedly but once Encrypt() has been called
  //       you cannot call Decrypt() and <i>visa versa</i>.
  //  </ul>
  //
  //  The object can be reused by calling Initialise() again (this will call Reset() before running through
  //  the initialisation process).<p>
  //
  //  Any error returned by a call to an mcrypt function results in an ExceptionC being thrown.
  
  class MCryptC {
    public:
      enum CryptTypeT {
        CT_Encryption,
        CT_Decryption,
        CT_None
      };
      //: Defines whether this does encryption or decryption or neither (the initial, obviously useless, state).
      //  This is implicitly set when Encrypt() or Decrypt() is used for the first time after a call to
      //  Initialise().

      MCryptC()
        : dMcryptHandle(NULL)
      { }
      //: Constructor - creates invalid handle

      MCryptC(char *algorithm, char *mode)
        : dMcryptHandle(mcrypt_module_open(algorithm, NULL, mode, NULL)), dInitialised(false), dCryptType(CT_None)
      {
        // Check for failure
        if (dMcryptHandle == MCRYPT_FAILED) {
          throw ExceptionC("MCryptC():: Failed to open mcrypt module!");
        }
      }
      //: Constructor
      //!param: algorithm - the name of the algorithm to use
      //!param: mode - the mode to use
      //
      //  <i>libmcrypt</i> defines the following algorithms (use the string or the definition for
      //  the algorithm parameter):
      //
      //  <ul>
      //  <li> MCRYPT_BLOWFISH		"blowfish"
      //  <li> MCRYPT_DES 		"des"
      //  <li> MCRYPT_3DES 		"tripledes"
      //  <li> MCRYPT_3WAY 		"threeway"
      //  <li> MCRYPT_GOST 		"gost"
      //  <li> MCRYPT_SAFER_SK64 	"safer-sk64"
      //  <li> MCRYPT_SAFER_SK128 	"safer-sk128"
      //  <li> MCRYPT_CAST_128 	"cast-128"
      //  <li> MCRYPT_XTEA 		"xtea"
      //  <li> MCRYPT_RC2	 	"rc2"
      //  <li> MCRYPT_TWOFISH 		"twofish"
      //  <li> MCRYPT_CAST_256 	"cast-256"
      //  <li> MCRYPT_SAFERPLUS 	"saferplus"
      //  <li> MCRYPT_LOKI97 		"loki97"
      //  <li> MCRYPT_SERPENT 		"serpent"
      //  <li> MCRYPT_RIJNDAEL_128 	"rijndael-128"
      //  <li> MCRYPT_RIJNDAEL_192 	"rijndael-192"
      //  <li> MCRYPT_RIJNDAEL_256 	"rijndael-256"
      //  <li> MCRYPT_ENIGMA 		"enigma"
      //  <li> MCRYPT_ARCFOUR		"arcfour"
      //  <li> MCRYPT_WAKE		"wake"
      //  </ul>
      //
      //  <i>libmcrypt</i> defines the following modes (use the string or the definition for
      //  the modes parameter):<p>
      //
      //  <ul>
      //  <li> MCRYPT_CBC		"cbc"
      //  <li> MCRYPT_ECB		"ecb"
      //  <li> MCRYPT_CFB		"cfb"
      //  <li> MCRYPT_OFB		"ofb"
      //  <li> MCRYPT_nOFB		"nofb"
      //  <li> MCRYPT_STREAM		"stream"
      //  </ul>
      //
      //  These definitions are taken from mcrypt.h.<p>
      //
      //  Throws ExceptionC on error.

      ~MCryptC() {
        if (dMcryptHandle != NULL) {
          if (dInitialised) {
            mcrypt_generic_deinit(dMcryptHandle);
          }

          mcrypt_module_close(dMcryptHandle);
        }
      }
      //: Destructor

      void Initialise(const void *key, const UIntT keyLength, const void *initialValue = NULL);
      //: Initialise the encryption/decryption.
      //!param: key - string of bytes forming the key
      //!param: keyLength - length of the key in bytes (not bits)
      //!param: initialValue - initial value to use: not always required so can be NULL
      //
      //  The maximum length available for a key can be found by calling MaxKeySize().  The required size for
      //  the initial value can be obtained by calling InitialValueSize().  To find out whether or not an
      //  initial value is required call InitialValueRequired().<p>
      //
      //  If this method is called more than once then calls after the initial one will first call Reset().<p>
      //
      //!warning: Due to the nature of the libmcrypt API, we need to cast off the const-ness of these parameters.
      //
      //  Throws ExceptionC on error.

      void Reset();
      //: Undoes what Initialise() did and clears the way for a new encryption/decryption or for closing down
      //  the module.  Frees up memory.  Unlikely to be called independently as Initialise() or the
      //  destructor will do the same things if appropriate.<p>
      //
      //  Throws ExceptionC on error.

      UIntT MaxKeySize() {
        if (dMcryptHandle == NULL) {
          throw ExceptionC("MCryptC::MaxKeySize() - invalid handle!");
        }

        return mcrypt_enc_get_key_size(dMcryptHandle);
      }
      //: Get the maximum key size allowed for this algorithm.
      //
      //  Throws ExceptionC on error.

      bool InitialValueRequired() {
        if (dMcryptHandle == NULL) {
          throw ExceptionC("MCryptC::InitialValueRequired() - invalid handle!");
        }

        return (mcrypt_enc_mode_has_iv(dMcryptHandle) == 1) ? true : false;
      }
      //: Find out if an initial value is required.
      //
      //  Throws ExceptionC on error.
 
      UIntT InitialValueSize() {
        if (dMcryptHandle == NULL) {
          throw ExceptionC("MCryptC::InitialValueSize() - invalid handle!");
        }

        return mcrypt_enc_get_iv_size(dMcryptHandle);
      }
      //: Get the initial value size required for the mode or algorithm.
      //
      //  Throws ExceptionC on error.

      UIntT BlockSize() {
        if (dMcryptHandle == NULL) {
          throw ExceptionC("MCryptC::BlockSize() - invalid handle!");
        }

        return mcrypt_enc_get_block_size(dMcryptHandle);
      }
      //: Get the block size required.
      //
      //  Throws ExceptionC on error.

      void Encrypt(void *text, UIntT length);
      //: Encrypt the given text - the encrypted version is written over the given text
      //!param: text - pointer to block of plain text to be encrypted
      //!param: length - length of the block in bytes
      //
      //  Throws ExceptionC on error.

      void Decrypt(void *text, UIntT length);
      //: Decrypt the given encrypted text - the decrypted version is written over the given text
      //!param: text - pointer to block of encrypted text to be decrypted
      //!param: length - length of the block in bytes
      //
      //  Throws ExceptionC on error.

      /* Static methods */

      static void Crypt3DESCBC(const ByteT *key, const UIntT keyLength, const ByteT *initialValue,
                               ByteT *text, UIntT numBlocks, const CryptTypeT cryptType);
      //: Handy method for doing a once-off triple DES encryption or decryption in CBC (Cypher block chaining) mode. 
      //!param: key - array of bytes giving the key to use (max length = 24)
      //!param: keyLength - actual key length
      //!param: initialValue - array of bytes specifying the initial value to use
      //!param: text - array of bytes to encrypt/decrypt: result will be written back to this array
      //!param: numBlocks - length of text in 8-byte blocks: text length should be exact multiple of 8 bytes long
      //!param: cryptType - whether to encrypt or decrypt (specifying CT_None results in an exception)

      static void Crypt3DESECB(const ByteT *key, const UIntT keyLength, ByteT *text, UIntT numBlocks, const CryptTypeT cryptType);
      //: Handy method for doing a once-off triple DES encryption or decryption in ECB (Electronic Code Book) mode.
      //!param: key - array of bytes giving the key to use (max length = 24)
      //!param: keyLength - actual key length
      //!param: text - array of bytes to encrypt/decrypt: result will be written back to this array
      //!param: numBlocks - length of text in 8-byte blocks: text length should be exact multiple of 8 bytes long
      //!param: cryptType - whether to encrypt or decrypt (specifying CT_None results in an exception)


    private:
      const MCRYPT dMcryptHandle;
      bool dInitialised;
      CryptTypeT dCryptType;

  };
  
} // namespace RavlCryptN

#endif // RAVL_MCRYPT_HEADER
