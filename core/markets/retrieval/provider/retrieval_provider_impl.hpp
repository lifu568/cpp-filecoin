/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_PROVIDER_IMPL_HPP
#define CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_PROVIDER_IMPL_HPP

#include "api/api.hpp"
#include "common/logger.hpp"
#include "markets/retrieval/network/impl/network_server_impl.hpp"
#include "markets/retrieval/protocols/query_protocol.hpp"
#include "markets/retrieval/provider/retrieval_provider_types.hpp"
#include "markets/retrieval/retrieval_provider.hpp"
#include "storage/piece/piece_storage.hpp"

namespace fc::markets::retrieval::provider {
  class RetrievalProviderImpl : public RetrievalProvider,
                                network::NetworkServerImpl {
   protected:
    using PieceStorageShPtr =
        std::shared_ptr<::fc::storage::piece::PieceStorage>;
    using ApiShPtr = std::shared_ptr<api::Api>;

   public:
    RetrievalProviderImpl(std::shared_ptr<HostService> host_service,
                          PieceStorageShPtr piece_storage,
                          ApiShPtr api)
        : NetworkServerImpl{std::move(host_service)},
          piece_storage_{std::move(piece_storage)},
          logger_{common::createLogger("Retrieval provider")},
          config_{},
          api_{std::move(api)} {}

    void start() override;

    void setPricePerByte(TokenAmount amount) override;

    void setPaymentInterval(uint64_t payment_interval,
                            uint64_t payment_interval_increase) override;

   private:
    PieceStorageShPtr piece_storage_;
    common::Logger logger_;
    ProviderConfig config_;
    ApiShPtr api_;

    void queryRequestHandler(NetworkStreamShPtr stream);
  };
}  // namespace fc::markets::retrieval::provider

#endif
