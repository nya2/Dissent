#include "DissentTest.hpp"

namespace Dissent {
namespace Tests {
  QList<QSharedPointer<Node> > GenerateOverlay(int count, const QString &session_type)
  {
    Address base = BufferAddress(1);
    QList<Address> local;
    local.append(base);
    QList<Address> remote;
    remote.append(base);

    QList<QSharedPointer<Node> > nodes;

    Library *lib = CryptoFactory::GetInstance().GetLibrary();

    for(int idx = 0; idx < count; idx++) {
      Id id;
      QByteArray bid(id.GetByteArray());
      QSharedPointer<AsymmetricKey> key(lib->GeneratePrivateKey(bid));
      QSharedPointer<DiffieHellman> dh(lib->GenerateDiffieHellman(bid));

      nodes.append(QSharedPointer<Node>(new Node(Credentials(id, key, dh),
              local, remote, count, session_type)));
      nodes[idx]->sink = QSharedPointer<ISink>(new MockSinkWithSignal());
      local[0] = AddressFactory::GetInstance().CreateAny(local[0].GetType());
    }

    SignalCounter sc;

    foreach(QSharedPointer<Node> node, nodes) {
      QObject::connect(node.data(), SIGNAL(Ready()), &sc, SLOT(Counter()));
      node->bg.Start();
    }


    qint64 next = Timer::GetInstance().VirtualRun();
    while(next != -1 && sc.GetCount() != count) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    foreach(QSharedPointer<Node> node, nodes) {
      EXPECT_EQ(count, node->bg.GetConnectionTable().GetConnections().count());
    }

    return nodes;
  }

  void TerminateOverlay(const QList<QSharedPointer<Node> > &nodes)
  {
    SignalCounter sc;
    foreach(QSharedPointer<Node> node, nodes) {
      QObject::connect(&node->bg, SIGNAL(Disconnected()), &sc, SLOT(Counter()));
      node->bg.Stop();
    }

    qint64 next = Timer::GetInstance().VirtualRun();
    while(next != -1 && sc.GetCount() != nodes.count()) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_EQ(sc.GetCount(), nodes.count());

    foreach(QSharedPointer<Node> node, nodes) {
      EXPECT_EQ(node->bg.GetConnectionTable().GetConnections().count(), 0);
    }
  }

  void SendTest(const QList<QSharedPointer<Node> > &nodes)
  {
    Library *lib = CryptoFactory::GetInstance().GetLibrary();
    QScopedPointer<Dissent::Utils::Random> rand(lib->GetRandomNumberGenerator());

    QByteArray msg(512, 0);
    rand->GenerateBlock(msg);
    nodes[0]->session->Send(msg);

    SignalCounter sc;
    foreach(QSharedPointer<Node> node, nodes) {
      MockSinkWithSignal *sink = dynamic_cast<MockSinkWithSignal *>(node->sink.data());
      if(sink == 0) {
        qFatal("MockSinkWithSignal expected");
      }
      QObject::connect(sink, SIGNAL(ReadReady(MockSinkWithSignal *)), &sc, SLOT(Counter()));
    }

    int count = nodes.count();
    qint64 next = Timer::GetInstance().VirtualRun();
    while(next != -1 && sc.GetCount() != count) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    foreach(QSharedPointer<Node> node, nodes) {
      MockSinkWithSignal *sink = dynamic_cast<MockSinkWithSignal *>(node->sink.data());
      if(sink == 0) {
        qFatal("MockSinkWithSignal expected");
      }
      EXPECT_EQ(msg, sink->GetLastData());
    }
  }


  TEST(BasicGossip, Bootstrap)
  {
    int count = Random::GetInstance().GetInt(TEST_RANGE_MIN, TEST_RANGE_MAX);
    Timer::GetInstance().UseVirtualTime();
    QList<QSharedPointer<Node> > nodes = GenerateOverlay(count, "null");
    TerminateOverlay(nodes);
  }

  TEST(BasicGossip, Null)
  {
    int count = Random::GetInstance().GetInt(TEST_RANGE_MIN, TEST_RANGE_MAX);
    Timer::GetInstance().UseVirtualTime();
    QList<QSharedPointer<Node> > nodes = GenerateOverlay(count, "null");
    SendTest(nodes);
    TerminateOverlay(nodes);
  }

  TEST(BasicGossip, Shuffle)
  {
    int count = Random::GetInstance().GetInt(TEST_RANGE_MIN, TEST_RANGE_MAX);
    Timer::GetInstance().UseVirtualTime();
    QList<QSharedPointer<Node> > nodes = GenerateOverlay(count, "shuffle");
    SendTest(nodes);

    foreach(QSharedPointer<Node> node, nodes) {
      EXPECT_EQ(node->bg.OutstandingConnectionAttempts(), 0);
    }

    TerminateOverlay(nodes);
  }

  TEST(BasicGossip, UnorderedBootstrap)
  {
    int count = Random::GetInstance().GetInt(TEST_RANGE_MIN, TEST_RANGE_MAX);
    QString session_type = "null";
    Timer::GetInstance().UseVirtualTime();

    Address base = BufferAddress(1);
    QList<Address> local;
    local.append(base);
    QList<Address> remote;
    remote.append(base);

    QList<QSharedPointer<Node> > nodes;

    Library *lib = CryptoFactory::GetInstance().GetLibrary();

    for(int idx = 0; idx < count - 1; idx++) {
      local[0] = AddressFactory::GetInstance().CreateAny(local[0].GetType());

      Id id;
      QByteArray bid(id.GetByteArray());
      QSharedPointer<AsymmetricKey> key(lib->GeneratePrivateKey(bid));
      QSharedPointer<DiffieHellman> dh(lib->GenerateDiffieHellman(bid));

      nodes.append(QSharedPointer<Node>(new Node(Credentials(id, key, dh),
              local, remote, count, session_type)));
      nodes[idx]->sink = QSharedPointer<ISink>(new MockSinkWithSignal());
    }

    SignalCounter sc;

    foreach(QSharedPointer<Node> node, nodes) {
      QObject::connect(node.data(), SIGNAL(Ready()), &sc, SLOT(Counter()));
      node->bg.Start();
      EXPECT_TRUE(node->bg.NeedConnection());
    }

    qint64 next = Timer::GetInstance().VirtualRun();
    qint64 cnext = next + 10000;
    while(0 <= cnext) {
      Time::GetInstance().IncrementVirtualClock(next);
      cnext -= next;
      next = Timer::GetInstance().VirtualRun();
    }

    foreach(QSharedPointer<Node> node, nodes) {
      EXPECT_TRUE(node->bg.NeedConnection());
    }

    local[0] = base;
    Id id;
    QByteArray bid(id.GetByteArray());
    QSharedPointer<AsymmetricKey> key(lib->GeneratePrivateKey(bid));
    QSharedPointer<DiffieHellman> dh(lib->GenerateDiffieHellman(bid));

    nodes.append(QSharedPointer<Node>(new Node(Credentials(id, key, dh),
            local, remote, count, session_type)));
    nodes.last()->sink = QSharedPointer<ISink>(new MockSinkWithSignal());
    QObject::connect(nodes.last().data(), SIGNAL(Ready()), &sc, SLOT(Counter()));
    nodes.last()->bg.Start();

    while(next != -1 && sc.GetCount() != count) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    foreach(QSharedPointer<Node> node, nodes) {
      EXPECT_EQ(count, node->bg.GetConnectionTable().GetConnections().count());
    }

    TerminateOverlay(nodes);
  }

  TEST(BasicGossip, IdGeneration)
  {
    Id local_id;
    Id id(local_id.ToString());
    QList<Address> empty;
    BasicGossip bg(id, empty, empty);
    EXPECT_EQ(local_id, bg.GetId());

    QSharedPointer<AsymmetricKey> key;
    QSharedPointer<DiffieHellman> dh;
    Node n(Credentials(id, key, dh), empty, empty, 1, "Null");
    EXPECT_EQ(local_id, n.bg.GetId());
  }
}
}
