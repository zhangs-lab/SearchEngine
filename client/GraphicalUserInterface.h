#ifndef __GRAPHICALUSERINTERFACE_H__
#define __GRAPHICALUSERINTERFACE_H__

#include "TerminalProcess.h"
#include "SearchBar.h"
#include "candidateBox.h"
#include "Noncopyable.hpp"

#include <signal.h>

#include <vector>
#include <memory>

using std::vector;
namespace wk{


void sigFun(int sig, siginfo_t *p, void *p1);

enum tool{
    SEARCHBAR,
    CANDIDATEBOX
};

class winComponent;
using winComponentPtr = std::shared_ptr<winComponent>;

class GraphicalUserInterface : Noncopyable
{
public:
    static GraphicalUserInterface* getInstance();
    static void destroy();
    
    void windowSizeChange(int row, int col);
    void setTools(tool t);
    void showAll();
    void clear();

    int write(string &str);
    void recommand(vector<string>& strs);
    void setMiddleLocation(size_t& left, size_t& top);

private:
    GraphicalUserInterface();
    ~GraphicalUserInterface();

private:
    static GraphicalUserInterface* _pInstance;
    vector<winComponentPtr> _com;
    winComponentPtr _inputbox;
    TerminalProcess* _terminal;
    size_t _winRow;
    size_t _winCol;
};

}//end of namespace wk

#endif

