#ifndef __SEARCHBAR_H__
#define __SEARCHBAR_H__
#define MAX_RECOMMAND 10
#include "winComponent.h"
#include "TerminalProcess.h"

namespace wk{


class SearchBar : public winComponent
{
public:
    SearchBar(size_t left, size_t top, TerminalProcess* p) 
    :winComponent(left, top, 1, 22)
    ,_cursorPosition(_content.begin())
    ,_terminal(p)
    ,_recommandStrs()
    ,_wordsLocation(0)
    {}


    void changeLocation(size_t row, size_t col) override{
        _left = col / 2 - _length / 2;
        _top = row / 2 - _width / 2;
        show();
        reset();
        showAllContent(_content);
    }

    void show() override{
        MOVETO(static_cast<int>(_top), static_cast<int>(_left));
        for(size_t i = 0; i < _length; ++i){
            printf("-");
        }
        MOVELEFT(static_cast<int>(_length));
        MOVEDOWN(1);
        printf("|");
        MOVERIGHT(static_cast<int>(_length) - 2);
        printf("|");
        MOVELEFT(static_cast<int>(_length));
        MOVEDOWN(1);
        for(size_t i = 0; i < _length; ++i){
            printf("-");
        }
        showRecommand();
    }

    void showRecommand(){
        int n = _recommandStrs.size();
        if(n){
            for(int i = 1; i <= n; ++i){
                resetRecommand(i);
                CLEAR_LINE();
                _terminal->show(_recommandStrs[i-1]);
            }

            for(int i = n + 1; i <= MAX_RECOMMAND; ++i){
                resetRecommand(i);
                CLEAR_LINE();
            }
        }
    }

    void changeFocus(int preLoc){
        if(_recommandStrs.size()){
            if(preLoc > 0){
                resetRecommand(preLoc);
                CLEAR_LINE();
                _terminal->show(_recommandStrs[preLoc-1]);
            }

            resetRecommand(_wordsLocation);
            CLEAR_LINE();
            SET_BACKGROUND_COLOR(42);
            _terminal->show(_recommandStrs[_wordsLocation-1]);
            RECOVERY();
        }
    }

    void reset(){
        MOVETO(static_cast<int>(_top) + 1, static_cast<int>(_left) + 1);
    }

    void resetRecommand(int loc){
        if(_recommandStrs.size()){
            int n = loc + static_cast<int>(_top) + 2;
            MOVETO(n, static_cast<int>(_left) + 1);
        }
    }

    int write(char c, const string& str) override{
        if(_wordsLocation != 0 && c != 27 && c != 96 && c != 13 && c != 3){
            return 0;
        }
        if(c == 8 || c == 127){
            if(_content.size() == 0){
                return 0;
            }
            bool isChinese = false;//最后一个元素是中文吗
            if(_content.back() >> 7){
                _content.pop_back();
                _content.pop_back();
                _c_num -= 1;
                isChinese = true;
            }
            _content.pop_back();
            if(_c_num <= _length - 2){
                if(isChinese){
                    _terminal->show("\b\b  \b\b");
                }else{
                    _terminal->show("\b \b");
                }
            }else{
                showContent(str);
            }
            _c_num -= 1;
        }else if(c >= 32 && c < 127){//英文字符
            _c_num += 1;
            showContent(str);
        }else if(c == 13){//回车
            if(_wordsLocation == 0){
                if(_content.size() != 0){
                    _content.push_back('\n');
                }
            }else{
                resetRecommand(_wordsLocation);
                CLEAR_LINE();
                _terminal->show(_recommandStrs[_wordsLocation - 1]);
                
                _content = _recommandStrs[_wordsLocation - 1];
                
                calNum();
                SHOW_CURSOR();
                reset();
                string tempStr;
                for(size_t i = 0; i < _length - 2; ++i){
                    tempStr += " ";
                }
                showAllContent(tempStr);
                reset();
                showAllContent(_content);
                _wordsLocation = 0;
            }
        }else if(27 == c){
            if(91 == getchar()){
                int arrowKeys = getchar();
                if(65 == arrowKeys){//向上
                    if(_wordsLocation > 0){
                        _wordsLocation -= 1;
                        if(_wordsLocation == 0){
                            resetRecommand(1);
                            CLEAR_LINE();
                            _terminal->show(_recommandStrs[0]);
                            reset();
                            showAllContent(_content);
                            SHOW_CURSOR();
                        }else{
                            changeFocus(_wordsLocation+1);
                        }
                    }
                }else if(66 == arrowKeys){//向下
                    if(!_content.size()){
                        return 1;
                    }
                    if(_wordsLocation < _recommandStrs.size() && _wordsLocation <= MAX_RECOMMAND){
                        HIDE_CURSOR();
                        _wordsLocation += 1;
                        changeFocus(_wordsLocation-1);
                    }
                }else if(67 == arrowKeys){//向右

                }else if(68 == arrowKeys){//向左

                }
                return 1;
            }
        }else if(c == 96){//esc键
            _wordsLocation = 0;
            showRecommand();
        }else if(c == 3){//退出键 Ctrl+c
            return -1;
        }
        else if(c >> 7){
            _c_num += 2;
            showContent(str);
        }
        return 0;
    }

    int calNum(){
        _c_num = 0;
        for(int i = _content.size() - 1; i > -1; --i){
            if(_content[i] >> 7){
                _c_num += 2;
                i -= 2;
            }else{
                _c_num += 1;
            }
        }
        return _c_num;
    }

    void showContent(const string& str){
        _content += str;
        //如果删除了最后一个字符
        if(_content.size() == 0){
            reset();
            CLEAR_LINE();
            MOVELEFT(1);
            _terminal->show("|");
            _terminal->show("dfs");
            return;
        }
        showAllContent(str);
    }
    void showAllContent(const string &str){
        size_t num = 0;//中英文字符数量
        //如果搜索框装得下
        if(_c_num <= _length - 2){
            _terminal->show(str);
            return;
        }
        int i;
        //如果装不下，就从后往前遍历，找到能装得下的位置
        for(i = _content.size() - 1; i > -1; --i){
            if(_content[i] >> 7){
                num += 2;
                i -= 2;
            }else{
                num += 1;
            }
            if(num >= _length - 2){
                break;
            }
        }
        if(num > _length - 2){
            i += 3;
        }

        reset();
        CLEAR_LINE();
        _terminal->show(_content.substr(i));
        if(num > _length - 2){
            _terminal->show(" ");
            _terminal->show("|");
            MOVELEFT(1);
        }else{
            _terminal->show("|");
        }
        MOVELEFT(1);
    }

    string getContent() override{
        return _content;
    }

    void setContent(const string& str) override{
        _content = str;
    }

    void recommand(vector<string>& strs) override{
        _recommandStrs.assign(strs.begin(), strs.end());
        showRecommand();
        reset();
        showAllContent(_content);
        SHOW_CURSOR();
        _wordsLocation = 0;
    }

    void setFocus(size_t newLoc){
        _wordsLocation = newLoc;
    }


    ~SearchBar(){
        SHOW_CURSOR();
    }

private:
    string _content;
    string::iterator _cursorPosition;
    TerminalProcess* _terminal;
    size_t _c_num;//中英文字符总数，中文字符为2,
    vector<string> _recommandStrs;
    size_t _wordsLocation;//推荐框的焦点单词在什么位置, 0表示在搜索框
};

}//end of namespace wk

#endif

