#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
///////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                    //
// ver 3.3                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
// Author:			Ayush Khemka, Syracuse University, 538044584				 //
//							aykhemka@syr.edu																		 //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a 
* sequence of tokens that ends in {.  This has been extended to use the
* full set of terminators: {, }, ;, and '\n' if the line begins with #.
*
* Build Process:
* --------------
* Required Files: 
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp, itokcollection.h
* 
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.3 : 09 Feb 2016
* - final release
* ver 3.2 : 02 Feb 2016
* - declared SemiExp copy constructor and assignment operator = delete
* - added default argument for Toker pointer to nullptr so SemiExp
*   can be used like a container of tokens.
* - if pToker is nullptr then get() will throw logic_error exception
* ver 3.1 : 30 Jan 2016
* - changed namespace to Scanner
* - fixed bug in termination due to continually trying to read
*   past end of stream if last tokens didn't have a semiExp termination
*   character
* ver 3.0 : 29 Jan 2016
* - built in help session, Friday afternoon
*/

#include <vector>
#include "../Tokenizer/Tokenizer.h"
#include "../itokcollection.h"

namespace Scanner
{
  using Token = std::string;

  class SemiExp: public ITokCollection
  {
  public:
    SemiExp(Toker* pToker = nullptr);
    SemiExp(const SemiExp&) = delete;
    SemiExp& operator=(const SemiExp&) = delete;
		bool get(bool clear = true);
    std::string& operator[](int n);
    size_t length();
		std::string show(bool showNewLines = false);
		size_t find(const std::string& tok);
		bool isComment(std::string& token);
		void setReturnComments(bool returnComments = false) { _returnComments = returnComments; }
		bool doReturnComments() { return _returnComments; }
		void push_back(const std::string& tok);
		bool merge(const std::string& firstTok, const std::string& secondTok);
		bool remove(const std::string& tok);
		bool remove(size_t i);
		void toLower();
		void trimFront();
		void clear();
  private:
    std::vector<Token> _tokens;
    Toker* _pToker;
		bool _returnComments;
  };
}
#endif
