// DBSvr.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include "./MySql/Connection.h"
#include "./MySql/Row.h"
#include "./MySql/Result.h"


int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		MySqlConnection mysqlConnectio("localhost", "TestDataBase", "root", "root");
		std::auto_ptr<Result> queryResult;

//		int affectedRows = 0;
// 		mysqlConnectio.Query("insert into Student values('2007','Jay',27);");
// 		affectedRows = mysqlConnectio.GetAffectedRows();
		queryResult.reset(mysqlConnectio.Query("select * from Student;"));
		while (queryResult->Next())
		{
			std::auto_ptr<Row> row;
			row.reset( queryResult->GetCurrentRow() );
			std::cout << row->GetField(0) << std::endl;
			std::cout << row->GetField(1) << std::endl;
			std::cout << row->GetField(2) << std::endl;
		}

		std::cout << queryResult->GetRowCount();
		queryResult->Close();
	}
	catch (const char* exceptionString)
	{
		std::cout << exceptionString;
	}
	return 0;
}

