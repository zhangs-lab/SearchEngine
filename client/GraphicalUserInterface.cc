#include "GraphicalUserInterface.h"
#include "winComponent.h"

#include <string.h>
#include <unistd.h>

namespace wk{
GraphicalUserInterface* GraphicalUserInterface::getInstance(){
    if(!_pInstance){
        _pInstance = new GraphicalUserInterface();
    }
    return _pInstance;
}

void GraphicalUserInterface::destroy(){
    if(_pInstance){
        delete _pInstance;
    }
    _pInstance = nullptr;
}

GraphicalUserInterface* GraphicalUserInterface::_pInstance = nullptr;

void sigFun(int sig, siginfo_t *p, void *p1){
    GraphicalUserInterface* g = GraphicalUserInterface::getInstance();
    struct winsize size;
    ioctl(0, TIOCGWINSZ, &size);
    g->windowSizeChange(size.ws_row, size.ws_col);
    fflush(stdout);
}

GraphicalUserInterface::GraphicalUserInterface()
:_terminal(TerminalProcess::getInstance())
{
    struct winsize size;
    ioctl(0, TIOCGWINSZ, &size);
    _terminal->setOriginalMode();
    _winRow = size.ws_row;
    _winCol = size.ws_col;
    struct sigaction act;
    ::memset(&act, 0, sizeof(act));
    act.sa_flags = SA_SIGINFO | SA_RESTART;
    act.sa_sigaction = sigFun;
    sigaction(SIGWINCH, &act, nullptr);
}

GraphicalUserInterface::~GraphicalUserInterface(){
    _terminal->destroy();
}

void GraphicalUserInterface::windowSizeChange(int row, int col){
    _winRow = row;
    _winCol = col;
    clear();
    for(auto &ele : _com){
        ele->changeLocation(row, col);
    }
}


void GraphicalUserInterface::showAll(){
    clear();
    for(auto &ele : _com){
        ele->show();
    }
    if(_inputbox){
        MOVETO(static_cast<int>(_inputbox->getTop() + 1), 
               static_cast<int>(_inputbox->getLeft() + 1));
    }
}

void GraphicalUserInterface::setMiddleLocation(size_t& left, size_t& top){

}

void GraphicalUserInterface::setTools(tool t){
    winComponentPtr com;
   
    switch(t){
    case SEARCHBAR:
        com = std::make_shared<SearchBar>(0, 0, _terminal);
        com->changeLocation(_winRow, _winCol);
        _inputbox = com;
        break;
    case CANDIDATEBOX:
        com = std::make_shared<candidateBox>(0, 0);
        break;
    }
    _com.push_back(com);
}

void GraphicalUserInterface::clear(){
    /* CLEAR(); */
    system("clear");
}

int GraphicalUserInterface::write(string &str){
    string temp;
    int n = 0;
    if(_inputbox){
        char c;
        /* _terminal->getAll(); */
        /* while(!_terminal->isEmpty()){ */
        /*     c = _terminal->getCharacter1(temp); */
        /*     n = _inputbox->write(c, temp); */
        /* } */
        c = _terminal->getCharacter(temp);
        n = _inputbox->write(c, temp);

        if(n == -1){
            clear();
        }
        str = _inputbox->getContent();
        
        /* if(c == 13){ */
        /*     res.push_back('\n'); */
        /* }else if(c == 127 || c == 8){ */
        /*     /1* _terminal.show(std::to_string(res.size())); *1/ */
        /*     if(res.size() == 0){ */
        /*         return res; */
        /*     } */
            
        /* }else if(27 == c){ */
        /*     if(91 == getchar()){ */
        /*         int arrowKeys = getchar(); */
        /*         if(65 == arrowKeys){//向上 */

        /*         }else if(66 == arrowKeys){//向下 */

        /*         }else if(67 == arrowKeys){//向右 */

        /*         }else if(68 == arrowKeys){//向左 */

        /*         } */
        /*     } */

        /* }else{ */
        /*     _terminal.show(temp); */
        /*     res += temp; */
        /* } */
        /* _inputbox->setContent(res); */
    }
    return n;
}


void GraphicalUserInterface::recommand(vector<string>& strs){
    _inputbox->recommand(strs);
}


}//end of namespace wk
