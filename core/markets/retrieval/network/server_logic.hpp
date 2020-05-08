/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_LOGIC_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_LOGIC_HPP

#include "common/buffer.hpp"

namespace fc::markets::retrieval::network {
  /**
   * @class Request handler interface
   */
  class ServerLogic {
   public:
    /**
     * @brief Destructor
     */
    virtual ~ServerLogic() = default;

    /**
     * @brief Calls by network server on new client connection
     * @param id - connection identifier
     */
    virtual void onConnect(ConnectionId id) = 0;

    /**
     * @brief Calls by network server on new data received from a client
     * @param id - connection identifier
     * @param data - received data
     */
    virtual void onRequest(ConnectionId id, common::Buffer data) = 0;

    /**
     * @brief Calls by network server on client disconnect
     * @param id - connection identifier
     */
    virtual void onDisconnect(ConnectionId id) = 0;
  };
}  // namespace fc::markets::retrieval::network

#endif
