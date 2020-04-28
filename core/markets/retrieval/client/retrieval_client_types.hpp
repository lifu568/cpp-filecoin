/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_CLIENT_TYPES_HPP
#define CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_CLIENT_TYPES_HPP

#include <string>

#include "markets/retrieval/common_types.hpp"
#include "primitives/address/address.hpp"
#include "primitives/cid/cid.hpp"
#include "primitives/types.hpp"

namespace fc::markets::retrieval::client {
  using primitives::TokenAmount;
  using primitives::address::Address;

  /**
   * @struct Client-side states of a deal
   */
  struct ClientDealState {
    /* Total funds, which can be spent */
    TokenAmount total_tokens;

    /* Client address */
    Address client_wallet;

    /* Miner address */
    Address miner_wallet;

    /* Payment channel and lane id */
    PaymentInfo payment_info;

    /* Current deal status */
    DealStatus deal_status;

    /* Sender peer id */
    uint64_t sender_peer_id;

    /* Already received bytes count */
    uint64_t total_received;

    /* Optional message */
    std::string message;

    /* Already paid bytes */
    uint64_t bytes_paid_for;

    /* Current payment interval, bytes */
    uint64_t current_interval;

    /* Requested payment valued */
    TokenAmount payment_requested;

    /* Already spent value */
    TokenAmount tokens_spent;

    /* Signal message id */
    CID wait_message_cid;
  };

  /*
   * @enum Client-side deal lifecycle events
   */
  enum class ClientEvents : uint16_t {
    /* Deal was initiated */
    ClientEventOpen = 1,

    /* Creating payment channel error */
    ClientEventPaymentChannelErrored,

    /* Creating lane in the payment channel error */
    ClientEventAllocateLaneErrored,

    /* Waiting for a message to create a payment channel */
    ClientEventPaymentChannelCreateInitiated,

    /* Newly created payment channel is ready to for the deal to resume */
    ClientEventPaymentChannelReady,

    /* Waiting for funds to be added to payment channel */
    ClientEventPaymentChannelAddingFunds,

    /* Failed to add funds to payment channel */
    ClientEventPaymentChannelAddFundsErrored,

    /* Send deal proposal network error */
    ClientEventWriteDealProposalErrored,

    /* Receive deal proposal response network error */
    ClientEventReadDealResponseErrored,

    /* Provider rejected deal */
    ClientEventDealRejected,

    /* Provider couldn't find a piece for a deal */
    ClientEventDealNotFound,

    /* Provider accepted a deal */
    ClientEventDealAccepted,

    /* Received unknown deal proposal response from provider */
    ClientEventUnknownResponseReceived,

    /* Need to add more funds to a payment channel to continue a deal */
    ClientEventFundsExpended,

    /* Provider asked for funds in a way that doesn't match the deal terms */
    ClientEventBadPaymentRequested,

    /* Failed to create payment voucher */
    ClientEventCreateVoucherFailed,

    /* Send payment voucher network error */
    ClientEventWriteDealPaymentErrored,

    /* Payment voucher was sent to a provider */
    ClientEventPaymentSent,

    /* Network error while reading block from provider */
    ClientEventConsumeBlockFailed,

    /* Provider requested last payment */
    ClientEventLastPaymentRequested,

    /* Provider has sent all blocks */
    ClientEventAllBlocksReceived,

    /* Provider completed a deal without sending all blocks */
    ClientEventEarlyTermination,

    /* Provider requested a next payment */
    ClientEventPaymentRequested,

    /* Received next data from a provider */
    ClientEventProgress,

    /* Error occurred during a deal */
    ClientEventError,

    /* Deal has been completed */
    ClientEventComplete

  };
}  // namespace fc::markets::retrieval::client

#endif
