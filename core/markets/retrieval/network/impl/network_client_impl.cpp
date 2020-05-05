/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "markets/retrieval/network/impl/network_client_impl.hpp"

namespace fc::markets::retrieval::network {
  NetworkClientImpl::~NetworkClientImpl() {
    if (!stream_->isClosed()) {
      stream_->close([](outcome::result<void>) {});
    }
  }

  outcome::result<void> NetworkClientImpl::connect(const PeerInfo &peer) {
    operation_ = std::promise<void>();
    this->openStream(peer);
    auto future_connect = operation_.get_future();
    try {
      future_connect.get();
      return outcome::success();
    } catch (std::system_error &error) {
      return static_cast<NetworkClientError>(error.code().value());
    }
  }

  outcome::result<void> NetworkClientImpl::send(
      gsl::span<const uint8_t> request) {
    operation_ = std::promise<void>();
    this->sendRequest(request);
    auto future_send = operation_.get_future();
    try {
      future_send.get();
      return outcome::success();
    } catch (std::system_error &error) {
      return static_cast<NetworkClientError>(error.code().value());
    }
  }

  outcome::result<gsl::span<const uint8_t>> NetworkClientImpl::receive() {
    operation_ = std::promise<void>();
    this->receiveResponse();
    auto future_receive = operation_.get_future();
    try {
      future_receive.get();
      return gsl::make_span<const uint8_t>(buffer_.data(), buffer_size_);
    } catch (std::system_error &error) {
      return static_cast<NetworkClientError>(error.code().value());
    }
  }

  void NetworkClientImpl::openStream(const PeerInfo &peer) {
    host_service_->newStream(
        peer,
        proto_,
        [this](outcome::result<std::shared_ptr<Stream>> stream_result) {
          if (stream_result.has_value()) {
            stream_ = std::move(stream_result.value());
            operation_.set_value();
          } else {
            failure(NetworkClientError::ConnectionError);
          }
        });
  }

  void NetworkClientImpl::sendRequest(gsl::span<const uint8_t> request) {
    if (stream_ != nullptr) {
      stream_->writeSome(
          request,
          request.size(),
          [this, request](outcome::result<size_t> sentResult) {
            auto payload_size = static_cast<size_t>(request.size_bytes());
            if (sentResult.has_value() && payload_size == sentResult.value())
              this->operation_.set_value();
            else
              failure(NetworkClientError::SendRequestError);
          });
    } else {
      failure(NetworkClientError::NetworkError);
    }
  }

  void NetworkClientImpl::receiveResponse() {
    stream_->readSome(
        buffer_,
        buffer_.size(),
        [this](fc::outcome::result<size_t> dataLength) {
          if (dataLength.has_value()) {
            buffer_size_ = dataLength.value();
            operation_.set_value();
          } else {
            this->failure(NetworkClientError::ReceiveResponseError);
          }
        });
  }

  void NetworkClientImpl::failure(NetworkClientError error) {
    std::error_code ec{static_cast<int>(error), std::generic_category()};
    std::system_error error_exception{ec};
    response_promise_.set_exception(std::make_exception_ptr(error_exception));
  }
}  // namespace fc::markets::retrieval::network

OUTCOME_CPP_DEFINE_CATEGORY(fc::markets::retrieval::network,
                            NetworkClientError,
                            e) {
  using Error = fc::markets::retrieval::network::NetworkClientError;
  switch (e) {
    case Error::ConnectionError:
      return "Failed to connect to a remote peer";
    case Error::ReceiveResponseError:
      return "Failed to receive response from a remote peer";
    case Error::SendRequestError:
      return "Failed to send request to a remote peer";
    case Error::NetworkError:
      return "Network error";
  }
}
