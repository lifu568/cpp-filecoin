/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <thread>

#include "markets/retrieval/network/impl/network_server_impl.hpp"

namespace fc::markets::retrieval::network {
  void NetworkServerImpl::registerHandler(Protocol proto,
                                          ProtocolHandler handler) {
    host_->setProtocolHandler(proto, [this, handler](StreamShPtr stream) {
      auto network_stream = std::make_shared<NetworkStreamImpl>(stream);
      std::invoke(handler, this, network_stream);
    });
  }
}  // namespace fc::markets::retrieval::network
