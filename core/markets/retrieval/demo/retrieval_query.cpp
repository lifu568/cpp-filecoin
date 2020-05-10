/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <libp2p/common/literals.hpp>
#include <libp2p/injector/host_injector.hpp>
#include <libp2p/security/plaintext.hpp>

using libp2p::crypto::Key;
using libp2p::crypto::KeyPair;
using libp2p::crypto::PrivateKey;
using libp2p::crypto::PublicKey;
using libp2p::common::operator""_unhex;

int main() {
  KeyPair keypair{PublicKey{{Key::Type::Ed25519,
                             "48453469c62f4885373099421a7365520b5ffb"
                             "0d93726c124166be4b81d852e6"_unhex}},
                  PrivateKey{{Key::Type::Ed25519,
                              "4a9361c525840f7086b893d584ebbe475b4ec"
                              "7069951d2e897e8bceb0a3f35ce"_unhex}}};
  auto injector = libp2p::injector::makeHostInjector(
      libp2p::injector::useKeyPair(keypair),
      libp2p::injector::useSecurityAdaptors<libp2p::security::Plaintext>());
  auto provider_host = injector.create<std::shared_ptr<libp2p::Host>>();
  auto context = injector.create<std::shared_ptr<boost::asio::io_context>>();
}
