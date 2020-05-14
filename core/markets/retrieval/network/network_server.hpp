/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_HPP

#include <functional>
#include <memory>

#include <libp2p/peer/protocol.hpp>
#include "common/buffer.hpp"
#include "common/outcome.hpp"
#include "markets/retrieval/network/network_stream.hpp"
#include "markets/retrieval/types.hpp"

namespace fc::markets::retrieval::network {
  class NetworkServer {
   protected:
    using NetworkStreamShPtr = std::shared_ptr<NetworkStream>;
    using ProtocolHandler = void (NetworkServer::*)(NetworkStreamShPtr);

   public:
    /**
     * @brief Destructor
     */
    virtual ~NetworkServer() = default;

    virtual void registerHandler(Protocol proto, ProtocolHandler handler) = 0;
  };

  /**
   * @enum Possible server-side errors
   */
  enum class NetworkServerError {};

}  // namespace fc::markets::retrieval::network

OUTCOME_HPP_DECLARE_ERROR(fc::markets::retrieval::network, NetworkServerError)

#endif
