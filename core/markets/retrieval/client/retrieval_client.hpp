/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_CLIENT_HPP
#define CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_CLIENT_HPP

#include <functional>

#include "common/outcome.hpp"
#include "markets/retrieval/protocols/query_protocol.hpp"
#include "markets/retrieval/protocols/retrieval_protocol.hpp"
#include "primitives/address/address.hpp"
#include "primitives/cid/cid.hpp"

namespace fc::markets::retrieval::client {
  /*
   * @class Retrieval market client
   */
  class RetrievalClient {
   public:
    /**
     * @brief Destructor
     */
    virtual ~RetrievalClient() = default;

    /**
     * @brief Find providers, which has requested Piece
     * @param piece_cid - identifier of the requested Piece
     * @return Potential providers, sorted by priority to query
     */
    virtual outcome::result<std::vector<RetrievalPeer>> findProviders(
        const CID &piece_cid) const = 0;

    /**
     * @brief Query selected provider
     * @param peer - retrieval peer to query
     * @param request - query params for the provider
     * @return Query response
     */
    virtual outcome::result<QueryResponse> query(
        const RetrievalPeer &peer, const QueryRequest &request) const = 0;

    /**
     * @brief Retrieve Piece from selected provider
     * @param piece_cid - identifier of the Piece to retrieve
     * @param proposal_params - deal properties
     * @param total_funds - total allocated funds
     * @param retrieval_peer - provider to make a deal
     * @param client_wallet - client address
     * @param miner_wallet - miner payment address
     * @return Identifier of the deal or error
     */
    virtual outcome::result<DealID> retrieve(
        const CID &piece_cid,
        const DealProposalParams &proposal_params,
        TokenAmount total_funds,
        const RetrievalPeer &retrieval_peer,
        const Address &client_wallet,
        const Address &miner_wallet) const = 0;

    /**
     * @brief Subscribe on deal events
     * @param handler - events receiver
     */
    virtual void subscribe(std::function<void>(DealID,
                                               ClientEvent,
                                               ClientDealState) handler) = 0;
  };
}  // namespace fc::markets::retrieval::client

#endif
