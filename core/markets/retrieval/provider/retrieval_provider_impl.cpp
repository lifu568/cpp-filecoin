/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "markets/retrieval/provider/retrieval_provider_impl.hpp"

namespace fc::markets::retrieval::provider {
  void RetrievalProviderImpl::start() {
    this->registerHandler(kQueryProtocolId,
                          &RetrievalProviderImpl::queryRequestHandler);
  }

  outcome::result<void> RetrievalProviderImpl::stop() {}

  void RetrievalProviderImpl::setPricePerByte(TokenAmount amount) {}

  void RetrievalProviderImpl::setPaymentInterval(
      uint64_t payment_interval, uint64_t payment_interval_increase) {}

}  // namespace fc::markets::retrieval
