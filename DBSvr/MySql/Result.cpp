#include <stdafx.h>
#include "Result.h"
#include "Row.h"

Result::Result(T_RESULT* res) : rowCount(-1), result(res), currentRow((Row*)NULL){}

Result::~Result()
{
	Close();
}

int Result::Next()
{
	T_ROW row;
	if (result == (T_RESULT*)NULL)
	{
		throw "Result set closed";
	}
#ifdef HAS_MSQL
	row = msqlFetchRow(result);
#elif defined HAS_MYSQL
	row = mysql_fetch_row(result);
#else
#error No database linked.
#endif
	if (!row)
	{
		currentRow = (Row*)NULL;
		return 0;
	}
	else
	{
		currentRow = new Row(result, row);
		return 1;
	}
}

Row* Result::GetCurrentRow()
{
	if (result == (T_RESULT*)NULL)
	{
		throw "Result set closed.";
	}

	return currentRow;
}

void Result::Close()
{
	if (result == (T_RESULT*)NULL)
	{
		return;
	}
#ifdef HAS_MSQL
	msqlFreeResult(result);
#elif defined HAS_MYSQL
	mysql_free_result(result);
#else
#error No database linked.
#endif
	result = (T_RESULT*)NULL;
}

int Result::GetRowCount()
{
	if (result == (T_RESULT*)NULL)
	{
		throw "Result set closed.";
	}

#ifdef HAS_MSQL
	rowCount = msqlNumRows(result);
#elif defined HAS_MYSQL
	rowCount = mysql_num_rows(result);
#endif
	if (rowCount > -1)
	{
		return rowCount;
	}
	return 0;
}