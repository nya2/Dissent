#ifndef DISSENT_ANONYMITY_SESSION_MANAGER_H_GUARD
#define DISSENT_ANONYMITY_SESSION_MANAGER_H_GUARD

#include <QHash>
#include <QSharedPointer>

#include "../Connections/Id.hpp"
#include "../Messaging/RpcMethod.hpp"

namespace Dissent {
namespace Messaging {
  class RpcHandler;
  class RpcRequest;
}

namespace Anonymity {
  class Session;

  /**
   * Used to filter incoming messages across many sessions.
   */
  class SessionManager : public QObject {
    Q_OBJECT

    public:
      typedef Dissent::Messaging::RpcHandler RpcHandler;
      typedef Dissent::Messaging::RpcMethod<SessionManager> RpcMethod;
      typedef Dissent::Messaging::RpcRequest RpcRequest;
      typedef Dissent::Connections::Id Id;

      /**
       * Constructor
       * @param rpc
       */
      SessionManager(RpcHandler &rpc);

      /**
       * Deconstructor
       */
      virtual ~SessionManager();

      /**
       * Adds a Session for the SessionManager to handle. Does not start the session.
       * @param session The session to be handled
       */
      void AddSession(QSharedPointer<Session> session);

    private:
      QSharedPointer<Session> GetSession(RpcRequest &msg);
      void Ready(RpcRequest &request);
      void IncomingData(RpcRequest &notification);
      QHash<Id, QSharedPointer<Session> > _id_to_session;
      RpcMethod _ready;
      RpcMethod _data;
      RpcHandler &_rpc;
      // XXX this should be replaced with each node registering (continuously
      // poking until getting a response), then the leader begins the round
      // by sending a notify...
      QHash<Id, QList<RpcRequest> > _requests;

    private slots:
      void HandleSessionStop();
  };
}
}

#endif
