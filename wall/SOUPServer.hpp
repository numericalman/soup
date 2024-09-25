#ifndef _SOUPSERVER_HPP_
# define _SOUPSERVER_HPP_

# include <boost/asio.hpp>
# include <boost/bind/bind.hpp>
# include <boost/thread.hpp>
# include <boost/array.hpp>
# include <iostream>
# include <vector>
# include <atomic>
# include <string>

// g++ -std=c++11 -Wall -DBOOST_ALL_DYN_LINK <this-file> -lboost_chrono -lboost_thread

using namespace std;
using boost::asio::ip::udp;


class SOUPServer 
{
private:
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, 1024> recv_buffer_;
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer timer_;

public:
    SOUPServer(boost::asio::io_context& io_context, short port);

private:
    void start_receive();
    void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);
    void process_request();
};
#endif // _SOUPSERVER_HPP_
