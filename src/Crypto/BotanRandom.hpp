#ifndef DISSENT_CRYPTO_BOTAN_RANDOM_H_GUARD
#define DISSENT_CRYPTO_BOTAN_RANDOM_H_GUARD

#include <QScopedPointer>

#include <botan/rng.h>
#include <botan/x931_rng.h>
#include <botan/auto_rng.h>
#include <botan/aes.h>
#define	BOTAN_AES	16 /*128-bit*/

#include "../Utils/Random.hpp"

using namespace Botan;

namespace Dissent {
namespace Crypto {
  /**
   * Implementation of Random using Botan
   */
  class BotanRandom : public Dissent::Utils::Random {
    public:
      /**
       * Constructor
       * @param seed optional seed
       */
      BotanRandom(const QByteArray &seed = QByteArray(), uint index = 0);

      /**
       * Destructor
       */
      virtual ~BotanRandom() {}

      /**
       * Returns the optimal seed size, less than will provide suboptimal
       * results and greater than will be compressed into the chosen seed.
       */
      static uint OptimalSeedSize() { return BOTAN_AES; /*Botan::AES_128::minimum_keylength();*/ }

      virtual int GetInt(int min = 0, int max = RAND_MAX);
      virtual void GenerateBlock(QByteArray &data);
      Botan::RandomNumberGenerator *GetHandle() { return _rng.data(); }
    private:
      QScopedPointer<Botan::RandomNumberGenerator> _rng;
  };
}
}

#endif
