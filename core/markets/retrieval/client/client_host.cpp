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

const libp2p::peer::Protocol kProtocolId{"/fil/test/1"};

void requester(fc::outcome::result<std::shared_ptr<Stream>>) {
  std::cout << "Client: we are connected to provider" << std::endl;
}

int main() {
  auto common_context = std::make_shared<CommonContextImpl>();
  auto crypto_context = std::make_shared<CryptoContextImpl>();
  auto key_pair = crypto_context->getProvider()->generateKeys(KeyType::RSA);
  auto security_context =
      std::make_shared<SecurityContextImpl>(crypto_context, key_pair.value());
  auto network_context =
      std::make_shared<NetworkContextImpl>(security_context, common_context);
  auto listen_address = MultiAddress::create("/ip4/127.0.0.1/tcp/8080");
  auto provider_host = network_context->createHost();
  provider_host->setProtocolHandler(kProtocolId, responder);
  std::cout << "Provider host: " << provider_host->getId().toBase58()
            << std::endl;
  provider_host->listen(listen_address.value());
  auto client_host = network_context->createHost();
  client_host->newStream(provider_host->getPeerInfo(), kProtocolId, requester);
  common_context->runInputOutput(0);
}
