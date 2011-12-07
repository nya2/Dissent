#ifndef DISSENT_CRYPTO_LIBRARY_H_GUARD
#define DISSENT_CRYPTO_LIBRARY_H_GUARD

#include "../Utils/Random.hpp"
#include "AsymmetricKey.hpp"
#include "DiffieHellman.hpp"
#include "Hash.hpp"
#include "IntegerData.hpp"

namespace Dissent {
namespace Crypto {
  class Library {
    public:
      /**
       * Load a public key from a file
       */
      virtual AsymmetricKey *LoadPublicKeyFromFile(const QString &filename);

      /**
       * Loading a public key from a byte array
       */
      virtual AsymmetricKey *LoadPublicKeyFromByteArray(const QByteArray &data);

      /**
       * Generate a public key using the given data as a seed to a RNG
       */
      virtual AsymmetricKey *GeneratePublicKey(const QByteArray &seed) ;

      /**
       * Load a private key from a file
       */
      virtual AsymmetricKey *LoadPrivateKeyFromFile(const QString &filename);

      /**
       * Loading a private key from a byte array
       */
      virtual AsymmetricKey *LoadPrivateKeyFromByteArray(const QByteArray &data);

      /**
       * Generate a private key using the given data as a seed to a RNG
       */
      virtual AsymmetricKey *GeneratePrivateKey(const QByteArray &seed);

      /**
       * Generates a unique (new) private key
       */
      virtual AsymmetricKey *CreatePrivateKey() ;

      /**
       * Returns a random number generator
       */
      virtual Dissent::Utils::Random *GetRandomNumberGenerator(
          const QByteArray &seed = QByteArray(), uint index = 0) ;

      /**
       * Returns the optimal seed size for the RNG
       */
      virtual uint RngOptimalSeedSize() ;

      /**
       * Returns a hash algorithm
       */
      virtual Hash *GetHashAlgorithm() ;

      /**
       * Returns an integer data
       */
      virtual IntegerData *GetIntegerData(int value);

      /**
       * Returns an integer data
       */
      virtual IntegerData *GetIntegerData(const QByteArray &value);

      /**
       * Returns an integer data
       */
      virtual IntegerData *GetIntegerData(const QString &value) ;

      /**
       * Returns a DiffieHellman operator
       */
      virtual DiffieHellman *CreateDiffieHellman();

      /**
       * Generate a DiffieHellman operator using the given data as a seed to a RNG
       * @param seed seed used to generate the DiffieHellman exchange
       */
      virtual DiffieHellman *GenerateDiffieHellman(const QByteArray &seed) ;

      /**
       * Loads a DiffieHellman key from a byte array
       * @param private_component the private component in the DH exchange
       */
      virtual DiffieHellman *LoadDiffieHellman(const QByteArray &private_component) ;

      /**
       * Destructor
       */
      virtual ~Library() {}
  };
}
}

#endif
