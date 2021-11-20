#ifndef __WEBPAGETOOL_H__
#define __WEBPAGETOOL_H__
#include "winComponent.h"
namespace wk{

class WebPageTool : public winComponent
{
public:
    WebPageTool(size_t width, size_t length)
    :winComponent(0, 0, width, length)
    {}
    ~WebPageTool() {}

    void show() override{

    }

    int write(const char c, const string& str) override{

    }

    string getContent() override{
        return _content;
    }

    void setContent(const string& str) override{
        _content = str;
    }

private:
    string _content;

};

}//end of namespace wk

#endif

