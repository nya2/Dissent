#ifndef DISSENT_CRYPTO_BOTAN_LIBRARY_H_GUARD
#define DISSENT_CRYPTO_BOTAN_LIBRARY_H_GUARD

//#include "BotanDiffieHellman.hpp"
//#include "BotanHash.hpp"
//#include "BotanIntegerData.hpp"
#include "BotanRandom.hpp"
//#include "BotanPrivateKey.hpp"
//#include "BotanPublicKey.hpp"

#include "Library.hpp"

namespace Dissent {
namespace Crypto {
  class BotanLibrary : public Library {
    public:
      /**
       * Load a public key from a file
       */
/*      inline virtual AsymmetricKey *LoadPublicKeyFromFile(const QString &filename)
      {
        return new BotanPublicKey(filename);
      }
*/
      /**
       * Loading a public key from a byte array
       */
/*      inline virtual AsymmetricKey *LoadPublicKeyFromByteArray(const QByteArray &data) 
      {
        return new BotanPublicKey(data);
      }
*/
      /**
       * Generate a public key using the given data as a seed to a RNG
       */
/*      inline virtual AsymmetricKey *GeneratePublicKey(const QByteArray &seed) 
      {
        return BotanPublicKey::GenerateKey(seed);
      }

*/
      /**
       * Load a private key from a file
       */
/*      inline virtual AsymmetricKey *LoadPrivateKeyFromFile(const QString &filename) 
      {
        return new BotanPrivateKey(filename);
      }

*/
      /**
       * Loading a private key from a byte array
       */
/*      inline virtual AsymmetricKey *LoadPrivateKeyFromByteArray(const QByteArray &data) 
      {
        return new BotanPrivateKey(data);
      }

*/
      /**
       * Generate a private key using the given data as a seed to a RNG
       */
/*      inline virtual AsymmetricKey *GeneratePrivateKey(const QByteArray &seed) 
      {
        return BotanPrivateKey::GenerateKey(seed);
      }

*/
      /**
       * Generates a unique (new) private key
       */
/*      inline virtual AsymmetricKey *CreatePrivateKey() 
      {
        return new BotanPrivateKey();
      }

*/
      /**
       * Returns a deterministic random number generator
       */
      inline virtual Dissent::Utils::Random *GetRandomNumberGenerator(const QByteArray &seed, uint index)
      {
        return new BotanRandom(seed, index);
      }

      inline virtual uint RngOptimalSeedSize()
      {
        return BotanRandom::OptimalSeedSize();
      }

      /**
       * Returns a hash algorithm
       */
/*      inline virtual Hash *GetHashAlgorithm() 
      {
        return new BotanHash();
      }

*/
      /**
       * Returns an integer data
       */
/*      inline virtual IntegerData *GetIntegerData(int value)
      {
        return new BotanIntegerData(value);
      }

*/
      /**
       * Returns an integer data
       */
/*      inline virtual IntegerData *GetIntegerData(const QByteArray &value)
      {
        return new BotanIntegerData(value);
      }

*/
      /**
       * Returns an integer data
       */
/*      inline virtual IntegerData *GetIntegerData(const QString &value)
      {
        return new BotanIntegerData(value);
      }

*/
      /**
       * Returns a DiffieHellman operator
       */
/*      virtual DiffieHellman *CreateDiffieHellman()
      {
        return new BotanDiffieHellman();
      }

*/
      /**
       * Generate a DiffieHellman operator using the given data as a seed to a RNG
       * @param seed seed used to generate the DiffieHellman exchange
       */
/*      virtual DiffieHellman *GenerateDiffieHellman(const QByteArray &seed)
      {
        return new BotanDiffieHellman(seed, true);
      }

*/
      /**
       * Loads a DiffieHellman key from a byte array
       * @param private_component the private component in the DH exchange
       */
/*      virtual DiffieHellman *LoadDiffieHellman(const QByteArray &private_component)
      {
        return new BotanDiffieHellman(private_component);
      }*/
  };
}
}

#endif
