/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <thread>

#include "markets/retrieval/provider/query_responder/query_responder_impl.hpp"
#include "markets/retrieval/provider/retrieval_provider_impl.hpp"

namespace fc::markets::retrieval::provider {
  void RetrievalProviderImpl::start() {
    this->registerHandler(kQueryProtocolId,
                          static_cast<ProtocolHandler>(
                              &RetrievalProviderImpl::queryRequestHandler));
    logger_->info("has been launched with ID "
                  + host_->getPeerInfo().id.toBase58());
  }

  void RetrievalProviderImpl::setPricePerByte(TokenAmount amount) {
    config_.price_per_byte = amount;
  }

  void RetrievalProviderImpl::setPaymentInterval(
      uint64_t payment_interval, uint64_t payment_interval_increase) {
    config_.payment_interval = payment_interval;
    config_.interval_increase = payment_interval_increase;
  }

  void RetrievalProviderImpl::queryRequestHandler(NetworkStreamShPtr stream) {
    std::thread([this, client_stream{std::move(stream)}]() {
      auto responder = std::make_shared<QueryResponderImpl>(
          client_stream, piece_storage_, api_, config_);
      std::ignore = responder->run();
    }).detach();
  }

}  // namespace fc::markets::retrieval::provider
