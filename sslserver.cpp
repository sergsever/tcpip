#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <exception>
#include <vector>

using std::cout;
using std::endl;
namespace ssl = boost::asio::ssl;
namespace io = boost::asio;
using boost::asio::ip::tcp;


int main()
{
	const char* port = "443";
	cout << "ssl server" << endl;
	try
	{
		io::io_context io_context;
		ssl::context ctx(ssl::context::sslv23);
		ctx.set_default_verify_paths();
		ctx.load_verify_file("/usr/src/tcpip/server.pem");
		tcp::endpoint addr(boost::asio::ip::tcp::v4(), 443);
		tcp::acceptor acceptor(io_context);
		ssl::stream<tcp::socket> *sclient;
		sclient = new ssl::stream<tcp::socket>(io_context, ctx);
		acceptor.open(addr.protocol());
		acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor.bind(addr);
		acceptor.listen();
		acceptor.accept(sclient->next_layer());
		sclient->handshake(ssl::stream_base::server);
		std::vector<char> buff(256);
		boost::system::error_code err;
		io::read(*sclient, io::buffer(buff), err);
		std::string msg(buff.begin(), buff.end());
		cout << "msg: " << msg << endl;



	}
	catch (std::exception& e)
	{
		cout << "exception: " << e.what() << endl;
	}

}