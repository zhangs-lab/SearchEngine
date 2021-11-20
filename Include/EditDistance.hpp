#ifndef __RECO_EDITDISTANCE_HPP__
#define __RECO_EDITDISTANCE_HPP__

#include <stdio.h>
#include <string>
using std::string;

namespace reco
{
//unicode编码表：国际通用
//utf-8编码方案：采用变长编码，编码长度为1~6字节

/* 一个字母用一个字节编码 */
/* 一个汉字用多个字节编码（汉字太多，一个字节不够表示） */

struct EditDistance
{
public:
    //计算每一个字符所占字节数
    size_t nBytesCode(const char ch)
    {
        //ch与1000 0000相与,
        //非零时说明ch二进制的第一位为1
        if(ch&(1<<7))
        {
            int nBytes=1;
            for(int idx=0;idx!=6;++idx)
            {
                if(ch & (1 << (6-idx))){
                    ++nBytes;
                }else{
                    break;
                }
            }
            return nBytes;
        }
        return 1;
    }

    //计算字符串的字符数
    size_t length(const string &str)
    {
        size_t len=0;
        for(size_t idx=0;idx!=str.size();++idx)
        {
            int nBytes=nBytesCode(str[idx]);
            idx += (nBytes-1);//偏移至下一个字符
            ++len;
        }
        /* printf("len = %ld\n",len); */
        return len;
    }

    //返回word的第n个字符
    string getSubWord(const string& word,int n)
    {
        string tmp="";
        for(size_t idx=0;idx!=word.size();)
        {
            --n;
            int nBytes=nBytesCode(word[idx]);
            if(0==n){
                tmp=word.substr(idx,nBytes);
                break;
            }
            idx += nBytes;//偏移至下一个字符
        }
        return tmp;
    }

};

}//end of namespace reco;

#endif

