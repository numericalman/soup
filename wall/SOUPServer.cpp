#define TEST_SOUPSERVER
// g++ -std=c++11 -Wall -DBOOST_ALL_DYN_LINK <this-file> -lboost_chrono -lboost_thread

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <vector>
#include <atomic>
#include <string>

#include "SOUPServer.hpp"

using namespace std;
using boost::asio::ip::udp;


SOUPServer::
SOUPServer(boost::asio::io_context& io_context, short port)
          : socket_(io_context, udp::endpoint(udp::v4(), port)),
            strand_(boost::asio::make_strand(io_context)),
            timer_(io_context)
{
    start_receive();
}

void
SOUPServer::
start_receive()
{
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_),
        remote_endpoint_,
        boost::asio::bind_executor(
            strand_,
            boost::bind(
                &SOUPServer::handle_receive, this,
                boost::placeholders::_1,
                boost::placeholders::_2)));
}

void
SOUPServer::
handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
{
    if (!error) {
        cout << "Received request from " << remote_endpoint_ << endl;
        boost::thread(boost::bind(&SOUPServer::process_request, this));
    }
    start_receive();
}

void
SOUPServer::
process_request() 
{
    vector<char> send_buffer(1024, 'A');
    atomic<bool> ack_received{false};

    auto send_packets = [&]() {
        for (int i = 0; i < 100; ++i) {
            // boost::this_thread::sleep_for(boost::chrono::milliseconds(30));
            boost::system::error_code ec;
            size_t sent = socket_.send_to(boost::asio::buffer(send_buffer), remote_endpoint_, 0, ec);
            if (ec) {
                cout << "Error sending packet: " << ec.message() << endl;
                if (ec == boost::asio::error::would_block) {
                    cout << "Would block" << endl;
                    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
                    --i; // Retry this packet
                }
            } else {
                cout << "Sent packet " << i + 1 << " (" << sent << " bytes)" << endl;
            }
        }
        cout << "All packets sent. Waiting for ACK..." << endl;
    };

    auto wait_for_ack = [&]() {
        boost::array<char, 1024> ack_buffer;
        udp::endpoint sender_endpoint;
        boost::system::error_code ec;

        timer_.expires_after(boost::asio::chrono::seconds(10));
        timer_.async_wait([&](const boost::system::error_code& error) {
            if (!error && !ack_received) {
                cout << "Timeout: No ACK received within 10 seconds" << endl;
                ack_received = true; // To stop the receive loop
                socket_.cancel();
            }
        });

        socket_.async_receive_from(
            boost::asio::buffer(ack_buffer), sender_endpoint,
            [&](const boost::system::error_code& error, size_t bytes_received) {
                if (!error) {
                    string received_data(ack_buffer.data(), bytes_received);
                    if (*((int*)received_data.c_str()) == *((int*) "okey")) {
                        cout << "Received ACK ('okey') from " << sender_endpoint << endl;
                        ack_received = true;
                        timer_.cancel(); // Cancel the timeout timer
                    } else if (*((int*)received_data.c_str()) == *((int*) "ssss")) {
                        cout << "Received sleep request from " << sender_endpoint << ": " << received_data << endl;
                        sleep(15);
                        cout << "End sleep for request from " << sender_endpoint << ": " << received_data << endl;
                    } else {
                        cout << "Received non-ACK data from " << sender_endpoint << ": " << received_data << endl;
                    }
                } else if (error != boost::asio::error::operation_aborted) {
                    cout << "Error receiving ACK: " << error.message() << endl;
                    ack_received = true; // To stop the receive loop
                }
            }
        );

        // Run the io_context until the operation completes
        // boost::asio::io_context& io_context = socket_.get_executor().context();
        boost::asio::io_context& io_context = static_cast<boost::asio::io_context&>(socket_.get_executor().context());
        io_context.run();
    };

    boost::this_thread::sleep_for(boost::chrono::seconds(3));

    send_packets();
    wait_for_ack();

    if (!ack_received) {
        cout << "No valid ACK ('okey') received from client" << endl;
    }
}


#ifdef TEST_SOUPSERVER
int main() {
    try {
        boost::asio::io_context io_context;
        SOUPServer server(io_context, 12345);
        io_context.run();
    } catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
#endif
