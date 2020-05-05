/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_HPP

#include <libp2p/peer/protocol.hpp>

#include "markets/retrieval/network/network_server_logic.hpp"

namespace fc::markets::retrieval::network {
  class NetworkServer {
   protected:
    using Protocol = libp2p::peer::Protocol;
  };
}  // namespace fc::markets::retrieval::network

#endif
