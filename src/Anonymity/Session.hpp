#ifndef DISSENT_ANONYMITY_SESSION_H_GUARD
#define DISSENT_ANONYMITY_SESSION_H_GUARD

#include <QHash>
#include <QObject>
#include <QQueue>

#include "../Connections/Id.hpp"
#include "../Messaging/Filter.hpp"
#include "../Messaging/GetDataCallback.hpp"
#include "../Messaging/RpcMethod.hpp"
#include "../Utils/StartStop.hpp"

#include "Credentials.hpp"
#include "Group.hpp"
#include "GroupGenerator.hpp"
#include "Round.hpp"

namespace Dissent {
namespace Connections {
  class Network;
}

namespace Crypto {
  class AsymmetricKey;
}

namespace Messaging {
  class RpcRequest;
}

namespace Anonymity {
  /**
   * Maintains a group which is actively participating in anonymous exchanges
   */
  class Session : public QObject, public Dissent::Messaging::Filter,
      public Dissent::Utils::StartStop {
    Q_OBJECT

    public:
      typedef Dissent::Connections::Id Id;
      typedef Dissent::Connections::Network Network;
      typedef Dissent::Crypto::AsymmetricKey AsymmetricKey;
      typedef Dissent::Messaging::RpcRequest RpcRequest;
      typedef Dissent::Messaging::RpcMethod<Session> RpcMethod;
      typedef Dissent::Messaging::GetDataMethod<Session> GetDataCallback;

      /**
       * Constructor
       * @param group an ordered member of peers for the group
       * @param creds the local nodes credentials
       * @param leader_id the Id of the leader
       * @param session_id Id for the session
       * @param network handles message sending
       * @param create_round a callback for creating a secure round
       * @param group_generator generates a subgroup of the primary group for
       * use in the round
       */
      Session(const Group &group, const Credentials &creds, const Id &leader_id,
          const Id &session_id, QSharedPointer<Network> network,
          CreateRound create_round, CreateGroupGenerator group_generator);

      /**
       * Deconstructor
       */
      virtual ~Session() {}

      /**
       * Starts the session
       */
      virtual bool Start();

      /**
       * Stops the session
       */
      virtual bool Stop();

      /**
       * From the SessionManager, pass in a ReceiveReady
       * @param request The request from a group member
       */
      void ReceivedReady(RpcRequest &request);

      /**
       * From the SessionManager, pass in incoming data
       * @param notification The message containing the data
       */
      void IncomingData(RpcRequest &notification);

      /**
       * From a client software, send a message anonymously
       */
      virtual void Send(const QByteArray &data);

      /**
       * Returns true if the peer is the leader for this session
       */
      inline bool IsLeader() { return _creds.GetLocalId() == _leader_id; }

      /**
       * Returns the Session Id
       */
      inline const Id &GetId() const { return _session_id; }

      /**
       * Returns the current round
       */
      inline QSharedPointer<Round> GetCurrentRound() { return _current_round; }

      /**
       * Returns the Session / Round information
       */
      inline virtual QString ToString() const
      {
        return "Session: " + GetId().ToString() + "|" +
          (_current_round.isNull() ? "No current round" : 
          _current_round->ToString());
      }

      /**
       * Returns the underlying GroupGenerator
       */
      inline const GroupGenerator &GetGroupGenerator() { return *_generate_group; }

    signals:
      /**
       * Signals that a round has completed.  The round will be deleted after
       * the signal has returned.
       * @param round _current_round
       */
      void RoundFinished(QSharedPointer<Round> round);

      /**
       * Signfies that the session has been closed / stopped
       */
      void Stopping();

    private:
      /**
       * Checks to see if the leader has received all the Ready messsages and
       * broadcasts responses if it has.
       */
      bool LeaderReady();

      /**
       * Called to start the next Round
       */
      void NextRound();

      /**
       * Retrieves data from the data waiting queue, returns the byte array
       * containing data and a bool which is true if there is more data
       * available.
       * @param max the maximum amount of data to retrieve
       */
      QPair<QByteArray, bool> GetData(int max);

      /**
       * Called when a Ready has been responded to by the leader.  Calls Start
       * on the current round.
       * @param response The response from the server (empty)
       */
      void Ready(RpcRequest &response);

      /**
       * Used by the leader to queue Ready requests.
       */
      QHash<Id, RpcRequest> _id_to_request;

      /**
       * Used by a client to store messages to be sent for future rounds
       */
      QByteArray _send_queue;

      const Group _group;
      const Credentials _creds;
      const Id _leader_id;
      const Id _session_id;
      QSharedPointer<Network> _network;
      CreateRound _create_round;
      QSharedPointer<GroupGenerator> _generate_group;

      bool _round_ready;
      QSharedPointer<Round> _current_round;
      RpcMethod _ready;
      GetDataCallback _get_data_cb;
      int _round_idx;

    private slots:
      /**
       * Called when the current round has finished
       */
      virtual void HandleRoundFinished();

      /**
       * Called when a remote peer has disconnected from the session
       */
      virtual void HandleDisconnect();
  };
}
}

#endif
