///////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - test suite for Scaneer			                   //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:			Ayush Khemka, Syracuse University, 538044584				 //
//							aykhemka@syr.edu																		 //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains test cases to test the Scanner as per the needs
* of Project #1. All the requirements have been completed successfully
* and have been displayed below.
*
* Build Process:
* --------------
* Required Files:
*   TestExecutive.cpp, SemiExpression.h, SemiExpression.cpp, Tokenizer.h,
*		Tokenizer.cpp, itokcollection.h
*
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 09 Feb 2016
* - first release
*/

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

int main()
{
	std::string underline(40, '=');
	std::string dash(40, '-');
	std::cout << "\n  Demonstrating Requirements for Project #1" << std::endl;
	std::cout << "  " << underline << std::endl;
	Toker toker;
	
	std::string fileSpec = "Tokenizer/test.txt";

	std::cout << "\n  Requirement #3: It can be seen in the source code that all three have been implemented" << std::endl;
	std::cout << "  " << dash << std::endl;

	// ----------< Read tokens >---------------

	std::cout << "\n  Requirement #4" << std::endl;
	std::cout << "  " << dash << std::endl;

	std::cout << "\n  Reading tokens from " << fileSpec << " until //SemiExp is encountered" << std::endl;
	std::cout << "  " << dash << std::endl;
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker.attach(&in);

	SemiExp semi(&toker);

	do
	{
		std::string tok = toker.getTok();
		if (tok == "//SemiExp") break;
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in.good());
	std::cout << "\n\n";

	std::cout << "\n  Requirement #5: The getTok() function has been implemented in Tokenizer.cpp line #431" << std::endl;
	std::cout << "  " << dash << std::endl;

	std::cout << "\n  Requirement #6, #7 and #8" << std::endl;
	std::cout << "  " << dash << std::endl;

	// --------------< make semi expressions >------------
	std::cout << "\n  Reading semi expressions without comments from " << fileSpec << std::endl;
	std::cout << "  " << dash << std::endl;
	std::cout << "\n  \"-- \" means a new semi expression" << std::endl;
	std::cout << "  " << dash << std::endl;

	/* Uncomment the following to return comments */
	//semi.setReturnComments(true);

	while (semi.get())
	{
		std::cout << "-- " << semi.show() << "\n";
	}
	if (semi.length() > 0)
	{
		std::cout << "-- " << semi.show() << "\n";
		std::cout << "\n\n";
	}

	std::cout << "\n  Requirement #9: It has been implemented in SemiExp.cpp line #126" << std::endl;
	std::cout << "  " << dash << std::endl;

	std::cout << "\n  Requirement #10: The methods have been implemented in SemiExp.cpp completely" << std::endl;
	std::cout << "  " << dash << std::endl;

	return 0;
}

