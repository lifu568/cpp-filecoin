/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_PROVIDER_QUERY_RESPONDER_IMPL_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_PROVIDER_QUERY_RESPONDER_IMPL_HPP

#include <memory>

#include "markets/retrieval/network/network_stream.hpp"

namespace fc::markets::retrieval::provider {
  class QueryResponderImpl {
   protected:
    using NetworkStreamShPtr = std::shared_ptr<network::NetworkStream>;

   public:
    QueryResponderImpl(NetworkStreamShPtr network_stream)
        : stream_{std::move(network_stream)} {}

    outcome::result<void> run();

   private:
    NetworkStreamShPtr stream_;
  };
}  // namespace fc::markets::retrieval::provider

#endif
