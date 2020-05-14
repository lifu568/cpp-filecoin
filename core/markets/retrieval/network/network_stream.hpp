/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_NETWORK_STREAM_HPP
#define CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_NETWORK_STREAM_HPP

#include <gsl/span>
#include "common/buffer.hpp"
#include "common/outcome.hpp"
#include "markets/retrieval/types.hpp"

namespace fc::markets::retrieval::network {
  /**
   * @class Synchronous interface for streams
   */
  class NetworkStream {
   public:
    /**
     * @brief Destructor
     */
    virtual ~NetworkStream() = default;

    /**
     * @brief Read data from the stream
     * @return operation result
     */
    virtual outcome::result<gsl::span<const uint8_t>> read() = 0;

    /**
     * @brief Write data to the stream
     * @param content - data to write
     * @return operation result
     */
    virtual outcome::result<void> write(gsl::span<const uint8_t> content) = 0;

    /**
     * @brief Close stream
     * @return operation result
     */
    virtual outcome::result<void> close() = 0;

    /**
     * @brief Get stream status
     * @return true, if stream closed for read and write
     */
    virtual bool isClosed() const = 0;
  };

  /**
   * @enum Network stream errors
   */
  enum class NetworkStreamError { READ_ERROR, WRITE_ERROR, CLOSE_ERROR };
}  // namespace fc::markets::retrieval::network

OUTCOME_HPP_DECLARE_ERROR(fc::markets::retrieval::network, NetworkStreamError)

#endif
