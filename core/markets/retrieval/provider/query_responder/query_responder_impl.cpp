/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "markets/retrieval/provider/query_responder/query_responder_impl.hpp"
#include "markets/retrieval/protocols/query_protocol.hpp"
#include "codec/cbor/cbor.hpp"

namespace fc::markets::retrieval::provider {
  outcome::result<void> QueryResponderImpl::run() {
    OUTCOME_TRY(request_bytes, stream_->read());
    QueryResponse response{};
    response.message = "It works! 4745";
    OUTCOME_TRY(response_bytes, codec::cbor::encode(response));
    OUTCOME_TRY(stream_->write(response_bytes));
    return outcome::success();
  }
}  // namespace fc::markets::retrieval::provider
