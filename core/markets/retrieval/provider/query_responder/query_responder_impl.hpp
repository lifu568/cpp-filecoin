/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_PROVIDER_QUERY_RESPONDER_IMPL_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_PROVIDER_QUERY_RESPONDER_IMPL_HPP

#include <memory>

#include "api/api.hpp"
#include "markets/retrieval/network/network_stream.hpp"
#include "markets/retrieval/protocols/query_protocol.hpp"
#include "markets/retrieval/provider/retrieval_provider_types.hpp"
#include "storage/piece/piece_storage.hpp"

namespace fc::markets::retrieval::provider {
  class QueryResponderImpl {
   protected:
    using NetworkStreamShPtr = std::shared_ptr<network::NetworkStream>;
    using PieceStorageShPtr =
        std::shared_ptr<::fc::storage::piece::PieceStorage>;
    using ApiShPtr = std::shared_ptr<api::Api>;

   public:
    QueryResponderImpl(NetworkStreamShPtr network_stream,
                       PieceStorageShPtr piece_storage,
                       ApiShPtr api,
                       const ProviderConfig &config)
        : stream_{std::move(network_stream)},
          piece_storage_{std::move(piece_storage)},
          api_{std::move(api)},
          provider_config_{config} {}

    outcome::result<void> run();

   private:
    NetworkStreamShPtr stream_;
    PieceStorageShPtr piece_storage_;
    ApiShPtr api_;
    const ProviderConfig &provider_config_;

    QueryItemStatus getItemStatus(const CID &payload_cid,
                                  const CID &piece_cid) const;
  };
}  // namespace fc::markets::retrieval::provider

#endif
