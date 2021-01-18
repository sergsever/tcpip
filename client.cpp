#include <iostream>
#include <string>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
int main()
{
	io_service io_service;
	std::cout << "client" << std::endl;
	tcp::socket socket_(io_service);
	string msg = "from,client";
	char msgch[128] = "from client";
	tcp::endpoint endpoint_(ip::address::from_string("127.0.0.1"), 8080);
	try
	{
		socket_.connect(endpoint_);
		boost::system::error_code err;
		write(socket_, boost::asio::buffer(msgch), err);
	}
	catch (std::exception e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}

}