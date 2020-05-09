/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_NETWORK_STREAM_IMPL_HPP
#define CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_NETWORK_STREAM_IMPL_HPP

#include <memory>

#include <libp2p/connection/stream.hpp>
#include "markets/retrieval/network/async_operation.hpp"
#include "markets/retrieval/network/network_stream.hpp"

namespace fc::markets::retrieval::network {
  class NetworkStreamImpl : public NetworkStream {
   protected:
    using Stream = std::shared_ptr<libp2p::connection::Stream>;
    using AsyncOperation = AsyncOperation<NetworkStreamError>;
    using Operation = AsyncOperation::Operation;

   public:
    /**
     * @brief Constructor
     * @param stream - connection stream
     * @param read_buffer - size of the read buffer
     */
    NetworkStreamImpl(Stream stream, size_t read_buffer = 1024)
        : stream_{std::move(stream)}, buffer_(read_buffer, 0), buffer_size_{} {}

    outcome::result<gsl::span<const uint8_t>> read() override;

    outcome::result<void> write(gsl::span<const uint8_t> content) override;

    outcome::result<void> close() override;

    bool isClosed() const override;

   private:
    Stream stream_;
    Buffer buffer_;
    size_t buffer_size_;

    void internalRead(Operation operation);

    void internalWrite(Operation operation, gsl::span<const uint8_t> content);
  };
}  // namespace fc::markets::retrieval::network

#endif
