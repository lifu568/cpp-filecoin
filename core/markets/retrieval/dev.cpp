#include <chrono>
#include <future>
#include <iostream>
#include <thread>

struct Data {
  int content{};
};

class ExampleClient {
 public:
    Data getData() {
      auto data_promise = this->calculateData();
      auto data_future = data_promise.get_future();
      return data_future.get();
    }

  std::promise<Data> calculateData() {
    std::promise<Data> data_promise;
    std::thread([this, &data_promise]() {
      this->background(data_promise);
    }).detach();
    return data_promise;
  }

  void background(std::promise<Data> &data_promise) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    data_promise.set_value(Data{8});
  }
};



int main() {

}
