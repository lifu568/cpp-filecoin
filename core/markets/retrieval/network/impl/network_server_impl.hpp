/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_IMPL_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_IMPL_HPP

#include "common/buffer.hpp"
#include "markets/retrieval/network/async_operation.hpp"
#include "markets/retrieval/network/impl/network_stream_impl.hpp"
#include "markets/retrieval/network/network_server.hpp"

namespace fc::markets::retrieval::network {
  class NetworkServerImpl : public NetworkServer {
   protected:
    using HostServiceShPtr = std::shared_ptr<HostService>;
    using AsyncOperation = AsyncOperation<NetworkServerError>;
    using Operation = AsyncOperation::Operation;
    using NetworkStreamShPtr = std::shared_ptr<NetworkStream>;
    using StreamShPtr = std::shared_ptr<libp2p::connection::Stream>;

   public:
    /**
     * @brief Constructor
     * @param host_service - libp2p host
     */
    NetworkServerImpl(std::shared_ptr<HostService> host_service)
        : host_{std::move(host_service)} {}

    void registerHandler(Protocol proto, ProtocolHandler handler) override;

   private:
    HostServiceShPtr host_;
  };
}  // namespace fc::markets::retrieval::network

#endif
