/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_PROVIDER_IMPL_HPP
#define CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_PROVIDER_IMPL_HPP

#include "markets/retrieval/network/impl/network_server_impl.hpp"
#include "markets/retrieval/protocols/query_protocol.hpp"
#include "markets/retrieval/retrieval_provider.hpp"

namespace fc::markets::retrieval::provider {
  class RetrievalProviderImpl : public RetrievalProvider,
                                network::NetworkServerImpl {
   public:
    RetrievalProviderImpl(std::shared_ptr<HostService> host_service)
        : NetworkServerImpl{std::move(host_service)} {}

    void start() override;

    outcome::result<void> stop() override;

    void setPricePerByte(TokenAmount amount) override;

    void setPaymentInterval(uint64_t payment_interval,
                            uint64_t payment_interval_increase) override;

   private:
    void queryRequestHandler(NetworkStreamShPtr stream);
  };
}  // namespace fc::markets::retrieval::provider

#endif
