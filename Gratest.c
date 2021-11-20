#include "client/GraphicalUserInterface.h"


using namespace wk;

int main()
{
    GraphicalUserInterface* g = GraphicalUserInterface::getInstance();
    vector<string> t{"wan", "ke", "12", "3", "group", "abc", "d", "da", " ", "fjdk"};

    
    g->setTools(SEARCHBAR);
    g->showAll();
    bool f = true;
    while(1){
        string temp;
        int n = g->write(temp);
        if(f){
            g->recommand(t);
            f = false;
        }

        if(n == -1){
            break;
        }

    }
    GraphicalUserInterface::destroy();

    return 0;
}

