#ifndef CONNECTION_H
#define CONNECTION_H
//http ://www.cnblogs.com/-Lei/archive/2012/11/20/2779357.html

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/time.h>
#endif

#if defined(HAS_MSQL)
#include <msql.h>
#elif defined(HAS_MYSQL)
#include <mysql.h>
#endif

class Result;

class MySqlConnection
{
public:
	MySqlConnection(const char*, const char*);
	MySqlConnection(const char*, const char*, const char*, const char*);
	~MySqlConnection();

	void Close();
	void Connect(const char* host, const char* database, const char* userID, const char* password);
	int GetAffectedRows() const;
	const char* GetError();
	int IsConnected() const;
	Result* Query(const char* sqlCommand);
private:
	int affectedRows;
#if defined(HAS_MSQL)
	int connection;
#elif defined(HAS_MYSQL)
	MYSQL mysql;
	MYSQL* connection;
#else
#error No database defined.
#endif

};

#endif