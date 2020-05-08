/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_HPP

#include <libp2p/peer/protocol.hpp>
#include "common/buffer.hpp"
#include "common/outcome.hpp"
#include "markets/retrieval/types.hpp"

namespace fc::markets::retrieval::network {
  class NetworkServer {
   public:
    /**
     * @brief Destructor
     */
    virtual ~NetworkServer() = default;

    /**
     * @brief Send response to a client
     * @param connection - client connection id
     * @param data - response data
     * @return operation result
     */
    virtual outcome::result<void> send(ConnectionId connection,
                                       Buffer data) = 0;

    /**
     * @brief Close client connection
     * @param connection - client id
     * @return operation result
     */
    virtual outcome::result<void> close(ConnectionId connection) = 0;
  };

  /**
   * @enum Possible server-side errors
   */
  enum class NetworkServerError {
    UNKNOWN_CONNECTION_ID,
    SEND_RESPONSE_ERROR,
    CLOSE_CONNECTION_ERROR
  };

}  // namespace fc::markets::retrieval::network

OUTCOME_HPP_DECLARE_ERROR(fc::markets::retrieval::network, NetworkServerError)

#endif
