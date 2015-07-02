#include <stdafx.h>
#include "Connection.h"
#include "Result.h"

MySqlConnection::MySqlConnection(const char* host, const char* dataBase)
{
#if defined(HAS_MSQL)
	connection = -1;
#elif defined(HAS_MYSQL)
	connection = (MYSQL*)NULL;
#else
#error No database linked.
#endif
	Connect(host, dataBase, (const char*)NULL, (const char*)NULL);
}

MySqlConnection::MySqlConnection(const char* host, const char* dataBase, const char* userID, const char* password) : affectedRows(0)
{
#if defined(HAS_MSQL)
	connection = -1;
#elif defined(HAS_MYSQL)
	connection = (MYSQL*)NULL;
#else
#error No database linked;
#endif
	Connect(host, dataBase, userID, password);
}

void MySqlConnection::Connect(const char* host, const char* database, const char* userID, const char* password)
{
	int state = 0;
	if (IsConnected())
	{
		throw "Connection has been established";
	}

#if defined(HAS_MSQL)
	connection = msqlConnect(host);
	state = msqlSelectDB(connection, database);
#elif defined(HAS_MYSQL)
	mysql_init(&mysql);
	connection = mysql_real_connect(&mysql, host, userID, password, database, 0, NULL, 0);
#else
#error No database lined;
#endif
	if (!IsConnected())
	{
		throw GetError();
	}

	if (state < 0)
	{
		throw GetError();
	}
}

MySqlConnection::~MySqlConnection()
{
	if (IsConnected())
	{
		Close();
	}
}

void MySqlConnection::Close()
{
	if (!IsConnected())
	{
		return;
	}

#ifdef HAS_MSQL
	msqlClose(connection);
	connection = -1;
#elif defined HAS_MYSQL
	mysql_close(connection);
	connection = (MYSQL*)NULL;
#else
#error No database linked;
#endif
}

Result* MySqlConnection::Query(const char* sqlCommand)
{
	T_RESULT* result;
	int state;
	if (!IsConnected())
	{
		throw "DataBase not connected.";
	}

#ifdef HAS_MSQL
	state = mysqlQuery(connection, sqlCommand);
#elif defined HAS_MYSQL
	state = mysql_query(connection, sqlCommand);
#else
#error No database linked;
#endif

	if (0 != state)
	{
		throw GetError();
	}

#ifdef HAS_MSQL
	result = msqlStoreResult();
#elif defined HAS_MYSQL
	result = mysql_store_result(connection);
#else
#error No database linked.
#endif

	if (result == (T_RESULT*)NULL)
	{
#ifdef HAS_MSQL
		affectedRows = state;
#elif defined HAS_MYSQL
		int fieldCount = mysql_field_count(connection);
		if (0 != fieldCount)
		{
			throw GetError();
		}
		else
		{
			affectedRows = mysql_affected_rows(connection);
		}
#else
#error No database linked.
#endif // HAS_MSQL

		return (Result*)NULL;
	}

	return new Result(result);
}

int MySqlConnection::GetAffectedRows() const
{
	return affectedRows;
}

const char* MySqlConnection::GetError()
{
#ifdef HAS_MSQL
	return msqlErrMsg;
#elif defined HAS_MYSQL
	if (IsConnected())
	{
		return mysql_error(connection);
	}
	else
	{
		return mysql_error(&mysql);
	}
#else
#error No database linked.
#endif
}

int MySqlConnection::IsConnected() const
{
#ifdef HAS_MSQL
	return connection > 0;
#elif defined HAS_MYSQL
	return (connection != NULL);
#else
#error No database linked.
#endif
}