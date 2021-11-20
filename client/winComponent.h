#ifndef __WINCOMPONENT_H__
#define __WINCOMPONENT_H__
#include "ctrlcur.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace wk{

class winComponent
{
public:
    winComponent(size_t left, size_t top, size_t width, size_t length)
    :_left(left)
    ,_top(top)
    ,_width(width)
    ,_length(length)
    {}

    virtual ~winComponent(){}

    size_t getLeft(){
        return _left;
    }

    void setLeft(size_t left){
        _left = left;
    }

    size_t getTop(){
        return _top;
    }

    void setTop(size_t top){
        _top = top;
    }

    size_t getWidth(){
        return _width;
    }

    void setWidth(size_t width){
        _width = width;
    }

    size_t getLength(){
        return _length;
    }

    void setLength(size_t length){
        _length = length;
    }
    virtual void show() = 0;
    virtual string getContent() = 0;
    virtual void setContent(const string& str) = 0;
    virtual int write(const char c, const string& str){

    }
    virtual void recommand(vector<string>& str){

    }
    virtual void changeLocation(size_t left, size_t top){

    }
    
protected:
    size_t _left;
    size_t _top;
    size_t _width;
    size_t _length;
};

}//end of namespace wk

#endif

