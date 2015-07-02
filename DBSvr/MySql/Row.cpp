#include <stdafx.h>
#include "Row.h"

Row::Row(T_RESULT* res, T_ROW row) :result(res), fields(row){}
Row::~Row()
{
	if (!IsClosed())
	{
		Close();
	}
}

void Row::Close()
{
	if (IsClosed())
	{
		throw "Row closed.";
	}

	fields = (T_ROW)NULL;
	result = (T_RESULT *)NULL;
}

int Row::GetFieldCount()
{
	if (IsClosed())
	{
		throw "Row Closed.";
	}
#ifdef HAS_MSQL
	return msqlNumFields(result);
#elif defined HAS_MYSQL
	return mysql_num_fields(result);
#endif
}

char *Row::GetField(int index)
{
	if (IsClosed())
	{
		throw "Row closed.";
	}

	if (index < 0 || index > GetFieldCount() - 1)
	{
		throw "Field index out of bounds.";
	}

	return fields[index];
}

int Row::IsClosed() const
{
	return (fields == (T_ROW)NULL);
}


