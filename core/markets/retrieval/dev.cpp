#include <iostream>

#include "host/context/crypto/crypto_context_impl.hpp"

using fc::host::context::crypto::CryptoContextImpl;
using libp2p::crypto::Key;

void printKey(const std::vector<uint8_t> &key) {
  for (uint8_t byte : key) {
    printf("%02x", byte);
  }
  std::cout << std::endl;
}

int main() {
  auto crypto = std::make_shared<CryptoContextImpl>();
  auto keys = crypto->getProvider()->generateKeys(Key::Type::Ed25519).value();
  printKey(keys.privateKey.data);
  printKey(keys.publicKey.data);
}
