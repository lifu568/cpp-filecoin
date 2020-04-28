/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_PROVIDER_TYPES_HPP
#define CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_PROVIDER_TYPES_HPP

#include <string>

#include "markets/retrieval/common_types.hpp"
#include "primitives/types.hpp"

namespace fc::markets::retrieval::provider {
  using primitives::TokenAmount;

  /**
   * @struct Provider deal state
   */
  struct ProviderDealState {
    /* Current deal status */
    DealStatus deal_status;

    /* Sender PeerID */
    uint64_t sender_peer_id;

    /* Total bytes sent */
    uint64_t total_sent;

    /* Received tokens amount */
    TokenAmount funds_received;

    /* Signal message */
    std::string message;

    /* Current payment interval, bytes */
    uint64_t current_interval;
  };

  /**
   * @enum Provider events
   */
  enum class ProviderEvent : uint64_t {
    /* Received new deal from a client */
    ProviderEventOpen,

    /* Send deal response network error */
    ProviderEventWriteResponseFailed,

    /* Network error while reading payment voucher from a client */
    ProviderEventReadPaymentFailed,

    /* Failed to look up requested piece size */
    ProviderEventGetPieceSizeErrored,

    /* Provider cannot find piece for proposed deal */
    ProviderEventDealNotFound,

    /* Provider rejected client's deal proposal */
    ProviderEventDealRejected,

    /* Provider accepted client's deal proposal */
    ProviderEventDealAccepted,

    /* Failed to read next block from a piece */
    ProviderEventBlockErrored,

    /* Provider reads last block from a piece */
    ProviderEventBlocksCompleted,

    /* Provider asks for payment from a client */
    ProviderEventPaymentRequested,

    /* Failed to save a payment voucher */
    ProviderEventSaveVoucherFailed,

    /* Received a payment, which is less than requested */
    ProviderEventPartialPaymentReceived,

    /* Provider received payment and resumes a deal */
    ProviderEventPaymentReceived,

    /* Deal was completed */
    ProviderEventCompleted
  };
}  // namespace fc::markets::retrieval::provider

#endif
