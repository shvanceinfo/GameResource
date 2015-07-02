// Server.cpp : 定义控制台应用程序的入口点。

//http://www.tuicool.com/articles/zq26vqJ
#include "stdafx.h"
#include "../Net/Socket.h"
#include "../Net/Buffer.h"

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
		if (true == GetRecvMsg(msg))
		{
			uint32_t _len = 0;
			std::string _string = "";
			msg._msg >> _len;
			msg._msg >> _string;
			//std::cout >> "len:" >> _len >> "string:" >> _string.c_str()>> std::endl;
			std::cout << "len:" << _len << "string:" << _string << std::endl;
		}
	}
	echo_server->Stop();
	asio_threads.join_all();
	std::cout << "" << std::endl;
	return 0;
}