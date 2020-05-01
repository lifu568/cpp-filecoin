/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_CLIENT_DEAL_HPP
#define CPP_FILECOIN_CORE_MARKETS_RETRIEVAL_CLIENT_DEAL_HPP

#include <vector>

#include "fsm/fsm.hpp"
#include "markets/retrieval/provider/retrieval_client_types.hpp"

namespace fc::markets::retrieval::client {
  /*
   * @class Client-side event handlers
   */
  class ClientDeal {
   protected:
    using Transition =
        fc::fsm::Transition<ClientEvent, ClientState, ClientDeal>;

   public:
    /**
     * @brief Send retrieval deal proposal to a provider
     * @param deal  - current retrieval deal
     * @param event - EvSendProposal
     * @param from  - DealNew
     * @param to    - DealOpen
     * @note possible occurring events:
     *              - EvNetworkError
     */
    static void sendProposalHandler(std::shared_ptr<ClientDeal> deal,
                                    ClientEvent event,
                                    ClientState from,
                                    ClientState to);

    /**
     * @brief Handle deal, which was rejected by a provider
     * @param deal  - current retrieval deal
     * @param event - EvDealRejected
     * @param from  - DealOpen
     * @param to    - DealRejected
     */
    static void rejectDealHandler(std::shared_ptr<ClientDeal> deal,
                                  ClientEvent event,
                                  ClientState from,
                                  ClientState to);
    /**
     * @brief Handle accepted deal
     * @param deal  - current retrieval deal
     * @param event - EvDealAccepted
     * @param from  - DealOpen
     * @param to    - DealOngoing
     * @note possible occurring events:
     *              - EvNetworkError
     */
    static void acceptDealHandler(std::shared_ptr<ClientDeal> deal,
                                  ClientEvent event,
                                  ClientState from,
                                  ClientState to);

    /**
     * @brief Send retrieval deal proposal to a provider
     * @param deal  - current retrieval deal
     * @param event - EvSendProposal
     * @param from  - DealNew
     * @param to    - DealOpen
     * @note possible occurring events:
     *              - EvNetworkError
     */
    static void sendProposalHandler(std::shared_ptr<ClientDeal> deal,
                                    ClientEvent event,
                                    ClientState from,
                                    ClientState to);

    /**
     * @brief Receive next blocks from provider handler
     * @param deal  - current retrieval deal
     * @param event - EvReceiveProgress
     * @param from  - DealOngoing
     * @param to    - DealOngoing
     * @note possible occurring events:
     *              - EvSendPayment
     *              - EvDealCompleted
     *              - EvVerifyFail
     *              - EvWriteFail
     *              - EvConsumeFail
     *              - EvEarlyTermination
     */
    static void receiveBlocksHandler(std::shared_ptr<ClientDeal> deal,
                                     ClientEvent event,
                                     ClientState from,
                                     ClientState to);

    /**
     * @brief Handle provider's request to send next payment
     * @param deal  - current retrieval deal
     * @param event - EvSendPayment
     * @param from  - DealOngoing
     * @param to    - CreatePaymentChannel
     * @note possible occurring events:
     *              - EvCreatePaymentChannelError
     */
    static void createPaymentChannelHandler(std::shared_ptr<ClientDeal> deal,
                                            ClientEvent event,
                                            ClientState from,
                                            ClientState to);

    /**
     * @brief Handle error creating payment channel
     * @param deal  - current retrieval deal
     * @param event - EvCreatePaymentChannelError
     * @param from  - CreatingPaymentChannel
     * @param to    - DealFailed
     */
    static void createPaymentChannelErrorHandler(
        std::shared_ptr<ClientDeal> deal,
        ClientEvent event,
        ClientState from,
        ClientState to);

    /**
     * @brief Allocate lane in the successfully created payment channel
     * @param deal  - current retrieval deal
     * @param event - EvAllocateLane
     * @param from  - CreatingPaymentChannel
     * @param to    - AllocatingLane
     * @note possible occurring events:
     *              - EvAllocateLaneError
     */
    static void allocateLaneHandler(std::shared_ptr<ClientDeal> deal,
                                    ClientEvent event,
                                    ClientState from,
                                    ClientState to);

    /**
     * @brief Handle allocate lane error
     * @param deal  - current retrieval deal
     * @param event - EvAllocateLaneError
     * @param from  - AllocatingLane
     * @param to    - DealFailed
     */
    static void allocateLaneErrorHandler(std::shared_ptr<ClientDeal> deal,
                                         ClientEvent event,
                                         ClientState from,
                                         ClientState to);

    /**
     * @brief Add funds to payment channel
     * @param deal  - current retrieval deal
     * @param event - EvAddFunds
     * @param from  - AllocatingLane
     * @param to    - AddingFunds
     * @note possible occurring events:
     *              - EvAddFundsError
     */
    static void addFundsHandler(std::shared_ptr<ClientDeal> deal,
                                ClientEvent event,
                                ClientState from,
                                ClientState to);

    /**
     * @brief Add funds error handler
     * @param deal  - current retrieval deal
     * @param event - EvAddFundsError
     * @param from  - AddingFunds
     * @param to    - DealFailed
     */
    static void addFundsErrorHandler(std::shared_ptr<ClientDeal> deal,
                                     ClientEvent event,
                                     ClientState from,
                                     ClientState to);

    /**
     * @brief Create voucher and send it ot a provider
     * @param deal  - current retrieval deal
     * @param event - EvCreateVoucher
     * @param from  - AddingFunds
     * @param to    - CreatingVoucher
     * @note possible occurring events:
     *              - EvCreateVoucherError
     */
    static void createVoucherHandler(std::shared_ptr<ClientDeal> deal,
                                     ClientEvent event,
                                     ClientState from,
                                     ClientState to);

    /**
     * @brief Create voucher error handler
     * @param deal  - current retrieval deal
     * @param event - EvCreateVoucherError
     * @param from  - CreatingVoucher
     * @param to    - DealFailed
     */
    static void createVoucherErrorHandler(std::shared_ptr<ClientDeal> deal,
                                          ClientEvent event,
                                          ClientState from,
                                          ClientState to);

    /**
     * @brief Send payment to a provider error handler
     * @param deal  - current retrieval deal
     * @param event - EvWritePaymentError
     * @param from  - CreatingVoucher
     * @param to    - DealFailed
     */
    static void writePaymentErrorHandler(std::shared_ptr<ClientDeal> deal,
                                         ClientEvent event,
                                         ClientState from,
                                         ClientState to);

    /**
     * @brief Continue retrieval process after successful next payment
     * @param deal  - current retrieval deal
     * @param event - EvSendProposal
     * @param from  - CreatingVoucher
     * @param to    - DealOngoing
     */
    static void continueBlockConsumption(std::shared_ptr<ClientDeal> deal,
                                         ClientEvent event,
                                         ClientState from,
                                         ClientState to);

    /**
     * @brief Deal early termination handler
     * @param deal  - current retrieval deal
     * @param event - EvEarlyTermination
     * @param from  - DealOngoing
     * @param to    - DealFailed
     */
    static void earlyTerminationHandler(std::shared_ptr<ClientDeal> deal,
                                        ClientEvent event,
                                        ClientState from,
                                        ClientState to);

    /**
     * @brief Consume blocks error handler
     * @param deal  - current retrieval deal
     * @param event - EvBlockConsumeFail
     * @param from  - DealOngoing
     * @param to    - DealFailed
     */
    static void blockConsumptionFailHandler(std::shared_ptr<ClientDeal> deal,
                                            ClientEvent event,
                                            ClientState from,
                                            ClientState to);

    /**
     * @brief Write block to the local storage error handler
     * @param deal  - current retrieval deal
     * @param event - EvBlockWriteFail
     * @param from  - DealOngoing
     * @param to    - DealFailed
     */
    static void writeBlockFailHandler(std::shared_ptr<ClientDeal> deal,
                                      ClientEvent event,
                                      ClientState from,
                                      ClientState to);

    /**
     * @brief Verify block error handler
     * @param deal  - current retrieval deal
     * @param event - EvBlockVerifyFail
     * @param from  - DealOngoing
     * @param to    - DealFailed
     */
    static void verifyBlockHandler(std::shared_ptr<ClientDeal> deal,
                                   ClientEvent event,
                                   ClientState from,
                                   ClientState to);

    /**
     * @brief Handler of the unknown response from a provider
     * @param deal  - current retrieval deal
     * @param event - EvUnknownResopnse
     * @param from  - DealOngoing
     * @param to    - DealFailed
     */
    static void unknownResponseHandler(std::shared_ptr<ClientDeal> deal,
                                       ClientEvent event,
                                       ClientState from,
                                       ClientState to);

    /**
     * @brief Handle provider's request of the next payment, which violates
     * deal's terms
     * @param deal  - current retrieval deal
     * @param event - EvBadPaymentRequested
     * @param from  - DealOngoing
     * @param to    - DealFailed
     */
    static void badPaymentRequestHandler(std::shared_ptr<ClientDeal> deal,
                                         ClientEvent event,
                                         ClientState from,
                                         ClientState to);

    /**
     * @brief Handle completed deal
     * @param deal  - current retrieval deal
     * @param event - EvDealCompleted
     * @param from  - DealOngoing
     * @param to    - DealFinished
     */
    static void completeDealHandler(std::shared_ptr<ClientDeal> deal,
                                    ClientEvent event,
                                    ClientState from,
                                    ClientState to);

   private:
    std::vector<Transition> transitions_{
        /* EvSendProposal : DealNew -> DealOpen */
        Transition{ClientEvents::EvSendProposal}
            .from(ClientState::DealNew)
            .to(ClientState::DealOpen)
            .action(ClientDeal::sendProposalHandler),
        /* EvDealRejected : DealOpen -> DealRejected */
        Transition{ClientEvents::EvDealRejected}
            .from(ClientState::DealOpen)
            .to(ClientState::DealRejected)
            .action(ClientDeal::rejectDealHandler),
        /* EvDealAccepted : DealOpen -> DealOngoing */
        Transition{ClientEvents::EvDealAccepted}
            .from(ClientState::DealOpen)
            .to(ClientState::DealOngoing)
            .action(ClientDeal::acceptDealHandler),
        /* EvReceiveProgress : DealOngoing -> DealOngoing */
        Transition{ClientEvents::EvReceiveProgress}
            .from(ClientState::DealOngoing)
            .to(ClientState::DealOngoing)
            .action(ClientDeal::receiveBlocksHandler),
        /* EvSendPayment : DealOngoing -> CreatingPaymentChannel */
        Transition{ClientEvents::EvSendPayment}
            .from(ClientState::DealOngoing)
            .to(ClientState::CreatingPaymentChannel)
            .action(ClientDeal::createPaymentChannelHandler),
        /* EvAllocateLane : CreatingPaymentChannel -> AllocatingLane */
        Transition{ClientEvents::EvAllocateLane}
            .from(ClientState::CreatingPaymentChannel)
            .to(ClientState::AllocatingLane)
            .action(ClientDeal::allocateLaneHandler),
        /* EvCreatePaymentChannelError : CreatingPaymentChannel -> DealFailed */
        Transition{ClientEvents::EvCreatePaymentChannelError}
            .from(ClientState::CreatingPaymentChannel)
            .to(ClientState::DealFailed)
            .action(ClientDeal::createPaymentChannelErrorHandler),
        /* EvAddFunds : AllocatingLane -> AddingFunds */
        Transition{ClientEvents::EvAddFunds}
            .from(ClientState::AllocatingLane)
            .to(ClientState::AddingFunds)
            .action(ClientDeal::addFundsHandler),
        /* EvAllocatingLaneError : AllocatingLane -> DealFailed */
        Transition{ClientEvents::EvAllocatingLaneError}
            .from(ClientState::AllocatingLane)
            .to(ClientState::DealFailed)
            .action(ClientDeal::allocateLaneErrorHandler),
        /* EvCreateVoucher : AddingFunds -> CreatingVoucher */
        Transition{ClientEvents::EvCreateVoucher}
            .from(ClientState::AddingFunds)
            .to(ClientState::CreatingVoucher)
            .action(ClientDeal::addFundsHandler),
        /* EvAddFundsError : AddingFunds -> DealFailed */
        Transition{ClientEvents::EvAddFundsError}
            .from(ClientState::AddingFunds)
            .to(ClientState::DealFailed)
            .action(ClientDeal::addFundsErrorHandler),
        /* EvCreateVoucherError : CreatingVoucher -> DealFailed */
        Transition{ClientEvents::EvCreateVoucherError}
            .from(ClientState::CreatingVoucher)
            .to(ClientState::DealFailed)
            .action(ClientDeal::createVoucherErrorHandler),
        /* EvWritePaymentError : CreatingVoucher -> DealFailed */
        Transition{ClientEvents::EvWritePaymentError}
            .from(ClientState::CreatingVoucher)
            .to(ClientState::DealFailed)
            .action(ClientDeal::writePaymentErrorHandler),
        /* EvPaymentSent : CreatingVoucher -> DealOngoing */
        Transition{ClientEvents::EvPaymentSent}
            .from(ClientState::CreatingVoucher)
            .to(ClientState::DealOngoing)
            .action(ClientDeal::continueBlockConsumption),
        /* EvEarlyTermination : DealOngoing -> DealFailed */
        Transition{ClientEvents::EvEarlyTermination}
            .from(ClientState::DealOngoing)
            .to(ClientState::DealFailed)
            .action(ClientDeal::earlyTerminationHandler),
        /* EvBlockConsumeFail : DealOngoing -> DealFailed */
        Transition{ClientEvents::EvBlockConsumeFail}
            .from(ClientState::DealOngoing)
            .to(ClientState::DealFailed)
            .action(ClientDeal::blockConsumptionFailHandler),
        /* EvBlockWriteFail : DealOngoing -> DealFailed */
        Transition{ClientEvents::EvBlockWriteFail}
            .from(ClientState::DealOngoing)
            .to(ClientState::DealFailed)
            .action(ClientDeal::writeBlockFailHandler),
        /* EvBlockVerifyFail : DealOngoing -> DealFailed */
        Transition{ClientEvents::EvBlockVerifyFail}
            .from(ClientState::DealOngoing)
            .to(ClientState::DealFailed)
            .action(ClientDeal::writeBlockFailHandler),
        /* EvUnknownResponse : DealOngoing -> DealFailed */
        Transition{ClientEvents::EvUnknownResponse}
            .from(ClientState::DealOngoing)
            .to(ClientState::DealFailed)
            .action(ClientDeal::unknownResponseHandler),
        /* EvNetworkError : DealOpen -> DealFailed */
        Transition{ClientEvents::EvNetworkError}
            .from(ClientState::DealOpen)
            .to(ClientState::DealFailed),
        /* EvDealCompleted : DealOngoing -> DealFinished */
        Transition{ClientEvents::EvDealCompleted}
            .from(ClientState::DealOngoing)
            .to(ClientState::DealFinished)
            .action(ClientDeal::completeDealHandler)};
  };
}  // namespace fc::markets::retrieval::client

#endif
