//
// Created by cloudicen on 2021/8/27.
//

#ifndef MY_COMPILER_REGTREE_H
#define MY_COMPILER_REGTREE_H

#include "RegEngineDef.h"
#include "RegNode.h"
#include "logger/LoggerFactory.h"
#include <cassert>
#include <cctype>
#include <functional>
#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace regEngine {

    class RegTree {
    private:
        std::shared_ptr<logger> LOGGER = LoggerFactory::getLogger("regTree");
        /* -------------------------需要转换的表达式----------------------------- */
        std::string regExpression = "";
        /* ------------------------------------------------------------------- */

        std::unique_ptr<RegNode> root = nullptr;

    public:
        explicit RegTree(const std::string &_regExpression)
                : regExpression(_regExpression), currentMatchChar(_regExpression.at(0)) {};

        RegTree(const RegTree &other) = delete;

        const RegNode* generateRegTree();

    private:
        int currentMatchPos = 0;
        char currentMatchChar = 0;
        std::stack<char> matchPairStack;

    private:
        void goNext();

        bool match(char ch);

        void matchPair_push(char ch);

        bool matchPair_match(char ch);

        /*
         * 这里直接将语义操作（生成语法树结构）嵌入递归下降子程序。
         * 用了三个函数，对应三个非终结符，但函数的形式并不相同。
         * 这与先前对文法的定义有关。其中regExpr、escapeChar的开头都是基本元素节点，都是作为叶子节点，子节点为空（这里将(regExpr)整体是为一个节点，道理是一样的）。
         * 而subExpr的开头是‘|’或‘*’,其拥有子节点。
         * 为了在递归过程中正确构建节点间的树形关系，parseSubExpr(RegNode *
         * regExprNode)添加了额外的参数，用于传入其子节点;
         */

        RegNode *parseRegExpr();

        RegNode *parseSubExpr(RegNode *regExprNode);

        RegNode *parseEscapeChar();
    };
} // namespace regEngine

#endif // MY_COMPILER_REGTREE_H
