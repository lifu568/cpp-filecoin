/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_CLIENT_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_CLIENT_HPP

#include <gsl/span>
#include <libp2p/peer/peer_info.hpp>
#include "common/outcome.hpp"

namespace fc::markets::retrieval::network {
  class NetworkClient {
   protected:
    using PeerInfo = libp2p::peer::PeerInfo;
    using Protocol = libp2p::peer::Protocol;

   public:
    virtual ~NetworkClient() = default;

    virtual outcome::result<void> connect(const PeerInfo &peer,
                                          const Protocol &proto) = 0;

    virtual outcome::result<void> send(gsl::span<const uint8_t> request) = 0;

    virtual outcome::result<gsl::span<const uint8_t>> receive() = 0;
  };

  enum class NetworkClientError {
    ConnectionError,
    SendRequestError,
    ReceiveResponseError,
    NetworkError
  };
}  // namespace fc::markets::retrieval::network

OUTCOME_HPP_DECLARE_ERROR(fc::markets::retrieval::network, NetworkClientError);

#endif
