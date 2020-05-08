#include <iostream>
#include <string>
#include <thread>

#include <libp2p/connection/stream.hpp>
#include <libp2p/multi/multiaddress.hpp>
#include "host/context/common/common_context_impl.hpp"
#include "host/context/crypto/crypto_context_impl.hpp"
#include "host/context/network/network_context_impl.hpp"
#include "host/context/security/security_context_impl.hpp"

#include "markets/retrieval/network/impl/network_client_impl.hpp"
#include "markets/retrieval/network/impl/network_server_impl.hpp"
#include "markets/retrieval/network/server_logic.hpp"

using fc::host::context::common::CommonContextImpl;
using fc::host::context::crypto::CryptoContextImpl;
using fc::host::context::network::NetworkContextImpl;
using fc::host::context::security::SecurityContextImpl;
using fc::markets::retrieval::network::ClientResponse;
using fc::markets::retrieval::network::ConnectionStatus;
using fc::markets::retrieval::network::NetworkClient;
using fc::markets::retrieval::network::NetworkServerImpl;
using fc::markets::retrieval::network::NetworkServerLogic;
using fc::markets::retrieval::network::RemoteClientId;
using KeyType = libp2p::crypto::Key::Type;
using MultiAddress = libp2p::multi::Multiaddress;
using Stream = libp2p::connection::Stream;
using HostService = libp2p::Host;
using PeerInfo = libp2p::peer::PeerInfo;
using Protocol = libp2p::peer::Protocol;

const Protocol kProtocolId{"/fil/test/1"};

std::vector<uint8_t> buffer{};

class Logger {
 public:
  Logger(const std::string &source) : src_{source} {}
  void log(const std::string &data) {
    std::cout << "[" << src_ << "]: " << data << std::endl;
  }

 private:
  std::string src_;
};

class Provider {
 public:
  Provider(std::shared_ptr<HostService> host)
      : network_host_{std::move(host)}, buffer_(128), logger_{"Provider"} {}

  void start() {
    network_host_->setProtocolHandler(kProtocolId,
                                      [this](std::shared_ptr<Stream> stream) {
                                        this->readRequest(std::move(stream));
                                      });
  }

  void readRequest(std::shared_ptr<Stream> stream) {
    logger_.log("New connection");
    stream->readSome(buffer_,
                     buffer_.size(),
                     [this](fc::outcome::result<size_t> dataLength) {
                       this->handleRequest(dataLength);
                     });
  }

  void handleRequest(fc::outcome::result<size_t> dataLength) {
    if (dataLength.has_value()) {
      logger_.log("Received " + std::to_string(dataLength.value()) + " bytes");
      for (size_t i = 0; i < dataLength.value(); ++i) {
        std::cout << std::to_string(buffer_.at(i)) << " ";
      }
      std::cout << std::endl;
    } else {
      logger_.log("Failed to read data from the client");
    }
  }

 private:
  std::shared_ptr<HostService> network_host_;
  std::vector<uint8_t> buffer_;
  Logger logger_;
};

class QueryResponder : public NetworkServerLogic {
 public:
  void onNewConnection(RemoteClientId id) override {
    logger_.log("New connection: #" + std::to_string(id));
  }

  ClientResponse onRequest(RemoteClientId id,
                           fc::common::Buffer data) override {
    logger_.log("New request from #" + std::to_string(id));
    for (size_t i = 0; i < data.size(); ++i) {
      std::cout << std::to_string(data.toVector().at(i)) << " ";
    }
    std::cout << std::endl;
    std::vector<uint8_t> content{4, 7, 4, 5};
    fc::common::Buffer buffer{content};
    ClientResponse response{.status = ConnectionStatus::CLOSE, .data = buffer};
    return response;
  }

  void onReceiveError(RemoteClientId id) override {
    logger_.log("Failed to receive from #" + std::to_string(id));
  }

  void onConnectionClose(RemoteClientId id) override {
    logger_.log("Close connection: #" + std::to_string(id));
  }

  void onSendResponseError(RemoteClientId id) override {
    logger_.log("Send response error: #" + std::to_string(id));
  }

 private:
  Logger logger_{"QueryResponder"};
};

std::shared_ptr<HostService> createHost(
    std::shared_ptr<CommonContextImpl> common_context) {
  auto crypto_context = std::make_shared<CryptoContextImpl>();
  auto key_pair = crypto_context->getProvider()->generateKeys(KeyType::RSA);
  auto security_context =
      std::make_shared<SecurityContextImpl>(crypto_context, key_pair.value());
  auto network_context =
      std::make_shared<NetworkContextImpl>(security_context, common_context);
  return network_context->createHost();
}

fc::outcome::result<void> testClient(std::shared_ptr<HostService> host,
                                     PeerInfo target) {
  fc::markets::retrieval::network::NetworkClientImpl client(host, kProtocolId);
  std::vector<uint8_t> request_data{4, 8, 15, 16, 23, 42};
  OUTCOME_TRY(client.connect(target));
  OUTCOME_TRY(client.send(request_data));
  OUTCOME_TRY(response, client.receive());
  for (size_t i = 0; i < 4; ++i) {
    std::cout << std::to_string(response.at(i)) << " ";
  }
  std::cout << std::endl;
  return fc::outcome::success();
}

int main() {
  auto common_context = std::make_shared<CommonContextImpl>();
  auto provider_host = createHost(common_context);
  auto client_host = createHost(common_context);
  auto responder = std::make_shared<QueryResponder>();
  NetworkServerImpl server{provider_host, kProtocolId, responder};
  auto listen_address = MultiAddress::create("/ip4/127.0.0.1/tcp/8080");
  std::ignore = provider_host->listen(listen_address.value());
  provider_host->start();
  std::thread([common_context]() {
    common_context->runInputOutput(5);
  }).detach();
  auto test_client_result =
      testClient(client_host, provider_host->getPeerInfo());
  if (test_client_result.has_value()) {
    std::cout << "Client test OK" << std::endl;
  } else {
    std::cout << test_client_result.error().message() << std::endl;
  }
  std::this_thread::sleep_for(std::chrono::seconds(3));
}
