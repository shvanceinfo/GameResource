// #ifndef __SOCKET_H__
 // #define __SOCKET_H__
 // 
 // #include "boost/asio/ip/tcp.hpp"
 // #include "boost/asio/write.hpp"
 // #include "boost/asio/read.hpp"
 // #include "atomic"
 // #include "mutex"
 // #include "queue"
 // 
 // using boost::asio::ip::tcp;
 // 
 // class MessageBuffer;
 // 
 // template< class T >
 // class Socket : public std::enable_shared_from_this<T>
 // {
 // 	explicit Socket(tcp::socket && socket) :_socket(std::move(socket)), _remoteAddress(socket.remote_endpoint().address()), _remotePort(socket.remote_endpoint().port())
 // 	{
 // 		_buffer = new std::shared_ptr<MessageBuffer>;
 // 		_session = new std::shared_ptr<T>;
 // 	}
 // 
 // 	virtual ~Socket()
 // 	{
 // 		boost::system::error_code error;
 // 		_socket.close(error);
 // 	}
 // 
 // 	boost::asio::ip::address GetRemoteAddress()
 // 	{
 // 		return _remoteAddress;
 // 	}
 // 
 // 	uint16_t GetRemotePort()
 // 	{
 // 		return _remotePort;
 // 	}
 // 
 // 	void AsyncRead()
 // 	{
 // 		_buffer->Normalize();
 // 		_socket.async_read_some( boost::asio::buffer(_buffer->GetWritePointer(), MESSAGE_MAX_LENGTH),
 // 			std::bind(&Socket<T>::)
 // 	}
 // 
 // 	void ReadHandlerInternal(boost::system::error_code error, size_t transferedBytes)
 // 	{
 // 		if ( error)
 // 		{
 // 			CloseSocket();
 // 			return;
 // 		}
 // 
 // 		_buffer->WriteComplete(transferedBytes);
 // 		ReadHandler();
 // 	}
 // 
 // 	virtual void ReadHandler() = 0;
 // 
 // 	void CloseSocket()
 // 	{
 // 		if (_closed )
 // 		{
 // 			return;
 // 		}
 // 
 // 		boost::system::error_code shutdownError;
 // 		_socket.shutdown(boost::asio::socket_base::shutdown_send, shutdownError);
 // 		if ( shutdownError)
 // 		{
 // 			//TODO
 // 		}
 // 	}
 // 
 // 	void QueuePacket(MessageBuffer &&buffer, std::unique_lock<std::mutex> &guard)
 // 	{
 // 		_writeQueue.push(std::move(buffer));
 // 	}
 // 
 // 	bool AsyncProcessQueue(std::unique_lock<std::mutex>&)
 // 	{
 // 		if ( _isWritingAsync)
 // 		{
 // 			return false;
 // 		}
 // 
 // 		MessageBuffer & buffer = _writeQueue.front();
 // 		//////////////////////////////////////////////////////////////////////////
 // 		//////////////////////////////////////////////////////////////////////////
 // 	}
 // 
 // private:
 // 	tcp::socket _socket;
 // 	boost::asio::ip::address _remoteAddress;
 // 	uint16_t _remotePort;
 // 	std::shared_ptr<MessageBuffer> _buffer;
 // 	std::shared_ptr<T> _session;
 // 	std::queue<MessageBuffer> _writeQueue;
 // 
 // 	bool _closed;
 // 	bool _closing;
 // };
 // 
 //  #endif


#ifndef __SOCKET_H__
#define __SOCKET_H__

#ifndef _WIN32
#include "signal.h"
#include "unistd.h"
#endif

#include "assert.h"
#include "iostream"
#include "string"
#include "deque"
#include "set"
#include "boost/asio.hpp"
#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/thread/thread.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/atomic.hpp"
#include "Buffer.h"


#define LOCK_GUARD(x) boost::lock_guard<boost::mutex> guard(x);

namespace
{

	struct Recv_Msg
	{
		boost::shared_ptr<boost::asio::ip::tcp::socket> pSocket_;
		std::string _msg;

		Recv_Msg() :pSocket_(NULL), _msg(""){}
		Recv_Msg(boost::shared_ptr<boost::asio::ip::tcp::socket> _p, std::string _temp) : pSocket_(_p), _msg(_temp){}
		Recv_Msg(const Recv_Msg &_temp) : pSocket_(_temp.pSocket_), _msg(_temp._msg){}
		Recv_Msg& operator=(const Recv_Msg& msg)
		{
			if (this == &msg)
			{
				return *this;
			}

			this->pSocket_ = msg.pSocket_;
			this->_msg = msg._msg;
			return *this;
		}
	};
	std::list<Recv_Msg> Recv_Msg_List;
	boost::mutex Recv_Msg_Mutex;

	bool GetRecvMsg(Recv_Msg &msg)
	{
		LOCK_GUARD(Recv_Msg_Mutex);
		if (Recv_Msg_List.size())
		{
			msg = Recv_Msg_List.front();
			Recv_Msg_List.pop_front();
			return true;
		}

		return false;
	}

	class EchoServer;
	typedef boost::shared_ptr<EchoServer> EchoServerPtr;
	typedef boost::shared_ptr<boost::asio::io_service> IOServeicePtr;
	typedef boost::shared_ptr<boost::asio::ip::tcp::socket> SocketPtr;

	class Connection;
	typedef boost::shared_ptr<Connection> ConnPtr;
	typedef boost::shared_ptr<std::string> StringPtr;
	typedef boost::shared_ptr<boost::asio::deadline_timer> TimerPtr;

	class Connection : public boost::enable_shared_from_this<Connection>
	{
	public:
		enum ConnStatus
		{
			kConnected = 0,
			kError = 1,
			kClosed = 2,
		};

		Connection(SocketPtr socket) : status_(kConnected), socket_(socket), msgbuf_(){}
		~Connection()
		{
			std::cout << __FUNCTION__ << std::endl;
		}

		void Start()
		{
			socket_->async_receive(boost::asio::buffer(msgbuf_.GetReadPointer(), msgbuf_.GetRemainingSpace()), boost::bind(&Connection::ReadHandler, shared_from_this(), _1, _2));
		}

		void Close()
		{
			if (status_.exchange(kClosed) != kClosed)
			{
				LOCK_GUARD(socket_mutex);
				boost::system::error_code errcode;
				if (socket_->close(errcode))
				{
					std::cerr << "" << std::endl;
				}
				else
				{
					std::cerr << "" << std::endl;
				}
			}
		}

		ConnStatus status() { return status_.load(); }

	private:
		void ReadHandler(const boost::system::error_code & error, std::size_t bytes_transferred)
		{
			if (!error)
			{
				{
					LOCK_GUARD(socket_mutex);
					socket_->async_receive(boost::asio::buffer(msgbuf_.GetReadPointer(), msgbuf_.GetRemainingSpace()), boost::bind(&Connection::ReadHandler, shared_from_this(), _1, _2));
				}

				if ( uint32_t _len = msgbuf_.read<uint32_t>(sizeof(uint32_t)))
				{
					//TODO不知道会不会死锁,socket_mutex需要测试一下
					if ( _len >= msgbuf_.GetActiveSize() )
					{
						uint8_t *byptes = new uint8_t[_len+1];
						msgbuf_.read(byptes,_len);
						std::string _temp;
						msgbuf_.Normalize();
						Recv_Msg msg(socket_, _temp);
						LOCK_GUARD(Recv_Msg_Mutex);
						Recv_Msg_List.push_back(msg);
					}
				}
			}
			else if (error == boost::asio::error::operation_aborted)
			{
				std::cout << "" << std::endl;
			}
			else
			{
				ConnStatus expected = kConnected;
				if (status_.compare_exchange_strong(expected, kError))
				{
					std::cout << "" << std::endl;
				}
			}
		}

		void WriteHandler(const boost::system::error_code& error, std::size_t bytes_transferred)
		{
			if (!error)
			{
				LOCK_GUARD(socket_mutex);
				write_queue_.pop_front();
				if (write_queue_.size())
				{
					StringPtr next_msg = write_queue_.front();
					async_write(*socket_, boost::asio::buffer(*next_msg), boost::bind(&Connection::WriteHandler, shared_from_this(), _1, _2));
				}
			}
			else if (error == boost::asio::error::operation_aborted)
			{
				std::cout << "" << std::endl;
			}
			else
			{
				ConnStatus expected = kConnected;
				if (status_.compare_exchange_strong(expected, kError))
				{
					std::cout << "" << std::endl;
				}
			}
		}

		void EchoMsg(StringPtr msg)
		{
			LOCK_GUARD(socket_mutex);
			write_queue_.push_back(msg);
			if (write_queue_.size() == 1)
			{
				async_write(*socket_, boost::asio::buffer(*msg), boost::bind(&Connection::WriteHandler, shared_from_this(), _1, _2));
			}
		}

		std::deque<StringPtr> write_queue_;
		boost::mutex socket_mutex;
		boost::atomic<ConnStatus> status_;
		//char msgbuf_[1024 * 16];
		MessageBuffer msgbuf_;
		SocketPtr socket_;

	};

	class EchoServer : public boost::enable_shared_from_this<EchoServer>
	{
	public:
		EchoServer(IOServeicePtr io_service) : stopped_(false), io_service_(io_service), acceptor_(*io_service){}
		~EchoServer()
		{
			std::cout << __FUNCTION__ << std::endl;
			LOCK_GUARD(conn_set_mutex_);
			for (ConneSetIter iter = conn_set_.begin(); iter != conn_set_.end(); iter++)
			{
				(*iter)->Close();
			}
		}

		bool Start(const std::string &host, unsigned short port)
		{
			boost::system::error_code errcode;
			boost::asio::ip::address address = boost::asio::ip::address::from_string(host, errcode);
			if (errcode)
			{
				return false;
			}

			if (acceptor_.open(boost::asio::ip::tcp::v4(), errcode))
			{
				return false;
			}

			acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
			boost::asio::ip::tcp::endpoint endpoint(address, port);
			if (acceptor_.bind(endpoint, errcode) || acceptor_.listen(1024, errcode))
			{
				return false;
			}

			SocketPtr socket(new boost::asio::ip::tcp::socket(*io_service_));
			acceptor_.async_accept(*socket, boost::bind(&EchoServer::AcceptHandler, shared_from_this(), socket, _1));
			return true;
		}

		void Stop()
		{
			boost::system::error_code errcode;
			if (acceptor_.close(errcode))
			{
				std::cerr << "" << std::endl;
			}
			stopped_.store(true);
		}

	private:
		void AcceptHandler(SocketPtr socket, const boost::system::error_code& error)
		{
			if (error == boost::asio::error::operation_aborted)
			{
				std::cout << "" << std::endl;
				return;
			}
			else if (!error)
			{
				std::cout << "" << std::endl;
				ConnPtr new_conn(new Connection(socket));
				new_conn->Start();
				{
					LOCK_GUARD(conn_set_mutex_);
					conn_set_.insert(new_conn);
				}

				TimerPtr socket_time(new boost::asio::deadline_timer(*io_service_));
				socket_time->expires_from_now(boost::posix_time::seconds(1));
				socket_time->async_wait(boost::bind(&EchoServer::CheckSocketStatus, shared_from_this(), new_conn, socket_time, _1));
			}
			else
			{
				std::cout << "" << std::endl;
			}

			SocketPtr new_socket(new boost::asio::ip::tcp::socket(*io_service_));
			acceptor_.async_accept(*new_socket, boost::bind(&EchoServer::AcceptHandler, shared_from_this(), new_socket, _1));
		}

		void CheckSocketStatus(ConnPtr conn, TimerPtr socket_timer, const boost::system::error_code& error)
		{
			LOCK_GUARD(conn_set_mutex_);
			ConneSetIter iter = conn_set_.find(conn);
			assert(iter != conn_set_.end());
			if (stopped_.load())
			{
			}
			else if (conn->status() == Connection::kError)
			{
				conn->Close();
				conn_set_.erase(conn);
			}
			else if (conn->status() == Connection::kClosed)
			{
				conn_set_.erase(conn);
			}
			else
			{
				socket_timer->expires_from_now(boost::posix_time::seconds(1));
				socket_timer->async_wait(boost::bind(&EchoServer::CheckSocketStatus, shared_from_this(), conn, socket_timer, _1));
			}
		}

		typedef std::set<ConnPtr> ConnSet;
		typedef ConnSet::iterator ConneSetIter;
		boost::atomic<bool> stopped_;
		boost::mutex conn_set_mutex_;
		ConnSet conn_set_;
		IOServeicePtr io_service_;
		boost::asio::ip::tcp::acceptor acceptor_;
	};

	volatile sig_atomic_t g_shutdown_server = 0;
	void ShutdownServerHandler(int signo)
	{
		g_shutdown_server = 1;
	}

	void SetupSignalHandler()
	{
#ifndef _WIN32
		sigset_t sigset;
		sigfillset(&sigset);
		sigdelset(&sigset, SIGTERM);
		sigdelset(&sigset, SIGINT);
		sigprocmast(SIG_SETMASK, &sigset, NULL);

		struct sigaction act;
		memset(&act, 0, sizeof(act));
		act.sa_handler = ShutdownServerHandler;
		sigaction(SIGINT, &act, NULL);
		sigaction(SIGTERM, &act, NULL);
#endif
	}

	void AsioThreadMain(IOServeicePtr io_service)
	{
		io_service->run();
	}
}



#endif