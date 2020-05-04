#include <iostream>
#include <string>

#include <libp2p/connection/stream.hpp>
#include <libp2p/multi/multiaddress.hpp>
#include "host/context/common/common_context_impl.hpp"
#include "host/context/crypto/crypto_context_impl.hpp"
#include "host/context/network/network_context_impl.hpp"
#include "host/context/security/security_context_impl.hpp"

using fc::host::context::common::CommonContextImpl;
using fc::host::context::crypto::CryptoContextImpl;
using fc::host::context::network::NetworkContextImpl;
using fc::host::context::security::SecurityContextImpl;
using KeyType = libp2p::crypto::Key::Type;
using MultiAddress = libp2p::multi::Multiaddress;
using Stream = libp2p::connection::Stream;
using NetworkHost = libp2p::Host;
using PeerInfo = libp2p::peer::PeerInfo;

const libp2p::peer::Protocol kProtocolId{"/fil/test/1"};

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
  Provider(std::shared_ptr<NetworkHost> host)
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
  std::shared_ptr<NetworkHost> network_host_;
  std::vector<uint8_t> buffer_;
  Logger logger_;
};

class Client {
 public:
  Client(std::shared_ptr<NetworkHost> host)
      : network_host_{std::move(host)}, logger_{"Client"} {}

  void request(const PeerInfo &peer_info, std::vector<uint8_t> data) {
    current_request_ = std::move(data);
    network_host_->newStream(
        peer_info,
        kProtocolId,
        [this](fc::outcome::result<std::shared_ptr<Stream>> stream) {
          if (stream.has_value()) {
            stream.value()->writeSome(
                this->current_request_,
                this->current_request_.size(),
                [this](fc::outcome::result<size_t> sentLength) {
                  this->sendRequestHandler(sentLength);
                });
          } else {
            this->logger_.log("Failed to sent request");
          }
        });
  }

  void sendRequestHandler(fc::outcome::result<size_t> sentLength) {
    if (sentLength.has_value()) {
      logger_.log("Sent " + std::to_string(sentLength.value()) + " bytes");
    } else {
      logger_.log("Failed to send request");
    }
  }

 private:
  std::shared_ptr<NetworkHost> network_host_;
  std::vector<uint8_t> current_request_;
  Logger logger_;
};

std::shared_ptr<NetworkHost> createHost(
    std::shared_ptr<CommonContextImpl> common_context) {
  auto crypto_context = std::make_shared<CryptoContextImpl>();
  auto key_pair = crypto_context->getProvider()->generateKeys(KeyType::RSA);
  auto security_context =
      std::make_shared<SecurityContextImpl>(crypto_context, key_pair.value());
  auto network_context =
      std::make_shared<NetworkContextImpl>(security_context, common_context);
  return network_context->createHost();
}

int main() {
  auto common_context = std::make_shared<CommonContextImpl>();
  auto provider_host = createHost(common_context);
  auto client_host = createHost(common_context);
  Provider provider(provider_host);
  Client client(client_host);
  provider.start();
  auto listen_address = MultiAddress::create("/ip4/127.0.0.1/tcp/8080");
  std::ignore = provider_host->listen(listen_address.value());
  provider_host->start();
  client.request(provider_host->getPeerInfo(), {4, 8, 15, 16, 23, 42});
  common_context->runInputOutput(0);
}
