#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/error.hpp>
#include <exception>
#include <vector>
#include <memory>

using std::cout;
using std::endl;
namespace ssl = boost::asio::ssl;
namespace io = boost::asio;
using boost::asio::ip::tcp;


int main()
{
	const char* port = "443";
	std::string cert = "server.pem";//"/etc/ssl/certs/GlobalSign_Root_CA.pem";
	cout << "ssl server" << endl;
	try
	{
		boost::asio::io_service io_context;
		ssl::context ctx(boost::asio::ssl::context::tlsv1_server);
		ctx.set_options(
			boost::asio::ssl::context::default_workarounds
			| boost::asio::ssl::context::no_sslv2
			| boost::asio::ssl::context::single_dh_use);
		ctx.use_certificate_chain_file(cert);
		ctx.use_private_key_file("privkey.pem", boost::asio::ssl::context::pem);
		ctx.use_tmp_dh_file("dh2048.pem");
//		ctx.set_verify_mode(ssl::verify_peer);
///		ctx.add_verify_path("/etc/ssl/certs");
//		ctx.set_default_verify_paths();
//		ctx.use_certificate_chain_file(/usr/src/tcpip/server.pem");
//		ctx.use_private_key_file(/*"/usr/src/tcpip/key.pem"*/cert.c_str(), boost::asio::ssl::context::pem);
//		ctx.use_certificate_chain_file(/*"/usr/src/tcpip/cert.pem"*/ cert);
//		ctx.load_verify_file("/usr/src/tcpip/tcp.pem");
//		ctx.use_tmp_dh_file("/usr/src/tcpip/dh2048.pem");
		std::unique_ptr<ssl::stream<tcp::socket>> sclient = std::make_unique<ssl::stream<tcp::socket>>(io_context, ctx);
		tcp::endpoint addr(boost::asio::ip::tcp::v4(), 443);
		tcp::acceptor acceptor(io_context);
//		sclient = new ssl::stream<tcp::socket>(io_context, ctx);
		acceptor.open(addr.protocol());
		acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor.bind(addr);
		acceptor.listen();
		cout << "accepting:" << endl;
		acceptor.accept(sclient->next_layer());
		cout << "handshaking" << endl;
		sclient->handshake(boost::asio::ssl::stream_base::server);
		std::vector<char> buff(256);
		boost::system::error_code err;
		io::read(*sclient, io::buffer(buff), err);
		cout << "readed." << endl;
		if (err.value() != 0 && err.value() != 1)
			cout << "read err: " << err.message() << "code: " << err.value() << endl;
		std::string msg(buff.begin(), buff.end());
		cout << "msg: " << msg << endl;
		std::string answer = "from ssl server";
		io::write(*sclient, boost::asio::buffer(answer), err);
		}
	catch (std::exception& e)
	{
		cout << "exception: " << e.what() << endl;
	}

}