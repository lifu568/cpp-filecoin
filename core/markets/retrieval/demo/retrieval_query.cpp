/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <thread>

#include <libp2p/common/literals.hpp>
#include <libp2p/injector/host_injector.hpp>
#include <libp2p/security/plaintext.hpp>
//#include "../test/testutil/literals.hpp"
#include "host/context/crypto/crypto_context_impl.hpp"
#include "markets/retrieval/client/retrieval_client_impl.hpp"
#include "markets/retrieval/provider/retrieval_provider_impl.hpp"

using fc::markets::retrieval::client::RetrievalClientImpl;
using fc::markets::retrieval::provider::RetrievalProviderImpl;
using libp2p::crypto::Key;
using libp2p::crypto::KeyPair;
using libp2p::crypto::PrivateKey;
using libp2p::crypto::PublicKey;
using libp2p::peer::PeerId;
using libp2p::peer::PeerInfo;
using libp2p::common::operator""_unhex;
using libp2p::common::operator""_peerid;
using ProviderShPtr =
    std::shared_ptr<fc::markets::retrieval::provider::RetrievalProvider>;
using ClientShPtr =
    std::shared_ptr<fc::markets::retrieval::client::RetrievalClient>;
using ContextShPtr = std::shared_ptr<boost::asio::io_context>;
using HostShPtr = std::shared_ptr<libp2p::Host>;
using fc::CID;
using fc::markets::retrieval::QueryRequest;

namespace demo {
  std::vector<uint8_t> payload_a{"bacefe"_unhex};
}

namespace config {
  namespace provider {
    KeyPair keypair{
        PublicKey{
            {Key::Type::Ed25519,
             "5373b3cd50a00df1d6274872990e3b7a0b3e7461dbbeb411c82fda525b38a202"_unhex}},
        PrivateKey{
            {Key::Type::Ed25519,
             "fb2b5f11125f49230736087ae6af01c9460b77ed34b0f055447437aa9fa44cde"_unhex}}};
    std::string multiaddress{"/ip4/127.0.0.1/tcp/8080"};
  }  // namespace provider
  namespace client {
    KeyPair keypair{
        PublicKey{
            {Key::Type::Ed25519,
             "3329beb9ead4582e1a524225cde47ce959e0892841e3a8de7d3d09dd1edafda0"_unhex}},
        PrivateKey{
            {Key::Type::Ed25519,
             "322ee47e407e085bb15b136b8ab208857ce90aa471cbb79bcf9ac6de376b346b"_unhex}}};
  }
}  // namespace config

struct Sandbox {
  ProviderShPtr provider;
  ClientShPtr client;
  ContextShPtr context;
  HostShPtr provider_host;
  HostShPtr client_host;

  Sandbox() {
    auto injector = libp2p::injector::makeHostInjector(
        libp2p::injector::useKeyPair(config::provider::keypair),
        libp2p::injector::useSecurityAdaptors<libp2p::security::Plaintext>());
    this->provider_host = injector.create<std::shared_ptr<libp2p::Host>>();
    this->client_host = injector.create<std::shared_ptr<libp2p::Host>>();
    this->context = injector.create<std::shared_ptr<boost::asio::io_context>>();
    auto listen_address =
        libp2p::multi::Multiaddress::create(config::provider::multiaddress)
            .value();
    std::ignore = this->provider_host->listen(listen_address);
    this->provider_host->start();
    provider = std::make_shared<RetrievalProviderImpl>(this->provider_host);
    client = std::make_shared<RetrievalClientImpl>(this->provider_host);
  }
};

int main() {
  Sandbox box;
  box.provider->start();
  std::thread{[&box]() {
    box.context->run();
  }}.detach();
  QueryRequest query_request{
      .payload_cid = {fc::common::getCidOf(demo::payload_a).value()}};
  auto response =
      box.client->query(box.provider_host->getPeerInfo(), query_request);
  if (response.has_value()) {
    std::cout << response.value().message << std::endl;
  } else {
    std::cout << response.error().message() << std::endl;
  }
  box.context->stop();
}
