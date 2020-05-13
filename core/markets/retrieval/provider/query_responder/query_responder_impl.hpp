/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_PROVIDER_QUERY_RESPONDER_IMPL_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_PROVIDER_QUERY_RESPONDER_IMPL_HPP

#include <memory>

#include "markets/retrieval/network/network_stream.hpp"
#include "markets/retrieval/protocols/query_protocol.hpp"
#include "storage/piece/piece_storage.hpp"

namespace fc::markets::retrieval::provider {
  class QueryResponderImpl {
   protected:
    using NetworkStreamShPtr = std::shared_ptr<network::NetworkStream>;
    using PieceStorageShPtr = std::shared_ptr<storage::piece::PieceStorage>;

   public:
    QueryResponderImpl(NetworkStreamShPtr network_stream,
                       PieceStorageShPtr piece_storage)
        : stream_{std::move(network_stream)},
          piece_storage_{std::move(piece_storage)} {}

    outcome::result<void> run();

   private:
    NetworkStreamShPtr stream_;
    PieceStorageShPtr piece_storage_;

    QueryItemStatus getItemStatus(const CID &payload_cid,
                                  const CID &piece_cid) const;
  };
}  // namespace fc::markets::retrieval::provider

#endif
