#ifndef __RECO_MYCPPJIEBA_HPP__
#define __RECO_MYCPPJIEBA_HPP__
#include "../cppjieba/Jieba.hpp"

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
/* using std::cout; */
/* using std::endl; */

namespace reco
{
//对于MIX模式，要同时包含MP，HMM
const char * const DICT_PATH = "/home/zhangs/cppjieba/dict/jieba.dict.utf8";//最大概率法MP
const char * const HMM_PATH = "/home/zhangs/cppjieba/dict/hmm_model.utf8";//隐式马尔科夫模型HMM,
const char * const USER_DICT_PATH = "/home/zhangs/cppjieba/dict/user.dict.utf8";//用户自定义词典路径
const char * const IDF_PATH = "/home/zhangs/cppjieba/dict/idf.utf8";//IDF路径
const char * const STOP_WORD_PATH = "/home/zhangs/cppjieba/dict/stop_words.utf8";//停用词路径


class MyCppjieba
{
public:
    MyCppjieba()
    :_jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH)
    {
        /* cout<<"cppjieba init"<<endl; */
    }
    vector<string> operator()(const string str)
    {
        vector<string> words;
        _jieba.Cut(str,words,true);//cut with HMM
        /* _jieba.Cut(str,words,false);//cut without HMM */
        /* _jieba.CutAll(str,words);//全模式 */
        /* _jieba.CutForSearch(str,words);//搜索引擎模式 */
        return words;
    }

private:
    cppjieba::Jieba _jieba;
};

}//end of namespace reco

#endif

