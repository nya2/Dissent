#include "BotanRandom.hpp"

namespace Dissent {
namespace Crypto {
  BotanRandom::BotanRandom(const QByteArray &seed, uint index)
  {
    if(seed.isEmpty()) {
      _rng.reset(new Botan::AutoSeeded_RNG());
      //_rng.reset(new CryptoPP::AutoSeededX917RNG<CryptoPP::AES>());
      return;
    }

    QByteArray seed_tmp(seed);
    seed_tmp.resize(BOTAN_AES);

    AES_128 cipher;
    cipher.encrypt(reinterpret_cast<byte *>(seed_tmp.data()));
    cipher.set_key(reinterpret_cast<byte *>(seed_tmp.data()), seed_tmp.size());

    _rng.reset(new ANSI_X931_RNG(cipher.clone(), (new Botan::Randpool())->make_rng()));

    if(index) {
      MoveRngPosition(index);
    }
  }

  int BotanRandom::GetInt(int min, int max)
  {
    if(min == max) {
      return min;
    }
    IncrementByteCount(4);
    SecureVector<byte> output = _rng->random_vec(4);
    int randomInt = (int) output[3];
    for(int i = 2; i >= 0; i--) {
	randomInt = (randomInt << 8) + (int) output[i];
    }

    /* TODO: What if min-max are in a negative range? Issue? */
    return ((randomInt % (max - 1 - min)) + min);
  }

  void BotanRandom::GenerateBlock(QByteArray &data)
  {
    _rng->randomize(reinterpret_cast<byte *>(data.data()), (size_t) data.size());
    IncrementByteCount(data.size());
  }
}
}
