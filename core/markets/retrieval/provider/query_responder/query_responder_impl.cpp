/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "markets/retrieval/provider/query_responder/query_responder_impl.hpp"

#include <iostream>

namespace fc::markets::retrieval::provider {
  outcome::result<void> QueryResponderImpl::run() {
    OUTCOME_TRY(request_bytes, stream_->read());
    std::cout << "QueryResponder received request" << std::endl;
    return outcome::success();
  }
}  // namespace fc::markets::retrieval::provider
