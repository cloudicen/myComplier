目前的实现中，只支持：【选择、连接、闭包】运算，由于每个字符都是一个词法单元，故直接构造一个语法制导翻译器，其输出是一个正则语法树

- 终结符号列表：
    *     '|' -> 选择
    *     '*' -> 闭包
    *     '(' -> 左括号
    *     ')' -> 右括号
    *     '\' -> 反斜杠，用于转义控制字符
    *     '.' -> 代表除换行符 \n 之外的任何单字符
    *     char -> 任意除保留字以外的可打印字符
- 非终结符列表：
    *     regExpr -> 正则表达式（开始符号）
    *     EscapeChar -> 转义字符
    
- 构造产生式集合（优先级从上到下递增）：
    * regExpr   -> regExpr | regExpr —>选择
                -> regExpr regExpr ->连接
                -> regExpr *  ->闭包
                -> ( regExpr ) ->括号影响优先级
                -> \ EscapeChar ->转义字符
                -> . 
                -> char 
    * EscapeChar    -> |
                    -> .
                    -> \
                    -> *
                    -> (
                    -> )
                    -> r (回车符)
                    -> t (换行符)
                    -> n (换行符)
                    -> s (空白字符）
                    -> w (digit|alpha|_)
                    -> d (digit)
                    -> a (alpha)
-  消除左递归，提取左因子，以便自顶向下分析
    *     regExpr -> (regExpr) subExpr
    *             -> \EscapeChar subExpr
    *             -> char subExpr
    *             -> . subExpr
    *     subExpr -> |regExpr subExpr
    *             -> *subExpr
    *             -> regExpr
    *             -> empty
---
-  最终的非终结符列表
    *     regExpr -> 正则表达式（开始符号）
    *     subExpr -> 子表达式
    *     EscapeChar -> 转义字符