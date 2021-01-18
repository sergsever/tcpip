#include <iostream>
#include <boost/asio.hpp>
#include <exception>
#include <vector>

using namespace boost::asio;
using ip::tcp;

int main()
{
     std::cout << "hello from " << "tcpip" << std::endl;

	io_service io_service;
	try
	{
		tcp::endpoint endpoint_(tcp::v4(), 8080);

		tcp::acceptor acceptor_(io_service, endpoint_);
		tcp::socket client(io_service);
		acceptor_.accept(client);
		
		std::vector<char> msg(128);
		boost::system::error_code err_code;
		boost::asio::read(client, boost::asio::buffer(msg), err_code);
		
		std::string smsg(msg.begin(), msg.end());
		std::cout << "message: " << smsg << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}


    return 0;
}