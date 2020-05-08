/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_IMPL_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_IMPL_HPP

#include <atomic>
#include <future>
#include <map>

#include <libp2p/connection/stream.hpp>

#include "common/buffer.hpp"
#include "markets/retrieval/network/async_operation.hpp"
#include "markets/retrieval/network/network_server.hpp"
#include "markets/retrieval/network/server_logic.hpp"

namespace fc::markets::retrieval::network {
  class NetworkServerImpl : public NetworkServer {
   protected:
    using PeerInfo = libp2p::peer::PeerInfo;
    using Stream = libp2p::connection::Stream;
    using AsyncOperation = AsyncOperation<NetworkServerError>;
    using Operation = AsyncOperation::Operation;

   public:
    /**
     * @brief Constructor
     * @param host_service - libp2p host
     * @param proto - protocol id
     * @param logic_handler - client requests handler
     * @param buffer_size - receiving buffer size
     */
    NetworkServerImpl(std::shared_ptr<HostService> host_service,
                      const Protocol &proto,
                      std::shared_ptr<ServerLogic> logic_handler,
                      size_t buffer_size = 1024)
        : buffer_(buffer_size, 0),
          counter_{},
          service_{std::move(host_service)},
          handler_{std::move(logic_handler)}

    {
      service_->setProtocolHandler(proto,
                                   [this](std::shared_ptr<Stream> stream) {
                                     this->onNewStream(std::move(stream));
                                   });
    }

    outcome::result<void> send(ConnectionId connection, Buffer data) override;

    outcome::result<void> close(ConnectionId connection) override;

   private:
    common::Buffer buffer_;
    std::atomic<ConnectionId> counter_;
    std::promise<void> send_operation_;
    std::promise<void> close_operation_;
    std::shared_ptr<HostService> service_;
    std::shared_ptr<ServerLogic> handler_;
    std::map<ConnectionId, std::shared_ptr<Stream>> streams_;

    void onNewStream(std::shared_ptr<Stream> stream);

    void readStream(ConnectionId id);

    void writeStream(Operation operation, ConnectionId id, Buffer data);

    void closeStream(ConnectionId id);
  };
}  // namespace fc::markets::retrieval::network

#endif
