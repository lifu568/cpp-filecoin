/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "markets/retrieval/client/retrieval_client_impl.hpp"
#include "codec/cbor/cbor.hpp"
#include "markets/retrieval/network/impl/network_client_impl.hpp"
#include "markets/retrieval/protocols/query_protocol.hpp"

namespace fc::markets::retrieval::client {
  outcome::result<QueryResponse> RetrievalClientImpl::query(
      const PeerInfo &peer, const QueryRequest &request) const {
    network::NetworkClientImpl client{host_service_};
    OUTCOME_TRY(client.connect(peer, kQueryProtocolId));
    OUTCOME_TRY(encoded_request, codec::cbor::encode(request));
    OUTCOME_TRY(client.send(encoded_request));
    OUTCOME_TRY(encoded_response, client.receive());
    OUTCOME_TRY(response, codec::cbor::decode<QueryResponse>(encoded_response));
    return response;
  }

  outcome::result<std::vector<Block>> RetrievalClientImpl::retrieve(
      const CID &piece_cid,
      const PeerInfo &provider_peer,
      const DealProfile &deal_profile) {
    return std::vector<Block>{};
  }
}  // namespace fc::markets::retrieval::client
