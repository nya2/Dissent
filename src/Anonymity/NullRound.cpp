#include "../Connections/Network.hpp"

#include "NullRound.hpp"

namespace Dissent {
namespace Anonymity {
  NullRound::NullRound(QSharedPointer<GroupGenerator> group_gen,
      const Credentials &creds, const Id &round_id,
      QSharedPointer<Network> network, GetDataCallback &get_data) :
    Round(group_gen, creds, round_id, network, get_data)
  {
  }

  bool NullRound::Start()
  {
    if(!Round::Start()) {
      return false;
    }

    QPair<QByteArray, bool> data = GetData(1024);
    GetNetwork()->Broadcast(data.first);
    return true;
  }

  void NullRound::ProcessData(const QByteArray &data, const Id &id)
  {
    if(_received_from.contains(id)) {
      qWarning() << "Receiving a second message from: " << id.ToString();
      return;
    }
    _received_from.append(id);

    if(!data.isEmpty()) {
      PushData(data, this);
    }

    if(_received_from.count() == GetGroup().Count()) {
      SetSuccessful(true);
      Stop("Round successfully finished.");
    }
  }
}
}
