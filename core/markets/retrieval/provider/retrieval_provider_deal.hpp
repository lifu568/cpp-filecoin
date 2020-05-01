/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_PROVIDER_DEAL_HPP
#define CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_PROVIDER_DEAL_HPP

#include <memory>
#include <vector>

#include "fsm/fsm.hpp"
#include "markets/retrieval/provider/retrieval_provider_types.hpp"

namespace fc::markets::retrieval {
  /**
   * @class Provider-side event handlers
   */
  class ProviderDeal {
   protected:
    using Transition =
        fc::fsm::Transition<ProviderEvent, ProviderState, ProviderDeal>;

   public:
    /**
     * @brief Validate client proposal, check existence of the requested Piece
     * and unseal sector if needed
     * @param deal  - current retrieval deal
     * @param event - EvHandleProposal
     * @param from  - DealNew
     * @param to    - DealProposalReceived
     * @note possible occurring events:
     *              - EvAcceptDeal
     *              - EvRejectDeal
     */
    static void proposalHandler(std::shared_ptr<ProviderDeal> deal,
                                ProviderEvent event,
                                ProviderState from,
                                ProviderState to);

    /**
     * @brief Send appropriate response to the client
     * @param deal  - current retrieval deal
     * @param event - EventDealAccepted
     * @param from  - DealProposalReceived
     * @param to    - DealAccepted
     * @note possible occurring events:
     *              - EvNetworkError
     */
    static void acceptDealHandler(std::shared_ptr<ProviderDeal> deal,
                                  ProviderEvent event,
                                  ProviderState from,
                                  ProviderState to);

    /**
     * @brief Send appropriate response to the client
     * @param deal  - current retrieval deal
     * @param event - EvRejectDeal
     * @param from  - DealProposalReceived
     * @param to    - DealRejected
     * @note possible occurring events:
     *              - EvNetworkError
     */
    static void rejectDealHandler(std::shared_ptr<ProviderDeal> deal,
                                  ProviderEvent event,
                                  ProviderState from,
                                  ProviderState to);

    /**
     * @brief Handle network error
     * @param deal  - current retrieval deal
     * @param event - EvNetworkError
     * @param from  - Any
     * @param to    - Any
     */
    static void networkErrorHandler(std::shared_ptr<ProviderDeal> deal,
                                    ProviderEvent event,
                                    ProviderState from,
                                    ProviderState to);

    /**
     * @brief Handle internal error
     * @param deal  - current retrieval deal
     * @param event - EvInternalError
     * @param from  - Any
     * @param to    - DealFailed
     */
    static void internalErrorHandler(std::shared_ptr<ProviderDeal> deal,
                                     ProviderEvent event,
                                     ProviderState from,
                                     ProviderState to)

        /**
         * @brief Failed to read next block of the Piece
         * @param deal  - current retrieval deal
         * @param event - EvReadBlockError
         * @param from  - DealAccepted
         * @param to    - DealFailed
         * @note possible occuring events:
         *              - EvNetworkError
         */
        static void readBlockErrorHandler(std::shared_ptr<ProviderDeal> deal,
                                          ProviderEvent event,
                                          ProviderState from,
                                          ProviderState to);

    /**
     * @brief Request next payment from the client
     * @param deal  - current retrieval deal
     * @param event - EvRequestPayment
     * @param from  - DealAccepted
     * @param to    - DealPaymentNeeded
     * @note possible occuring events:
     *              - EvNetworkError
     */
    static void requestClientPaymentHandler(std::shared_ptr<ProviderDeal> deal,
                                            ProviderEvent event,
                                            ProviderState from,
                                            ProviderState to);

    /**
     * @brief Handle client payment
     * @param deal  - current retrieval deal
     * @param event - EvHandleClientPayment
     * @param from  - DealPaymentNeeded
     * @param to    - DealPaymentNeeded
     * @note possible occuring events:
     *              - EvContinueDeal
     *              - EvReadPaymentError
     *              - EvSaveVoucherError
     */
    static void clientPaymentHandler(std::shared_ptr<ProviderDeal> deal,
                                     ProviderEvent event,
                                     ProviderState from,
                                     ProviderState to);

    /**
     * @brief Continue deal after receiving next payment
     * @param deal  - current retrieval deal
     * @param event - EvContinueDeal
     * @param from  - DealPaymentNeeded
     * @param to    - DealAccepted
     * @note possible occuring events:
     *              - EvNetworkError
     */
    static void continueDealHandler(std::shared_ptr<ProviderDeal> deal,
                                    ProviderEvent event,
                                    ProviderState from,
                                    ProviderState to);

    /**
     * @brief Handle read client payment error
     * @param deal  - current retrieval deal
     * @param event - EvReadPaymentError
     * @param from  - DealPaymentNeeded
     * @param to    - DealFailed
     */
    static void readPaymentErrorHandler(std::shared_ptr<ProviderDeal> deal,
                                        ProviderEvent event,
                                        ProviderState from,
                                        ProviderState to);

    /**
     * @brief Handle save voucher error
     * @param deal  - current retrieval deal
     * @param event - EvSaveVoucherError
     * @param from  - DealPaymentNeeded
     * @param to    - DealFailed
     */
    static void saveVoucherFailHandler(std::shared_ptr<ProviderDeal> deal,
                                       ProviderEvent event,
                                       ProviderState from,
                                       ProviderState to);

    /**
     * @brief Handle error reading next block of the requested Piece
     * @param deal  - current retrieval deal
     * @param event - EvReadBlockError
     * @param from  - DealAccepted
     * @param to    - DealFailed
     */
    static void blocksCompletedHandler(std::shared_ptr<ProviderDeal> deal,
                                       ProviderEvent event,
                                       ProviderState from,
                                       ProviderState to);

    /**
     * @brief All blocks of a requested Piece was sent
     * @param deal  - current retrieval deal
     * @param event - EvBlocksCompleted
     * @param from  - DealAccepted
     * @param to    - DealFinalizing
     * @note possible occuring events:
     *              - ProviderEventCompleted
     */
    static void blocksCompletedHandler(std::shared_ptr<ProviderDeal> deal,
                                       ProviderEvent event,
                                       ProviderState from,
                                       ProviderState to);

    /**
     * @brief Finalize deal
     * @param deal  - current retrieval deal
     * @param event - EvCloseDeal
     * @param from  - DealFinalizing
     * @param to    - DealClosed
     */
    static void closeDealHandler(std::shared_ptr<ProviderDeal> deal,
                                 ProviderEvent event,
                                 ProviderState from,
                                 ProviderState to);

   private:
    std::vector<Transtition> transitions_{
        /* EvHandleProposal : DealNew -> DealProposalReceived */
        Transition{ProviderEvent::EvHandleProposal}
            .from(ProviderState::DealNew)
            .to(ProviderState::DealProposalReceived)
            .action(ProviderDeal::proposalHandler),
        /* EvRejectDeal : DealProposalReceived -> DealRejected */
        Transition{ProviderEvent::EvRejectDeal}
            .from(ProviderState::DealProposalReceived)
            .to(ProviderState::DealRejected)
            .action(ProviderDeal::rejectDealHandler),
        /* EvInternalError : Any -> DealFailed */
        Transition{ProviderEvent::EvInternalError}
            .fromAny()
            .to(ProviderState::DealFailed)
            .action(ProviderDeal::internalErrorHandler),
        /* EvNetworkError : DealRejected -> DealRejected,
         *                  DealAccepted -> DealFailed
         *                  DealPaymentNeeded -> DealFailed*/
        Transition{ProviderEvent::EvNetworkError}
            .from(ProviderDeal::DealRejected)
            .toSameState()
            .from(ProviderState::DealAccepted)
            .to(ProviderState::DealFailed)
            .action(ProviderDeal::networkErrorHandler),
        /* EvAcceptDeal : DealProposalReceived -> DealAccepted */
        Transition{ProviderEvent::EvAcceptDeal}
            .from(ProviderState::DealProposalReceived)
            .to(ProviderState::DealAccepted)
            .action(ProviderDeal::acceptDealHandler),
        /* EvReadBlockError : DealAccepted -> DealFailed */
        Transition{ProviderEvent::EvReadBlockError}
            .from(ProviderState::DealAccepted)
            .to(ProviderState::DealFailed)
            .action(ProviderDeal::readBlockErrorHandler),
        /* EvRequestPayment : DealAccepted -> DealPaymentNeeded */
        Transition{ProviderEvent::EvRequestPayment}
            .from(ProviderState::DealAccepted)
            .to(ProviderState::DealPaymentNeeded)
            .action(ProviderDeal::requestClientPaymentHandler),
        /* EvHandleClientPayment : DealPaymentNeeded -> DealPaymentNeeded */
        Transition{ProviderEvent::EvHandleClientPayment}
            .from(ProviderState::DealPaymentNeeded)
            .to(ProviderState::DealPaymentNeeded)
            .action(ProviderDeal::clientPaymentHandler),
        /* EvContinueDeal : DealPaymentNeeded -> DealAccepted */
        Transition{ProviderEvent::EvContinueDeal}
            .from(ProviderState::DealPaymentNeeded)
            .to(ProviderState::DealAccepted)
            .action(ProviderDeal::continueDealHandler)
        /* EvReadPaymentError : DealPaymentNeeded -> DealFailed */
        Transition{ProviderEvent::EvReadPaymentError}
            .from(ProviderState::DealPaymentNeeded)
            .to(ProviderState::DealFailed)
            .action(ProviderDeal::readPaymentErrorHandler),
        /* EvSaveVoucherError : DealPaymentNeeded -> DealFailed */
        Transition{ProviderEvent::EvSaveVoucherError}
            .from(ProviderState::DealPaymentNeeded)
            .to(ProviderState::DealFailed)
            .action(ProviderDeal::saveVoucherErrorHandler),
        /* EvBlocksCompleted : DealAccepted -> DealFinalizing */
        Transition{ProviderEvent::EvSaveVoucherError}
            .from(ProviderState::DealAccepted)
            .to(ProviderState::DealFinalizing)
            .action(ProviderDeal::blocksCompletedHandler),
        /* EvCloseDeal : DealFinalizing -> DealClosed */
        Transition{ProviderEvent::EvCloseDeal}
            .from(ProviderState::DealFinalizing)
            .to(ProviderState::DealClosed)
            .action(ProviderDeal::closeDealHandler)};
  };

}  // namespace fc::markets::retrieval

#endif
