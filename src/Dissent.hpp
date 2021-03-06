#ifndef DISSENT_DISSENT_H_GUARD
#define DISSENT_DISSENT_H_GUARD

#include "Anonymity/BulkRound.hpp"
#include "Anonymity/Credentials.hpp"
#include "Anonymity/FixedSizeGroupGenerator.hpp"
#include "Anonymity/Group.hpp"
#include "Anonymity/GroupGenerator.hpp"
#include "Anonymity/Log.hpp"
#include "Anonymity/NullRound.hpp"
#include "Anonymity/RepeatingBulkRound.hpp"
#include "Anonymity/Round.hpp"
#include "Anonymity/Session.hpp"
#include "Anonymity/SessionManager.hpp"
#include "Anonymity/ShuffleRound.hpp"
#include "Anonymity/TrustedBulkRound.hpp"

#include "Applications/CommandLine.hpp"
#include "Applications/ConsoleSink.hpp"
#include "Applications/FileSink.hpp"
#include "Applications/Node.hpp"
#include "Applications/SessionFactory.hpp"
#include "Applications/Settings.hpp"

#include "Crypto/AsymmetricKey.hpp"
#include "Crypto/CppDiffieHellman.hpp"
#include "Crypto/CppHash.hpp"
#include "Crypto/CppIntegerData.hpp"
#include "Crypto/CppLibrary.hpp"
#include "Crypto/CppPrivateKey.hpp"
#include "Crypto/CppPublicKey.hpp"
#include "Crypto/CppRandom.hpp"
#include "Crypto/CryptoFactory.hpp"
#include "Crypto/DiffieHellman.hpp"
#include "Crypto/CppHash.hpp"
#include "Crypto/Hash.hpp"
#include "Crypto/Integer.hpp"
#include "Crypto/IntegerData.hpp"
#include "Crypto/Library.hpp"
#include "Crypto/NullDiffieHellman.hpp"
#include "Crypto/NullHash.hpp"
#include "Crypto/NullLibrary.hpp"
#include "Crypto/NullPrivateKey.hpp"
#include "Crypto/NullPublicKey.hpp"
#include "Crypto/OnionEncryptor.hpp"
#include "Crypto/Serialization.hpp"
#include "Crypto/ThreadedOnionEncryptor.hpp"

#include "Connections/Connection.hpp"
#include "Connections/ConnectionManager.hpp"
#include "Connections/ConnectionTable.hpp"
#include "Connections/DefaultNetwork.hpp"
#include "Connections/EmptyNetwork.hpp"
#include "Connections/Id.hpp"
#include "Connections/Network.hpp"

#include "Messaging/BufferSink.hpp"
#include "Messaging/BufferSinkWithSignal.hpp"
#include "Messaging/DummySink.hpp"
#include "Messaging/GetDataCallback.hpp" 
#include "Messaging/ISender.hpp"
#include "Messaging/ISink.hpp"
#include "Messaging/RpcHandler.hpp"
#include "Messaging/RpcMethod.hpp"
#include "Messaging/RpcRequest.hpp"
#include "Messaging/RpcResponse.hpp"
#include "Messaging/Source.hpp"

#include "Overlay/BasicGossip.hpp"

#include "Transports/Address.hpp"
#include "Transports/AddressFactory.hpp"
#include "Transports/BufferAddress.hpp"
#include "Transports/BufferEdge.hpp"
#include "Transports/BufferEdgeListener.hpp"
#include "Transports/Edge.hpp"
#include "Transports/EdgeFactory.hpp"
#include "Transports/EdgeListener.hpp"
#include "Transports/EdgeListenerFactory.hpp"
#include "Transports/TcpAddress.hpp"
#include "Transports/TcpEdge.hpp"
#include "Transports/TcpEdgeListener.hpp"

#include "Utils/Logging.hpp"
#include "Utils/QRunTimeError.hpp"
#include "Utils/Random.hpp"
#include "Utils/Serialization.hpp"
#include "Utils/SignalCounter.hpp"
#include "Utils/Sleeper.hpp"
#include "Utils/StartStop.hpp"
#include "Utils/Time.hpp"
#include "Utils/Timer.hpp"
#include "Utils/TimerCallback.hpp"
#include "Utils/TimerEvent.hpp"
#include "Utils/Triggerable.hpp"
#include "Utils/Triple.hpp"

using namespace Dissent::Anonymity;
using namespace Dissent::Applications;
using namespace Dissent::Crypto;
using namespace Dissent::Connections;
using namespace Dissent::Messaging;
using namespace Dissent::Overlay;
using namespace Dissent::Transports;
using namespace Dissent::Utils;

#endif
