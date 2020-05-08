/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_MARKETS_RETRIEVAL_PROVIDER_QUERY_RESPONDER_IMPL_HPP
#define CPP_FILECOIN_MARKETS_RETRIEVAL_PROVIDER_QUERY_RESPONDER_IMPL_HPP

#include "markets/retrieval/network/server_logic.hpp"

namespace fc::markets::retrieval::provider {
  class QueryResponderImpl : public network::NetworkServerLogic {
   public:
    QueryResponderImpl();

    void onNewConnection(RemoteClientId id) override;

    ClientResponse onRequest(RemoteClientId id, common::Buffer data) override;

    void onReceiveError(RemoteClientId id) override;

    void onConnectionClose(RemoteClientId id) override;

    void onSendResponseError(RemoteClientId id) override;
  };
}  // namespace fc::markets::retrieval::provider

#endif
