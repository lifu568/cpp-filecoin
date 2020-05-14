/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_CLIENT_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_CLIENT_HPP

#include <memory>

#include <libp2p/peer/peer_info.hpp>
#include "common/outcome.hpp"
#include "markets/retrieval/network/network_stream.hpp"

namespace fc::markets::retrieval::network {
  class NetworkClient {
   public:
    using PeerInfo = libp2p::peer::PeerInfo;
    using Protocol = libp2p::peer::Protocol;
    using NetworkStreamShPtr = std::shared_ptr<NetworkStream>;

    virtual ~NetworkClient() = default;

    virtual outcome::result<NetworkStreamShPtr> connect(
        const PeerInfo &peer, const Protocol &proto) = 0;
  };

  enum class NetworkClientError { ConnectionError, NetworkError };
}  // namespace fc::markets::retrieval::network

OUTCOME_HPP_DECLARE_ERROR(fc::markets::retrieval::network, NetworkClientError);

#endif
