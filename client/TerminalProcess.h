#ifndef __TERMINALPROCESS_H__
#define __TERMINALPROCESS_H__
#include "Noncopyable.hpp"

#include <termio.h>
#include <unistd.h>
#include <stdio.h>

#include <string>

using std::string;

namespace wk{

size_t nBytesCode(const char ch);

class TerminalProcess : Noncopyable
{
public:
    static TerminalProcess* getInstance();
    static void destroy();
        

    static int getCharacter(string& temp);
    int getCharacter1(string& temp);
    static void show(const string& temp);
    static void show(const char c);
    static void showEnter(const string& temp);
    static void showEnter();
    static bool isEng(const char ch);
    void getAll();
    bool isEmpty();

public:
    void setOriginalMode();
    void reduction();
private:
    TerminalProcess();
    ~TerminalProcess();
    static TerminalProcess* _pInstance;

    struct termios _tmOld;
    struct termios _tmNew;
    string _str;

};

}//end of namespace wk

#endif

