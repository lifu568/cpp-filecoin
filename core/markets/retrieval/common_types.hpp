/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_COMMON_TYPES_HPP
#define CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_COMMON_TYPES_HPP

#include <libp2p/peer/peer_info.hpp>
#include "codec/cbor/streams_annotation.hpp"
#include "common/buffer.hpp"
#include "primitives/address/address.hpp"

namespace fc::markets::retrieval {
  using common::Buffer;
  using libp2p::peer::PeerId;
  using primitives::address::Address;

  /* Retrieval deal identifier */
  using DealID = uint64_t;

  /**
   * @struct Everything needed to make a deal with a miner
   */
  struct RetrievalPeer {
    Address address;
    PeerId id;
  };

  /**
   * @struct IPLD data block in the bitswap format
   */
  struct DataBlock {
    /* Block prefix */
    Buffer prefix;

    /* Block raw bytes */
    Buffer data;
  };

  CBOR_TUPLE(DataBlock, prefix, data);

  /**
   * @struct Payment info
   */
  struct PaymentInfo {
    Address payment_channel;
    uint64_t lane;
  };

  /**
   * @enum Deal statuses
   */
  enum class DealStatus : uint64_t {
    /* New deal, nothing happened with it */
    DealStatusNew = 1,

    /* Waiting for the payment channel creation to complete */
    DealStatusPaymentChannelCreating,

    /* Waiting for funds to finish being sent to the payment channel */
    DealStatusPaymentChannelAddingFunds,

    /* Waiting for the lane allocation to complete */
    DealStatusPaymentChannelAllocatingLane,

    /* Payment channel and lane are ready */
    DealStatusPaymentChannelReady,

    /* Ready to proceed with retrieval */
    DealStatusAccepted,

    /* Something went wrong during retrieval */
    DealStatusFailed,

    /* Provider rejected client's deal proposal */
    DealStatusRejected,

    /* Provider needs a payment voucher to continue */
    DealStatusFundsNeeded,

    /* Provider is continuing to process a deal */
    DealStatusOngoing,

    /* Provider need a last payment voucher to complete a deal */
    DealStatusFundsNeededLastPayment,

    /* Deal is completed */
    DealStatusCompleted,

    /* Deal couldn't be identified */
    DealStatusDealNotFound,

    /* Deal had been verified as having right params */
    DealStatusVerified,

    /* Something went wrong with deal */
    DealStatusErrored,

    /* All blocks has been processed for the piece */
    DealStatusBlocksComplete,

    /* Last payment has been received, confirming deal */
    DealStatusFinalizing

  };
}  // namespace fc::markets::retrieval

#endif