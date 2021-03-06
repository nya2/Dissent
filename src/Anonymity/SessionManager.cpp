#include "../Messaging/RpcHandler.hpp"

#include "Session.hpp"
#include "SessionManager.hpp"

namespace Dissent {
namespace Anonymity {
  SessionManager::SessionManager(RpcHandler &rpc) :
    _ready(this, &SessionManager::Ready),
    _data(this, &SessionManager::IncomingData),
    _rpc(rpc)
  {
    _rpc.Register(&_data, "SM::Data");
    _rpc.Register(&_ready, "SM::Ready");
  }

  SessionManager::~SessionManager()
  {
    _rpc.Unregister("SM::Data");
    _rpc.Unregister("SM::Ready");
  }

  void SessionManager::AddSession(QSharedPointer<Session> session)
  {
    QObject::connect(session.data(), SIGNAL(Stopping()), this, SLOT(HandleSessionStop()));
    _id_to_session[session->GetId()] = session;
    if(!session->IsLeader()) {
      return;
    }

    const Id &id = session->GetId();
    if(!_requests.contains(id)) {
      return;
    }

    foreach(RpcRequest request, _requests[id]) {
      Ready(request);
    }

    _requests.remove(id);
  }

  void SessionManager::Ready(RpcRequest &request)
  {
    QSharedPointer<Session> session = GetSession(request);
    if(!session.isNull()) {
      session->ReceivedReady(request);
    } else {
      QByteArray bid = request.GetMessage()["session_id"].toByteArray();
      Id id(bid);
      if(!_requests.contains(id)) {
        _requests[id] = QList<RpcRequest>();
      }
      _requests[id].append(request);
    }
  }

  void SessionManager::IncomingData(RpcRequest &notification)
  {
    QSharedPointer<Session> session = GetSession(notification);
    if(!session.isNull()) {
      session->IncomingData(notification);
    }
  }

  QSharedPointer<Session> SessionManager::GetSession(RpcRequest &msg)
  {
    QByteArray bid = msg.GetMessage()["session_id"].toByteArray();
    if(bid.isEmpty()) {
      qWarning() << "Received a wayward session message from " << msg.GetFrom()->ToString();
      return QSharedPointer<Session>();
    }

    Id id(bid);
    if(_id_to_session.contains(id)) {
      return _id_to_session[id];
    } else {
      qWarning() << "Received a wayward session message for session " <<
        id.ToString() << " from " << msg.GetFrom()->ToString();
      return QSharedPointer<Session>();
    }
  }

  void SessionManager::HandleSessionStop()
  {
    Session *session = qobject_cast<Session *>(sender());
    if(!session) {
      qCritical() << "Expected session found null";
      return;
    }

    QObject::disconnect(session, SIGNAL(Stopping()), this, SLOT(HandleSessionStop()));
    _id_to_session.remove(session->GetId());
  }
}
}
