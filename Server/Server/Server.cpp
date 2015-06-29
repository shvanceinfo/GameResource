// Server.cpp : 定义控制台应用程序的入口点。

//http://www.tuicool.com/articles/zq26vqJ
#include "stdafx.h"
// #include "assert.h"
// //#include "signal.h"
// //#include "unistd.h"
// #include "iostream"
// #include "string"
// #include "deque"
// #include "set"
// #include "boost/asio.hpp"
// #include "boost/thread.hpp"
// #include "boost/bind.hpp"
// #include "boost/shared_ptr.hpp"
// #include "boost/enable_shared_from_this.hpp"
// #include "boost/thread/thread.hpp"
// #include "boost/date_time/posix_time/posix_time.hpp"
// #include "boost/atomic.hpp"
// #include <unordered_map>
// #include "map"
#include "../Net/Socket.h"

int main()
{
	SetupSignalHandler();
	IOServeicePtr io_service(new boost::asio::io_service());
	EchoServerPtr echo_server(new EchoServer(io_service));

	if ( !echo_server->Start("0.0.0.0",7566))
	{
		return -1;
	}

	boost::thread_group asio_threads;
	for (int i = 0; i < 64;++i)
	{
		asio_threads.create_thread(boost::bind(AsioThreadMain, io_service));
	}

	while (!g_shutdown_server)
	{
		Recv_Msg msg;
		if ( true == GetRecvMsg( msg ) )
		{
			std::cout << msg._msg << std::endl;
		}
	}
	echo_server->Stop();
	asio_threads.join_all();
	std::cout << "" << std::endl;
	return 0;
}