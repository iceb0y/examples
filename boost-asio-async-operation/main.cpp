#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/bind.hpp>

namespace asio = boost::asio;

namespace {

asio::io_service io_service;

template <typename CompletionTokenT>
auto async_hello(int a, int b, CompletionTokenT &&token) {
    typename asio::handler_type<CompletionTokenT, void(int)>::type handler(
        std::forward<CompletionTokenT>(token));
    asio::async_result<decltype(handler)> result(handler);
    io_service.post([a, b, handler]() mutable {
        using asio::asio_handler_invoke;
        asio_handler_invoke(std::bind(handler, a + b), &handler);
    });
    return result.get();
}

const auto coro_entry = [](asio::yield_context yield) {
    while (true) {
        std::cout << "1 + 2 = " << async_hello(1, 2, yield) << std::endl;
        io_service.post(yield);
    }
};

}

int main() {
    asio::spawn(io_service, coro_entry);
    std::thread thread;
    for (int i = 0; i < 10; ++i) {
        thread = std::thread([thread = std::move(thread)]() mutable { 
            io_service.run();
            if (thread.joinable()) {
                thread.join();
            }
        });
    }
    thread.join();
}
