/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 3.4                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Source:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
// Author:      Ayush Khemka, Syracuse University, 538044584       //
//              aykhemka@syr.edu		                               //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the functions of the Toker and ConsumeState
* classes. Classes derived from ConsumeState class help in reading of
* strings, alphanums, comments, punctuators, and whitespaces and new
* lines. This package also provides a default set of special single
* characters and character pairs, which can be changed. These help
* in tokenizing special operators like ++ and <<. Double quoted 
* strings and single quoted strings are handled differently. Comments
* are tokenized and returned, but its up to the user to display them
* or not, since a regular compiler will discard them while tokenizing.
*
* Build Process:
* --------------
* Required Files:
*   Tokenizer.h, Tokenizer.cpp
*
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.4 : 09 Feb 2016
* - final release
* ver 3.3 : 03 Feb 2016
* - final version given by Dr. Fawcett
*/

#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>

namespace Scanner
{
  class ConsumeState
  {
  public:
    ConsumeState();
    ConsumeState(const ConsumeState&) = delete;
    ConsumeState& operator=(const ConsumeState&) = delete;
    virtual ~ConsumeState();
    void attach(std::istream* pIn) { _pIn = pIn; }
    virtual void eatChars() = 0;
    void consumeChars() {
      _pState->eatChars();
      _pState = nextState();
    }
    bool canRead() { return _pIn->good(); }
    std::string getTok() { return token; }
    bool hasTok() { return token.size() > 0; }
    ConsumeState* nextState();
		bool isSpecialSingleCharacter(int sc);
		void setSpecialCharacter(std::string charset);
		bool isSpecialCharacterPair(int sp, int spNext);
		void setSpecialCharacterPair(std::string charset);
	protected:
		static std::string token;
    static std::istream* _pIn;
    static int prevChar;
    static int currChar;
		std::string ssc = "<>[](){}:=+-*\n";
		std::string scp = "<<>>::++--==+=-=*=/=";
		static ConsumeState* _pState;
    static ConsumeState* _pEatCppComment;
    static ConsumeState* _pEatCComment;
    static ConsumeState* _pEatWhitespace;
    static ConsumeState* _pEatPunctuator;
    static ConsumeState* _pEatAlphanum;
		static ConsumeState* _pEatString;
		static ConsumeState* _pEatSpecialSingleCharacter;
		static ConsumeState* _pEatSpecialCharacterPair;
    static ConsumeState* _pEatNewline;
  };
}

using namespace Scanner;

std::string ConsumeState::token;
std::istream* ConsumeState::_pIn = nullptr;
int ConsumeState::prevChar;
int ConsumeState::currChar;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatString = nullptr;
ConsumeState* ConsumeState::_pEatSpecialSingleCharacter = nullptr;
ConsumeState* ConsumeState::_pEatSpecialCharacterPair = nullptr;
ConsumeState* ConsumeState::_pEatNewline;

//---------------< test log to display the current state while debugging >----------

void testLog(const std::string& msg);

//----------------< change the set of special single characters by calling this function >---------

void ConsumeState::setSpecialCharacter(std::string charset) { ssc = charset; }

//----------------< change the set of special character pairs by calling this function >---------

void ConsumeState::setSpecialCharacterPair(std::string charset) { scp = charset; }

//--------------< check if the token is a special single character >-------------

bool ConsumeState::isSpecialSingleCharacter(int c) {
	if (ssc.find(c) < ssc.length()) return true;
	return false;
}

//-----------< check if the token and the next character make up a special character pair >-----------

bool ConsumeState::isSpecialCharacterPair(int sp, int spNext) {
	for (size_t i = 0;i < scp.length();i++)
		if (scp[i] == (char)sp&&scp[i + 1] == (char)spNext) return true;
	return false;
}

//-----------< calculate the next state to return >-------------

ConsumeState* ConsumeState::nextState() {
  if (!(_pIn->good())) {
    return nullptr;
  }
  int chNext = _pIn->peek();
  if (chNext == EOF) {
    _pIn->clear();
    // if peek() reads end of file character, EOF, then eofbit is set and
    // _pIn->good() will return false.  clear() restores state to good
  }
  if (std::isspace(currChar) && currChar != '\n') {
		testLog("state: eatWhitespace");
    return _pEatWhitespace;
  }
  if (currChar == '/' && chNext == '/') {
    testLog("state: eatCppComment");
    return _pEatCppComment;
  }
  if (currChar == '/' && chNext == '*') {
    testLog("state: eatCComment");
    return _pEatCComment;
  }
  if (currChar == '\n') {
    testLog("state: eatNewLine");
    return _pEatNewline;
  }
	if (std::isalnum(currChar) || currChar == '_') {
		testLog("state: eatAlphanum");
		return _pEatAlphanum;
	}
	if (ispunct(currChar) && !isSpecialSingleCharacter(currChar) && currChar != '\"' && currChar != '\'') {
		testLog("state: eatPunctuator");
		return _pEatPunctuator;
	}
	if (currChar == '\"' || currChar == '\'') {
		testLog("state: eatString");
		return _pEatString;
	}
	if (isSpecialSingleCharacter(currChar) && !isSpecialCharacterPair(currChar, chNext)) {
		testLog("state: eatSPecialSingleCharacter");
		return _pEatSpecialSingleCharacter;
	}
	if (isSpecialCharacterPair(currChar, chNext)) {
		testLog("state: eatSpecialCharacterPair");
		return _pEatSpecialCharacterPair;
	}
	if (!_pIn->good()) {
		return _pEatWhitespace;
	}
  throw(std::logic_error("invalid type"));
}

//-----------< eat white spaces but not new lines >------------

class EatWhitespace : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating whitespace";
    do {
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (std::isspace(currChar) && currChar != '\n');
  }
};

//----------------< eat CPP comments >------------

class EatCppComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating C++ comment";
    do {
			token += currChar;
			if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (currChar != '\n');
  }
};

//-------------< eat C Comments >------------

class EatCComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating C comment";
    do {
			token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (currChar != '*' || _pIn->peek() != '/');
		if (currChar == '*' && _pIn->peek() == '/') token += "*/"; //attach a */ at the end of the returned token
		_pIn->get();
    currChar = _pIn->get();
  }
};

//------< eat punctuators, but not the ones which belong to special characters or pairs >-------

class EatPunctuator : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating punctuator";
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
		} while (ispunct(currChar) && !isSpecialSingleCharacter(currChar));
  }
};

//--------< eat special single characters >-------------

class EatSpecialSingleCharacter : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating special single char";
		token = currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};

//------------< eat special character pairs >-----------

class EatSpecialCharacterPair : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating special char pair";
		token = currChar;
		token += _pIn->peek();
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		currChar = _pIn->get();
	}
};

//--------< eat quoted strings, with different cases for single and double quotes >------

class EatString : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating string";
		switch (currChar) {
		case '\"':
			do {
				token += currChar;
				if (!_pIn->good())  // end of stream
					return;
				currChar = _pIn->get();
			} while (currChar != '\"');
			//currChar = _pIn->get();
			if (currChar == '\"') token += '"';
			currChar = _pIn->get();
			break;
		case '\'':
			do {
				token += currChar;
				if (!_pIn->good())  // end of stream
					return;
				currChar = _pIn->get();
			} while (currChar != '\'');
			//currChar = _pIn->get();
			if (currChar == '\'') token += '\'';
			currChar = _pIn->get();
			break;
		}
	}
};

//-----------< eat alphanums including _ >-----------

class EatAlphanum : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating alphanum";
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
		} while (isalnum(currChar) || currChar == '_');
  }
};

//---------< eat new lines >---------

class EatNewline : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating alphanum";
    token += currChar;
    if (!_pIn->good())  // end of stream
      return;
    currChar = _pIn->get();
  }
};

//----------< constructor to initialize the states >-----------

ConsumeState::ConsumeState()
{
  static bool first = true;
	if (first)
  {
    first = false;
    _pEatAlphanum = new EatAlphanum();
    _pEatCComment = new EatCComment();
    _pEatCppComment = new EatCppComment();
    _pEatPunctuator = new EatPunctuator();
    _pEatWhitespace = new EatWhitespace();
    _pEatNewline = new EatNewline();
		_pEatString = new EatString();
		_pEatSpecialSingleCharacter = new EatSpecialSingleCharacter();
		_pEatSpecialCharacterPair = new EatSpecialCharacterPair();
    _pState = _pEatWhitespace;
  }
}

//---------< destructor to free memory >----------

ConsumeState::~ConsumeState()
{
  static bool first = true;
	if (first)
  {
    first = false;
    delete _pEatAlphanum;
    delete _pEatCComment;
    delete _pEatCppComment;
    delete _pEatPunctuator;
    delete _pEatWhitespace;
    delete _pEatNewline;
		delete _pEatSpecialSingleCharacter;
		delete _pEatSpecialCharacterPair;
		delete _pEatString;
  }
}

Toker::Toker() : pConsumer(new EatWhitespace()) {}

Toker::~Toker() { delete pConsumer; }

bool Toker::attach(std::istream* pIn)
{
  if (pIn != nullptr && pIn->good())
  {
		pConsumer->attach(pIn);
    return true;
  }
  return false;
}

//----------< keep getting tokens until a valid token is found >---------

std::string Toker::getTok()
{
  while(true) 
  {
    if (!pConsumer->canRead())
      return "";
    pConsumer->consumeChars();
    if (pConsumer->hasTok())
      break;
  }
  return pConsumer->getTok();
}

bool Toker::canRead() { return pConsumer->canRead(); }

void testLog(const std::string& msg)
{
#ifdef TEST_LOG
  std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
  std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
  
	std::ifstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open " << fileSpec << "\n\n";
    return 1;
  }
  Toker toker;
  toker.attach(&in);
  do
  {
    std::string tok = toker.getTok();
    if (tok == "\n")
      tok = "newline";
    std::cout << "\n -- " << tok;
  } while (in.good());

  std::cout << "\n\n";
  return 0;
}
#endif
