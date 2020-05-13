/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "markets/retrieval/network/impl/network_client_impl.hpp"
#include "markets/retrieval/network/impl/network_stream_impl.hpp"

namespace fc::markets::retrieval::network {
  outcome::result<std::shared_ptr<NetworkStream>> NetworkClientImpl::connect(
      const PeerInfo &peer, const Protocol &proto) {
    NetworkStreamShPtr network_stream{};
    OUTCOME_TRY(AsyncOperation::run(
        [this, &peer, &proto, &network_stream](Operation operation) {
          this->openStream(operation, peer, proto, network_stream);
        }));
    return network_stream;
  }

  void NetworkClientImpl::openStream(Operation operation,
                                     const PeerInfo &peer,
                                     const Protocol &proto,
                                     NetworkStreamShPtr &network_stream) {
    host_service_->newStream(
        peer,
        proto,
        [operation, &network_stream](outcome::result<Stream> stream) {
          if (stream.has_value()) {
            network_stream =
                std::make_shared<NetworkStreamImpl>(std::move(stream.value()));
            operation->set_value();
          } else {
            AsyncOperation::failure(operation,
                                    NetworkClientError::ConnectionError);
          }
        });
  }

}  // namespace fc::markets::retrieval::network

OUTCOME_CPP_DEFINE_CATEGORY(fc::markets::retrieval::network,
                            NetworkClientError,
                            e) {
  using Error = fc::markets::retrieval::network::NetworkClientError;
  switch (e) {
    case Error::ConnectionError:
      return "Failed to connect to a remote peer";
    case Error::NetworkError:
      return "Unknown network error";
  }
}
