/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
#include <functional>
#include <future>

#include "common/outcome.hpp"

namespace fc::markets::retrieval::network {
  /**
   * @class Execute async callback operations
   * @tparam Error - type of the error, which code can be assigned to the
   * operation promise
   */
  template <typename Error>
  class AsyncOperation {
   public:
    using Operation = std::shared_ptr<std::promise<void>>;
    using Action = std::function<void(Operation)>;

    /**
     * @brief Run async operation and wait it's result
     * @param action - function, which will assign operation promise result
     * value or error
     * @return operation result
     */
    static outcome::result<void> run(Action action) {
      auto operation = std::make_shared<std::promise<void>>();
      action(operation);
      auto future = operation->get_future();
      try {
        future.get();
      } catch (std::system_error &error) {
        return static_cast<Error>(error.code().value());
      }
      return outcome::success();
    }

    /**
     * @brief Cancel async operation and return error
     * @param operation - async action
     * @param error - code of the operation error
     */
    static void failure(Operation operation, Error error) {
      std::error_code ec{static_cast<int>(error), std::generic_category()};
      std::system_error error_exception{ec};
      operation->set_exception(std::make_exception_ptr(error_exception));
    }
  };
}  // namespace fc::markets::retrieval::network
