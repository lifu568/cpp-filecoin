#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <thread>

#include "markets/retrieval/dev.hpp"
#include "markets/retrieval/network/async_operation.hpp"

struct Data {
  int content{};
};

class ExampleClient {
 public:
  using AsyncProcessor = fc::markets::retrieval::network::AsyncProcessor;

  void test() {
    std::ignore = AsyncProcessor::run<Dev::ExampleError>(
        [this](std::shared_ptr<std::promise<void>> operation) {
          this->background(operation);
        });
  }

  void background(std::shared_ptr<std::promise<void>> operation) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "... data ..." << std::endl;
    operation->set_value();
  }
};

OUTCOME_CPP_DEFINE_CATEGORY(Dev, ExampleError, e) {
  return "Example error";
}

int main() {
  ExampleClient client;
  std::cout << "Waiting operation result" << std::endl;
  client.test();
  std::cout << "Finished" << std::endl;
}
