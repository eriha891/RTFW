//  Tokenizer.h
//
//  A lightweight tokenizer for strings.
//  Internally it uses two different versions,
//  one to recognize blank space ' ' and one generic
//  to recognize characters placed in a string, ex. " ,."
//
//  Author: Robin Skånberg

#pragma once
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <iostream>
#include <string>

class Tokenizer
{
private:
    std::vector<std::string> tokens;
    const std::string defstr;
    unsigned int counter;

public:
    Tokenizer() {}
    Tokenizer(const std::string &str)
    {
        tokenizeFast(str);
    }

    Tokenizer(const std::string &str, const std::string &chars)
    {
        tokenize(str, chars);
    }

    // Fast version that only looks for space ' '
    void tokenizeFast(const std::string &str)
    {
        tokens.clear();

        size_t begin = 0;

        for(size_t i=0; i<str.length(); ++i)
        {
            if(str[i] == ' ')
            {
                if(begin != i)
                {
                    tokens.push_back(str.substr(begin,i-begin));
                    begin = i+1;
                }
                else
                    ++begin;
            }
        }
        if(begin != str.length())
            tokens.push_back(str.substr(begin));

        counter = 0;
    }

    // Generic version that checks for any characters in the string chars.
    void tokenize(const std::string &str, const std::string &chars = " ")
    {
        tokens.clear();

        size_t begin = 0;
        size_t u;

        for(size_t i=0; i<str.length(); ++i)
        {
            for(u=0; u<chars.length(); ++u)
            {
                if(str[i]==chars[u])
                {
                    if(begin != i)
                    {
                        tokens.push_back(str.substr(begin,i-begin));
                        begin = i+1;
                    }
                    else
                        ++begin;
                }
            }
        }
        if(begin != str.length())
            tokens.push_back(str.substr(begin));

        counter = 0;
    }

    const std::string &getNext()
    {
        if(counter < tokens.size())
        {
            return tokens[counter++];
        }
        else
            return defstr;
    }

    void reset() { counter=0; }
    bool end() { return counter==tokens.size(); }
    unsigned int size() { return tokens.size(); }
};

#endif
