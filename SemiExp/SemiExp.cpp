///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.2                                                           //
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
* This package provides implementation of functions to  collect tokens
* and convert them into semi-expressions based on termination characters
* like {, }, ;, '\n' if starting with '#', and : if starting with either
* public, private, or protected. Comments are read but only displayed if
* the corressponding flag has been set. Other functions implement the
* itokcollection.h interface which contains functions like tolower(),
* merge(), and remove(), to be used in later projects. This package takes
* care of all the special cases mentioned in the Project #1 statement.
*
* Build Process:
* --------------
* Required Files:
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp,
*		itokcollection.h
*
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.2 : 09 Feb 2016
* - final release
* ver 3.1 : 29 Jan 2016
* - built in help session, Friday afternoon
*/

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

//-----------< constructor >------------

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

//-----------< clears the tokens >----------

void SemiExp::clear() { _tokens.clear(); }

//------------< removes the specified token from the list >-------------

bool SemiExp::remove(const std::string& token) {
	std::vector<Token>::iterator it;
	it = std::find(_tokens.begin(), _tokens.end(), token);
	if (it != _tokens.end()) {
		_tokens.erase(it);
		return true;
	}
	return false;
}

//---------------< removes the token at a specified index in the list >-----------

bool SemiExp::remove(size_t i) {
	if (i < 0 || _tokens.size() < i) return false;
	_tokens.erase(_tokens.begin() + i);							//http://stackoverflow.com/a/875109
	return true;
}

//---------------< converts each token to lower case >-------------

void SemiExp::toLower() {
	for (size_t i = 0;i < length();i++) {
		for (size_t j = 0;j < _tokens[i].length();j++)
			_tokens[i][j] = tolower(_tokens[i][j]);
	}
}

//--------------------< remove leading new lines if any >-------------

void SemiExp::trimFront() {
	if (_tokens.size() < 0) return;
	while (_tokens[0] == "\n" || _tokens[0] == "") {
		remove(0);
	}
}

//----------------< merge two tokens together, not required in this project >-------------

bool SemiExp::merge(const std::string& firstTok, const std::string& secondTok) { return true; }

//------------------< add a new token to the existing list >-----------------

void SemiExp::push_back(const std::string& token) {
	_tokens.push_back(token);
}

//---------------------< check if the received token is a C or CPP comment >-----------------

bool SemiExp::isComment(std::string& token) {
	if (token.length() < 0) return false;
	if (token[0] == '/' && (token[1] == '/' || token[1] == '*')) return true;
	return false;
}

//-------------------< look for a specific token in the list >------------------

size_t SemiExp::find(const std::string& tok) {
	for (size_t i = 0;i < length();i++)
		if (tok == _tokens[i])
			return i;
	return length();
}

/*-------------------< collect tokens and based on termination rules,
												make them into semi-expression >--------
*/

bool SemiExp::get(bool clear)
{
	if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	if (clear) _tokens.clear();
	while (true)
	{
		std::string token = _pToker->getTok();
		if (token == "")
			break;
		_tokens.push_back(token);

		if (token == "{" || token == "}" || token == ";")
			return true;
		if (token == "\n") {
			size_t n = find("#");
			if (n < length())
				return true;
		}
		if (isComment(token)) {
			if (doReturnComments()) return true;
			else _tokens.clear();
		}
		if (token == ":") {
			if (find("public") < length() || find("private") < length() || find("protected") < length())
				return true;
		}
	}
	return false;
}

//--------------------< overriding operator[] for the vector >----------------

std::string& SemiExp::operator[](int n)
{
  if (n < 0 || (size_t)n >= _tokens.size())
    throw(std::invalid_argument("index out of range"));
  return _tokens[n];
}

//-------------------< calculate size of vector >-----------------

size_t SemiExp::length()
{
  return _tokens.size();
}

//-----------------< display function >---------------------

std::string SemiExp::show(bool showNewLines)
{
	std::cout << "\n  ";
	std::string str;
	for (auto token : _tokens)
		if (token != "\n" || showNewLines)
			str.append(token).append(" ");
	std::cout<<" ";
	return str;
}

//-------------------< test stub >---------------

#ifdef TEST_SEMIEXP
int main()
{
  Toker toker;
  std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	std::fstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open file " << fileSpec << "\n\n";
    return 1;
  }
  toker.attach(&in);
	
  SemiExp semi(&toker);
	
	/* Uncomment the following to return comments */
	//semi.setReturnComments(true);
  
	while(semi.get())
  {
    std::cout << "\n  -- semiExpression --";
		std::cout << semi.show() << "\n";
  }
  /*
     May have collected tokens, but reached end of stream
     before finding SemiExp terminator.
   */
  if (semi.length() > 0)
  {
    std::cout << "\n  -- semiExpression --";
		std::cout << semi.show() << "\n";
    std::cout << "\n\n";
  }
  return 0;
}
#endif