#include <iostream>
#include <thread>

#include <ziapi/Http.hpp>

#include <AsioHttpsClient.hpp>

int main(int ac, char **av)
{
    if (ac < 3) {
        std::cerr << "Invalid number of arguments" << std::endl;
        return 84;
    }
    const std::string requestString{"GET toto HTTP/1.1\r\n"
                                    "User-Agent: PostmanRuntime/7.29.0\r\n"
                                    "Accept: */*\r\n"
                                    "Cache-Control: no-cache\r\n"
                                    "Postman-Token: ffa0b8c8-2d43-4805-9af2-7cfac22dc7f3\r\n"
                                    "Host: localhost:8080\r\n"
                                    "Accept-Encoding: gzip, deflate, br\r\n"
                                    "Connection: keep-alive\r\n"
                                    "Content-Length: 12\r\n"
                                    "Cookie: token=github|36404435\r\n\r\n"
                                    "Hello World!"
    };
    std::string ip(av[1]);
    std::size_t port = std::stol(av[2]);
    asio::io_context ctx;
//    try {
        std::cout << "Creating client..." << std::endl;
        network::https::AsioHttpsClient client(ctx);
        std::cout << "Client created" << std::endl;

//        try {
            client.connect(
                    network::Address{static_cast<std::uint32_t>(std::stoul(ip)), static_cast<std::uint16_t>(port)});
//        } catch (...) {
//            std::cerr << "Unable to connect to " << ip << ":" << port << std::endl;
//            return 84;
//        }
        std::cout << "Connecting to " << ip << ":" << port << std::endl;
        client.asyncReceive([](error::ErrorSocket const &, std::string &str) {
            std::cout << "Receiving: " << str << std::endl;
        });
        while (1) {
            client.asyncSend(requestString, [](error::ErrorSocket const &) {
                std::cout << "Client sent message." << std::endl;
            });
            sleep(3);
        }
//    } catch (...) {
//        std::cerr << "An error occurred" << std::endl;
//    }
    return 0;
}