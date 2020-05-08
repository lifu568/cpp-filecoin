/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_CLIENT_IMPL_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_CLIENT_IMPL_HPP

#include <future>
#include <memory>
#include <vector>

#include <libp2p/connection/stream.hpp>
#include <libp2p/host/basic_host/basic_host.hpp>
#include "markets/retrieval/network/async_operation.hpp"
#include "markets/retrieval/network/network_client.hpp"

namespace fc::markets::retrieval::network {

  class NetworkClientImpl : public NetworkClient {
   public:
    using HostService = libp2p::Host;
    using PeerInfo = libp2p::peer::PeerInfo;
    using Stream = libp2p::connection::Stream;
    using AsyncOperation = AsyncOperation<NetworkClientError>;
    using Operation = AsyncOperation::Operation;

    NetworkClientImpl(std::shared_ptr<HostService> service,
                      const size_t buffer_size = 1024)
        : host_service_{std::move(service)},
          response_promise_{},
          stream_{},
          buffer_(buffer_size) {}

    ~NetworkClientImpl();

    outcome::result<void> connect(const PeerInfo &peer,
                                  const Protocol &proto) override;

    outcome::result<void> send(gsl::span<const uint8_t> request) override;

    outcome::result<gsl::span<const uint8_t>> receive() override;

   private:
    std::shared_ptr<HostService> host_service_;
    std::promise<void> operation_;
    std::promise<gsl::span<const uint8_t>> response_promise_;
    std::shared_ptr<Stream> stream_;
    std::vector<uint8_t> buffer_;
    size_t buffer_size_{};

    void openStream(Operation operation,
                    const PeerInfo &peer,
                    const Protocol &proto);

    void sendRequest(Operation operation, gsl::span<const uint8_t> request);

    void receiveResponse(Operation operation);

    void failure(NetworkClientError error);
  };

}  // namespace fc::markets::retrieval::network

#endif
