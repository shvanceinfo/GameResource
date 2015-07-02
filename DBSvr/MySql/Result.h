#ifndef RESULT_H
#define RESULT_H

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

#include "Row.h"

class Row;

class Result
{
public:
	Result(T_RESULT*);
	~Result();

	void Close();
	Row* GetCurrentRow();
	int GetRowCount();
	int Next();

private:
	int rowCount;
	T_RESULT* result;
	Row* currentRow;
};

#endif