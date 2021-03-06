﻿#include "reg_tree.h"

QSet<QChar> regTree::keptChar = {'|','.','\\','*','(',')'};// 保留符号

QMap<QChar,QString> regTree::escapeChar = {
        {'w',"word"}, // 转义匹配 数字+字母+下划线
        {'a',"alpha"},// 转义匹配 字母
        {'d',"digit"},// 转义匹配 数字
        {'s',"space"},// 转义匹配 空白字符
        {'n',"newline"},// 转义匹配 换行符
        {'t',"tab"},// 转义匹配 制表符
        {'r',"return"},// 转义匹配 回车
        {'e',"empty"},// 转义匹配空串，辅助实现‘？’算符的零次或一次匹配

        {'\\',"\\"},// 转义匹配保留符号
        {'|',"|"},
        {'.',"."},
        {'*',"*"},
        {'(',"("},
        {')',")"}
    };

void regTree::getNextChar()
{
    if(pos+1 < regExpr.length())
    {
        pos++;
        currentChar = regExpr.at(pos);
    }
    else
    {
        pos = regExpr.length();
        currentChar = 0;
    }
    qDebug() << "getNext->current:" << currentChar;
}

void regTree::putCharback()
{
    if(pos-1 >= 0)
    {
        pos--;
        currentChar = regExpr.at(pos);
    }
    else
    {
        pos = -1;
        currentChar = 0;
    }
    qDebug() << "putback->current:" << currentChar;
}

bool regTree::match(QChar ch)
{
    bool ok = ch == currentChar;
    if(ok)
    {
        qDebug() << "match char" << currentChar;
    }
    return ok;
}

bool regTree::matchSign(QChar ch)
{
    if(!signStack.isEmpty() && signStack.back() == ch)
    {
        qDebug() << "matchSign" << ch;
        signStack.pop_back();
        return true;
    }
    else
    {
        return false;
    }
}

QSharedPointer<regNode> regTree::parse_regExpr()
{
    qDebug() << "parse reg expression";
    getNextChar();
    if(match('\\'))
    {
        auto t = parse_EscapeChar();
        auto p = parse_subExpr();
        auto cur = p;
        if(cur != nullptr)
        {
            while(cur->children[0] != nullptr)
            {
                cur = cur->children[0];
            }
            cur->children[0] = t;
            return p;
        }
        return t;
    }
    else if(match('('))
    {
        int current = pos+1;
        signStack.push_back(')');
        auto t = parse_regExpr();
        getNextChar();
        if(!matchSign(currentChar))
        {
            //error
            qDebug() << "missing ) for '(' at pos " << current;
            putCharback();
        }
        auto p = parse_subExpr();
        auto cur = p;
        if(cur != nullptr)
        {
            while(cur->children[0] != nullptr)
            {
                cur = cur->children[0];
            }
            cur->children[0] = t;
            return p;
        }
        return t;
    }
    else if(currentChar.isPrint() && !keptChar.contains(currentChar))
    {
        qDebug() << "match char" << currentChar;
        auto t = QSharedPointer<regNode>::create(Element,currentChar);
        nodes.push_back(t);
        auto p = parse_subExpr();
        auto cur = p;
        if(cur != nullptr)
        {
            while(cur->children[0] != nullptr)
            {
                cur = cur->children[0];
            }
            cur->children[0] = t;
            return p;
        }
        return t;
    }
    else
    {
        putCharback();
        return nullptr;
    }
}

QSharedPointer<regNode> regTree::parse_subExpr()
{
    qDebug() << "parse sub expression";
    getNextChar();
    if(match('|'))
    {
        auto p = parse_regExpr();
        auto t = parse_subExpr();
        auto a = QSharedPointer<regNode>::create(Alternate,"|",nullptr,p);
        nodes.push_back(a);
        if(t != nullptr)
        {
            t->children[0] = a;
            return t;
        }
        return a;
    }
    else if(match('.'))
    {
        auto p = parse_regExpr();
        auto t = parse_subExpr();
        auto a = QSharedPointer<regNode>::create(Concat,".",nullptr,p);
        nodes.push_back(a);
        if(t != nullptr)
        {
            t->children[0] = a;
        }
        return a;
    }
    else if(match('*'))
    {
        auto t = parse_subExpr();
        auto p = QSharedPointer<regNode>::create(Closure,"*");
        nodes.push_back(p);
        if(t != nullptr)
        {
            t->children[0] = p;
            return t;
        }
        return p;
    }
    else
    {
        putCharback();
        return nullptr;
    }
}

QSharedPointer<regNode> regTree::parse_EscapeChar()
{
    qDebug() << "parse escape char";
    getNextChar();
    auto info = escapeChar.find(currentChar);
    if(info != escapeChar.end())
    {
        qDebug() << "match escape char" << currentChar;
        auto newNode = QSharedPointer<regNode>::create(Element,*info);
        nodes.push_back(newNode);
        return newNode;
    }
    else
    {
        //error
        qDebug() << "invalid escape char error.";
        putCharback();
        abort();
        return nullptr;
    }
}

bool regTree::analyze()
{
    root = parse_regExpr();
    if(pos != regExpr.length()-1 || root.isNull())
    {
        qDebug() << "syntax error, expected statment after position:" << pos;
        return false;
    }
    analyzed = true;
    return true;
}


