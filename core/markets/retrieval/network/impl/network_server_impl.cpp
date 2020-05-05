/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "markets/retrieval/network/impl/network_server_impl.hpp"

#include <iostream>

namespace fc::markets::retrieval::network {
  void NetworkServerImpl::onNewStream(std::shared_ptr<Stream> stream) {
    RemoteClientId client_id = last_client_id_++;
    streams_[client_id] = std::move(stream);
    handler_->onNewConnection(client_id);
    readStream(client_id);
  }

  void NetworkServerImpl::readStream(RemoteClientId client_id) {
    streams_.at(client_id)->readSome(
        buffer_,
        buffer_.size(),
        [this, client_id](outcome::result<size_t> length) {
          if (length.has_value()) {
            ClientResponse action = handler_->onRequest(
                client_id, buffer_.subbuffer(0, length.value()));
            writeStream(client_id, std::move(action));
          } else {
            handler_->onReceiveError(client_id);
            closeStream(client_id);
          }
        });
  }

  void NetworkServerImpl::writeStream(RemoteClientId client_id,
                                      ClientResponse response) {
    ConnectionStatus status = response.status;
    if (!response.data.empty()) {
      streams_.at(client_id)->writeSome(
          response.data.toVector(),
          response.data.size(),
          [this, client_id, status](outcome::result<size_t> sent) {
            if (sent.has_value()) {
              if (status == ConnectionStatus::CLOSE)
                closeStream(client_id);
              else
                readStream(client_id);
            } else {
              handler_->onSendResponseError(client_id);
              closeStream(client_id);
            }
          });
    } else {
      if (response.status == ConnectionStatus::CLOSE) closeStream(client_id);
    }
  }

  void NetworkServerImpl::closeStream(RemoteClientId client_id) {
    streams_.at(client_id)->close(
        [this, client_id](outcome::result<void> result) {
          streams_.erase(client_id);
          handler_->onConnectionClose(client_id);
        });
  }
}  // namespace fc::markets::retrieval::network
