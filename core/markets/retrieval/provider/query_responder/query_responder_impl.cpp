/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "markets/retrieval/provider/query_responder/query_responder_impl.hpp"
#include "codec/cbor/cbor.hpp"

namespace fc::markets::retrieval::provider {
  outcome::result<void> QueryResponderImpl::run() {
    OUTCOME_TRY(request_bytes, stream_->read());
    OUTCOME_TRY(request, codec::cbor::decode<QueryRequest>(request_bytes));
    QueryResponse response{
        .response_status = QueryResponseStatus::QueryResponseAvailable,
        .item_status =
            this->getItemStatus(request.payload_cid, request.params.piece_cid),
        .message = "DEMO"};
    OUTCOME_TRY(response_bytes, codec::cbor::encode(response));
    OUTCOME_TRY(stream_->write(response_bytes));
    return outcome::success();
  }

  QueryItemStatus QueryResponderImpl::getItemStatus(
      const CID &payload_cid, const CID &piece_cid) const {
    auto payload_info = piece_storage_->getPayloadLocation(payload_cid);
    if (payload_info.has_value()) {
      if (payload_cid != piece_cid
          && payload_info.value().parent_piece != piece_cid) {
        return QueryItemStatus::QueryItemUnavailable;
      }
      auto piece_info =
          piece_storage_->getPieceInfo(payload_info.value().parent_piece);
      if (piece_info.has_value()) {
        return QueryItemStatus::QueryItemAvailable;
      }
    }
    return QueryItemStatus::QueryItemUnknown;
  }
}  // namespace fc::markets::retrieval::provider
