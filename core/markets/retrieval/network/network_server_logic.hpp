/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_LOGIC_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_LOGIC_HPP

#include "common/buffer.hpp"

namespace fc::markets::retrieval::network {
  using RemoteClientId = size_t;

  enum class ConnectionStatus { KEEP, CLOSE };

  struct ClientResponse {
    ConnectionStatus status;
    common::Buffer data;
  };

  class NetworkServerLogic {
   public:
    virtual ~NetworkServerLogic() = default;

    virtual void onNewConnection(RemoteClientId id) = 0;

    virtual ClientResponse onRequest(RemoteClientId id,
                                     common::Buffer data) = 0;

    virtual void onReceiveError(RemoteClientId id) = 0;

    virtual void onConnectionClose(RemoteClientId id) = 0;

    virtual void onSendResponseError(RemoteClientId id) = 0;
  };
}  // namespace fc::markets::retrieval::network

#endif
