#ifndef ROW_H
#define ROW_H

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#endif

#include <malloc.h>

#if defined(HAS_MSQL)
#include <msql.h>
#define T_RESULT m_result
#define T_ROW m_row
#elif defined(HAS_MYSQL)
#define T_RESULT MYSQL_RES
#define T_ROW MYSQL_ROW
#include <mysql.h>
#endif

class Row
{
public:
	Row(T_RESULT*, T_ROW);
	~Row();

	char *GetField(int index);
	int GetFieldCount();
	int IsClosed() const;
	void Close();
private:
	T_RESULT *result;
	T_ROW fields;
};

#endif