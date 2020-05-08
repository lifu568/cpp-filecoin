/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <thread>

#include "markets/retrieval/network/impl/network_server_impl.hpp"

namespace fc::markets::retrieval::network {
  outcome::result<void> NetworkServerImpl::send(ConnectionId connection,
                                                Buffer data) {
    return AsyncOperation::run(
        [this, connection, buffer{std::move(data)}](Operation operation) {
          this->writeStream(operation, connection, buffer);
        });
  }

  void NetworkServerImpl::onNewStream(std::shared_ptr<Stream> stream) {
    ConnectionId id = counter_++;
    streams_[id] = std::move(stream);
    handler_->onConnect(id);
    readStream(id);
  }

  void NetworkServerImpl::readStream(ConnectionId id) {
    streams_.at(id)->readSome(
        buffer_, buffer_.size(), [this, id](outcome::result<size_t> length) {
          if (length.has_value()) {
            handler_->onRequest(id, buffer_.subbuffer(0, length.value()));
          } else {
            handler_->onDisconnect(id);
            closeStream(id);
          }
        });
  }

  void NetworkServerImpl::writeStream(Operation operation,
                                      ConnectionId id,
                                      Buffer data) {
    streams_.at(id)->writeSome(
        data,
        data.size(),
        [this, id, data_size{data.size()}, operation](
            outcome::result<size_t> sent) {
          if (sent.has_value() && data_size == sent.value()) {
            operation->set_value();
          } else {
            this->closeStream(id);
            AsyncOperation::failure(operation,
                                    NetworkServerError::SEND_RESPONSE_ERROR);
          }
        });
  }

  void NetworkServerImpl::closeStream(RemoteClientId client_id) {
    streams_.at(client_id)->close(
        [this, client_id](outcome::result<void> result) {
          streams_.erase(client_id);
          handler_->onConnectionClose(client_id);
        });
  }
}  // namespace fc::markets::retrieval::network
