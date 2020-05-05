/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_IMPL_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_NETWORK_SERVER_IMPL_HPP

#include <atomic>
#include <map>

#include <libp2p/connection/stream.hpp>
#include <libp2p/host/basic_host/basic_host.hpp>
#include "common/buffer.hpp"
#include "markets/retrieval/network/network_server.hpp"
#include "markets/retrieval/network/network_server_logic.hpp"

namespace fc::markets::retrieval::network {
  class NetworkServerImpl : public NetworkServer {
   protected:
    using HostService = libp2p::Host;
    using PeerInfo = libp2p::peer::PeerInfo;
    using Stream = libp2p::connection::Stream;

   public:
    NetworkServerImpl(std::shared_ptr<HostService> host_service,
                      Protocol proto,
                      std::shared_ptr<NetworkServerLogic> logic_handler,
                      size_t buffer_size = 1024)
        : service_{std::move(host_service)},
          handler_{std::move(logic_handler)},
          last_client_id_{},
          buffer_(buffer_size, 0) {
      service_->setProtocolHandler(proto,
                                   [this](std::shared_ptr<Stream> stream) {
                                     this->onNewStream(std::move(stream));
                                   });
    }

   private:
    std::shared_ptr<HostService> service_;
    std::shared_ptr<NetworkServerLogic> handler_;
    std::atomic<RemoteClientId> last_client_id_;
    std::map<RemoteClientId, std::shared_ptr<Stream>> streams_;
    common::Buffer buffer_;

    void onNewStream(std::shared_ptr<Stream> stream);

    void readStream(RemoteClientId client_id);

    void writeStream(RemoteClientId client_id, ClientResponse response);

    void closeStream(RemoteClientId client_id);
  };
}  // namespace fc::markets::retrieval::network

#endif
