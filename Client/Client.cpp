// Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include "boost/asio.hpp"
#include "../Server/Net/Buffer.h"
#include "../Server/Net/Socket.h"

using namespace boost::asio;

int _tmain(int argc, _TCHAR* argv[])
{
	MessageBuffer _sendbuff;
	io_service iosev;
	ip::tcp::socket socket(iosev);
	ip::tcp::endpoint ep(ip::address_v4::from_string("127.0.0.1"), 7566);
	boost::system::error_code ec;
	socket.connect(ep, ec);
	/*socket.connect(ep, ec);*/
	if ( ec )
	{
		std::cout << boost::system::system_error(ec).what() << std::endl;
		return -1;
	}

	while ( true )
	{
		char buf[100] = "xxxxxx";
		uint32_t _len = strlen(buf) + 1 + sizeof(uint32_t);
		_sendbuff << _len;
		_sendbuff << buf;
		socket.send(boost::asio::buffer(_sendbuff.GetReadPointer(),_sendbuff.GetActiveSize()));
		boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
	}

	socket.close();
	return 0;
}

