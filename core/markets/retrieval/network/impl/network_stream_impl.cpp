/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "markets/retrieval/network/impl/network_stream_impl.hpp"

namespace fc::markets::retrieval::network {
  outcome::result<gsl::span<const uint8_t>> NetworkStreamImpl::read() {
    auto result = AsyncOperation::run(
        [this](Operation operation) { this->internalRead(operation); });
    if (result.has_error()) return result.error();
    return gsl::make_span<const uint8_t>(buffer_.data(), buffer_size_);
  }

  outcome::result<void> NetworkStreamImpl::write(
      gsl::span<const uint8_t> content) {
    return AsyncOperation::run([this, content](Operation operation) {
      this->internalWrite(operation, content);
    });
  }

  outcome::result<void> NetworkStreamImpl::close() {
    return AsyncOperation::run([this](Operation operation) {
      stream_->close([operation](outcome::result<void> result) {
        if (!result.has_error())
          operation->set_value();
        else
          AsyncOperation::failure(operation, NetworkStreamError::CLOSE_ERROR);
      });
    });
  }

  bool NetworkStreamImpl::isClosed() const {
    return stream_->isClosedForRead() || stream_->isClosedForWrite();
  }

  void NetworkStreamImpl::internalRead(Operation operation) {
    stream_->readSome(buffer_,
                      buffer_.size(),
                      [this, operation](fc::outcome::result<size_t> length) {
                        if (length.has_value()) {
                          buffer_size_ = length.value();
                          operation->set_value();
                        } else {
                          AsyncOperation::failure(
                              operation, NetworkStreamError::READ_ERROR);
                        }
                      });
  }

  void NetworkStreamImpl::internalWrite(Operation operation,
                                        gsl::span<const uint8_t> data) {
    stream_->write(
        data, data.size(), [operation](outcome::result<size_t> length) {
          if (length.has_value())
            operation->set_value();
          else
            AsyncOperation::failure(operation, NetworkStreamError::WRITE_ERROR);
        });
  }

}  // namespace fc::markets::retrieval::network
