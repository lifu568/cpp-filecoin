/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "markets/retrieval/provider/retrieval_provider_impl.hpp"
#include "markets/retrieval/provider/query_responder/query_responder_impl.hpp"

namespace fc::markets::retrieval::provider {
  void RetrievalProviderImpl::start() {
    this->registerHandler(kQueryProtocolId,
                          static_cast<ProtocolHandler>(
                              &RetrievalProviderImpl::queryRequestHandler));
  }

  outcome::result<void> RetrievalProviderImpl::stop() {
    return outcome::success();
  }

  void RetrievalProviderImpl::setPricePerByte(TokenAmount amount) {}

  void RetrievalProviderImpl::setPaymentInterval(
      uint64_t payment_interval, uint64_t payment_interval_increase) {}

  void RetrievalProviderImpl::queryRequestHandler(NetworkStreamShPtr stream) {
    auto responder = std::make_shared<QueryResponderImpl>(std::move(stream));
    std::ignore = responder->run();
  }

}  // namespace fc::markets::retrieval::provider
