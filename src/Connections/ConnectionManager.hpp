#ifndef DISSENT_CONNECTIONS_CONNECTION_MANAGER_H_GUARD
#define DISSENT_CONNECTIONS_CONNECTION_MANAGER_H_GUARD

#include "../Messaging/RpcMethod.hpp"
#include "../Transports/EdgeFactory.hpp"

#include "ConnectionTable.hpp"

namespace Dissent {
namespace Messaging {
  class RpcHandler;
  class RpcRequest;
}

namespace Transports {
  class Address;
  class Edge;
  class EdgeListener;
}

namespace Connections {
  /**
   * Manages incoming and outgoing connections -- A node should only
   * send requests on outgoing connections.
   */
  class ConnectionManager : public QObject {
    Q_OBJECT

    public:
      typedef Dissent::Messaging::RpcMethod<ConnectionManager> Callback;
      typedef Dissent::Messaging::RpcHandler RpcHandler;
      typedef Dissent::Messaging::RpcRequest RpcRequest;
      typedef Dissent::Transports::Address Address;
      typedef Dissent::Transports::Edge Edge;
      typedef Dissent::Transports::EdgeListener EdgeListener;
      typedef Dissent::Transports::EdgeFactory EdgeFactory;

      /**
       * Constructor
       * @param local_id the local id
       * @param rpc the Rpc Handler used in connection signaling
       */
      ConnectionManager(const Id &local_id, RpcHandler &rpc);

      /**
       * Deconstructor
       */
      virtual ~ConnectionManager();

      /**
       * Add an EdgeListener for creating edges for incoming and outgoing
       * connections.  The ConnectionManager will own EL and delete it during
       * deconstruction.
       * @param el an EdgeListener for the node to manage
       */
      void AddEdgeListener(QSharedPointer<EdgeListener> el);

      /**
       * Connect to the specified transport address
       * @param addr the transport address to connect to
       */
      void ConnectTo(const Address &addr);

      /**
       * Disconnect from all Connections, in preparation for shutdown
       */
      void Disconnect();

      /**
       * Returns the outgoing connection table
       */
      inline ConnectionTable &GetConnectionTable() { return _con_tab; }

      /**
       * Returns the incoming connection table
       */
      inline ConnectionTable &GetRemoteConnectionTable() { return _rem_con_tab; }

      /**
       * Returns the local Id
       */
      inline const Id &GetId() { return _local_id; }
      
    signals:
      /**
       * A new outgoing connection has been created
       * @param con the new connection
       * @param local true if owned locally
       */
      void NewConnection(Connection *con, bool local);

      /**
       * A ConnectTo attempt failed, I guess ideally ConnectTo would take an Id
       * and handle the translation internally, but then again...
       * @param to the ConnectTo(to) that failed
       * @param reason the reason it failed
       */
      void ConnectionAttemptFailure(const Address &to, const QString &reason);

      /**
       * Emitted when disconnected
       */
      void Disconnected();

    private:
      /**
       * A remote peer is inquiring about the nodes Id
       */
      void Inquire(RpcRequest &request);

      /**
       * Handles an Inquire response
       */
      void Inquired(RpcRequest &response);

      /**
       * Based upon the Inquire response, the node wants to create a connection.
       */
      void Connect(RpcRequest &notification);

      /**
       * Based upon the Inquire response, the node wants to close the edge.
       */
      void Close(RpcRequest &notification);

      /**
       * Notify a remote node of intent to close a connection (edge)
       */
      void Disconnect(RpcRequest &notification);

      Callback _inquire;
      Callback _inquired;
      Callback _close;
      Callback _connect;
      Callback _disconnect;

      ConnectionTable _con_tab;
      ConnectionTable _rem_con_tab;
      const Id _local_id;
      EdgeFactory _edge_factory;
      RpcHandler &_rpc;
      bool _closed;

    private slots:
      /**
       * Called when Connection::Disconnect is executed
       */
      void HandleDisconnect();

      /**
       * An Edge has been disconnected and the resulting connection has been too
       */
      void HandleDisconnected(const QString &reason);

      /**
       * A new Edge has been created
       */
      void HandleNewEdge(Edge *edge);
      
      /**
       * An Edge has been closed
       */
      void HandleEdgeClose(const QString &reason);

      /**
       * An Edge was never created
       */
      void HandleEdgeCreationFailure(const Address &to, const QString &reason);
  };
}
}

#endif
