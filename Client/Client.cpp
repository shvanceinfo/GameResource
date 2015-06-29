// Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include "boost/asio.hpp"

using namespace boost::asio;

int _tmain(int argc, _TCHAR* argv[])
{
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
		char buf[100];
		socket.send(buffer("xxxxxx"));
		size_t len = socket.read_some(buffer(buf), ec);
		if ( ec )
		{
			break;
		}
		std::cout.write(buf, len);
	}

	socket.close();
	return 0;
}

