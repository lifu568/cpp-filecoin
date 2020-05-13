/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "markets/retrieval/client/retrieval_client_impl.hpp"
#include "codec/cbor/cbor.hpp"
#include "markets/retrieval/network/impl/network_client_impl.hpp"
#include "markets/retrieval/protocols/query_protocol.hpp"

namespace fc::markets::retrieval::client {
  using PeerInfo = network::NetworkClient::PeerInfo;

  outcome::result<std::vector<PeerInfo>> RetrievalClientImpl::findProviders(
      const CID &piece_cid) const {
    return outcome::failure(RetrievalClientError::NOT_IMPLEMENTED);
  }

  outcome::result<QueryResponse> RetrievalClientImpl::query(
      const PeerInfo &peer, const QueryRequest &request) {
    OUTCOME_TRY(encoded_request, codec::cbor::encode(request));
    OUTCOME_TRY(stream, connect(peer, kQueryProtocolId));
    OUTCOME_TRY(stream->write(encoded_request));
    OUTCOME_TRY(encoded_response, stream->read());
    OUTCOME_TRY(response, codec::cbor::decode<QueryResponse>(encoded_response));
    OUTCOME_TRY(stream->close());
    return std::move(response);
  }

  outcome::result<std::vector<Block>> RetrievalClientImpl::retrieve(
      const CID &piece_cid,
      const PeerInfo &provider_peer,
      const DealProfile &deal_profile) {
    return std::vector<Block>{};
  }
}  // namespace fc::markets::retrieval::client

OUTCOME_CPP_DEFINE_CATEGORY(fc::markets::retrieval::client,
                            RetrievalClientError,
                            e) {
  return "NOT IMPLEMENTED";
}
