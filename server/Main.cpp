#include <iostream>
#include <boost/asio.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <unordered_map>

using namespace boost::asio;
struct UdpServer {
    explicit UdpServer(ip::udp::socket socket)
        : socket_(std::move(socket)) {
        read();
    }
private:
    void read() {
        socket_.async_receive_from(buffer(data_, 1500),
            remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t length) {

                if (ec)
                {
                    std::cerr << "udp read:" << ec << std::endl;
                    return;
                }

                data_[length] = '\0';

                if (strcmp(data_, "\n") == 0)
                {
                    return;
                }

                if (!clients.contains(remote_endpoint_)) {
                    clients.emplace(remote_endpoint_, data_);
                }

                boost::algorithm::to_upper(data_);
                std::cout << "Read from " << remote_endpoint_ << std::endl;
                this->write();
            }
        );
    }
    void write() {
        using namespace std::chrono_literals;

        std::string greeting = std::string("Hello ") + clients.at(remote_endpoint_);

        socket_.async_send_to(buffer(greeting),
            remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t) {
                if (ec) {
                    std::cerr << "udp write:" << ec << std::endl;
                    return;
                }
                socket_.async_send_to(buffer(data_, strlen(data_)),
                    remote_endpoint_,
                    [this](boost::system::error_code ec, std::size_t) {
                        if (ec) {
                            std::cerr << "udp write:" << ec << std::endl;
                            return;
                        }
                        std::cout << "Successfully answered " << data_ << std::endl;
                        data_[0] = '\0';
                        this->read();
                    }
                );
            }
        );
    }
    ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    std::unordered_map<boost::asio::ip::udp::endpoint, std::string> clients;
    char data_[1500 + 1]; // +1 for we can always null terminate safely
};


int main() {

    try {
        io_context io_context;

        ip::udp::endpoint ep(ip::udp::v6(), 1895); // also listens on ipv4
        ip::udp::socket sock(io_context, ep);
        UdpServer server(std::move(sock));
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}