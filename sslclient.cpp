#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <boost/asio/ssl.hpp>
#include <vector>
#include <example/common/root_certificates.hpp>

using std::cout;
using std::endl;
//using boost::asio::io_context;
using boost::asio::connect;
using boost::asio::ip::tcp;
typedef ssl::stream<tcp::socket> ssl_socket;
using namespace boost::asio;

auto cb = [](bool preverified, boost::asio::ssl::verify_context& ctx) {
	char subject_name[256];
	X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
	X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);

	std::cout << "SSL Verify: " << subject_name << "\n";

	return true;
};

std::string cert = "/etc/ssl/certs/GlobalSign_Root_CA.pem";

int main()
{
	const char* host = "localhost";
	unsigned short port = 443;
	std::cout << "ssl client" << std::endl;
	try
	{
		boost::asio::io_service io;
		boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv1_client);
//		ctx.set_verify_mode(ssl::verify_peer);
//		ctx.load_verify_file(cert);
//		ctx.add_verify_path("./");
		load_root_certificates(ctx);
		ctx.set_default_verify_paths();
//		ctx.use_certificate_chain_file("/usr/src/tcpip/cert.pem");
//		ctx.use_tmp_dh_file("/usr/src/tcpip/dh2048.pem");
		boost::asio::ssl::stream<tcp::socket> ssl_sock(io, ctx);
		ssl_sock.set_verify_callback(cb);
		tcp::resolver resolver(io);
//		tcp::resolver::query query(host, port);
		auto handle = ssl_sock.native_handle();
		std::cout << "ssl set host t: " << typeid(ssl_sock.native_handle()).name() << std::endl;
		if (!SSL_set_tlsext_host_name((SSL*)ssl_sock.native_handle(), host))
		{
			std::cout << "set host: err: " << std::endl;
			boost::system::error_code ec{ static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
			throw boost::system::system_error{ ec };
		}
		std::cout << "after set host" << std::endl;
		boost::asio::ip::tcp::resolver::query query(host, "https");

//		ip::tcp::endpoint addr(ip::address::from_string(host), port);
		std::cout << "was addr" << std::endl;
		auto const res = resolver.resolve(query);
		std::cout << "ssl connecting" << std::endl;
		boost::asio::connect(ssl_sock.next_layer(), res);
		std::cout << "ssl connected" << std::endl;
		ssl_sock.lowest_layer().set_option(tcp::no_delay(true));
		/*handshake*/
//		ssl_sock.set_verify_mode(boost::asio::ssl::verify_peer);
//		ssl_sock.set_verify_callback(ssl::rfc2818_verification(host));
		std::cout << "handshake:" << std::endl;
		ssl_sock.handshake(boost::asio::ssl::stream_base::client);
		std::string req = "GET / HTTP/1.0\r\nhost:config.me\r\n\r\n";
		boost::system::error_code err;
		write(ssl_sock, boost::asio::buffer(req), err);
		std::cout << "write err: " << err << std::endl;
	std::vector<char> resp(256);
	read(ssl_sock, boost::asio::buffer(resp), err);
		std::string sresp(resp.begin(), resp.end());
		std::cout << "read: err: " << err << "resp: " << sresp << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}



}