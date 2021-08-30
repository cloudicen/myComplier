//
// Created by cloudicen on 2021/8/27.
//

#include "RegTree.h"

std::unique_ptr<regEngine::RegNode> &regEngine::RegTree::generateRegTree() {
    root.reset(parseRegExpr());
    return root;
}

void regEngine::RegTree::goNext() {
    if (currentMatchPos + 1 < regExpression.length()) {
        currentMatchPos++;
        currentMatchChar = regExpression.at(currentMatchPos);
    } else {
        currentMatchPos = regExpression.length();
        currentMatchChar = 0;
    }
}

bool regEngine::RegTree::match(char ch) {
    return currentMatchChar == ch;
}

void regEngine::RegTree::matchPair_push(char ch) {
    char target = 0;
    target = signInPair.at(ch);
    matchPairStack.push(target);
}

bool regEngine::RegTree::matchPair_match(char ch) {
    if (matchPairStack.empty()) {
        return false;
    }
    if (matchPairStack.top() == ch) {
        matchPairStack.pop();
        return true;
    } else {
        return false;
    }
}

regEngine::RegNode *regEngine::RegTree::parseRegExpr() {
    printf("parseRegExpr: begin\n");
    if (match('.') || (isprint(currentMatchChar) && !matchRange(currentMatchChar, keptChar))) {
        printf("match: '%c'\n", currentMatchChar);
        auto cur = new regEngine::RegNode(Element, std::string({currentMatchChar}));
        goNext();
        auto f = parseSubExpr(cur);
        if (f != nullptr) {
            return f;
        } else {
            return cur;
        }
    } else if (match('\\')) {
        printf("match: '\\'\n");
        goNext();
        auto cur = parseEscapeChar();
        auto f = parseSubExpr(cur);
        if (f != nullptr) {
            return f;
        } else {
            return cur;
        }
    } else if (match('(')) {
        printf("match: '('\n");
        matchPair_push(currentMatchChar);
        goNext();
        auto cur = parseRegExpr();
        if (matchPair_match(currentMatchChar)) {
            printf("match: ')'\n");
            auto f = parseSubExpr(cur);
            if (f != nullptr) {
                return f;
            } else {
                return cur;
            }
        } else {
            printf("error, missing ')'\n");
            return nullptr;
        }
    } else {
        if(currentMatchPos == regExpression.length() - 1) {
            printf("parseRegExpr: match end\n");
        } else {
            printf("parseRegExpr: match error\n");
        }
        return nullptr;
    }
}

regEngine::RegNode *regEngine::RegTree::parseSubExpr(RegNode *regExprNode) {
    printf("parseSubExpr: begin\n");
    if (match('|')) {
        printf("match '|'\n");
        goNext();
        auto r = parseRegExpr();
        auto cur = new regEngine::RegNode(Alternate, "alternate", std::unique_ptr<regEngine::RegNode>(regExprNode),
                                          std::unique_ptr<regEngine::RegNode>(r));
        auto f = parseSubExpr(cur);
        if (f != nullptr) {
            return f;
        } else {
            return cur;
        }
    } else if (match('*')) {
        printf("match '*'\n");
        goNext();
        auto cur = new regEngine::RegNode(Closure, "closure", std::unique_ptr<regEngine::RegNode>(regExprNode));
        auto f = parseSubExpr(cur);
        if (f != nullptr) {
            return f;
        } else {
            return cur;
        }
    } else {
        auto cur = parseRegExpr();
        if (cur != nullptr) {
            return new regEngine::RegNode(Concat, "concat",
                                          std::unique_ptr<regEngine::RegNode>(regExprNode),
                                          std::unique_ptr<regEngine::RegNode>(cur));
        } else {
            if(currentMatchPos == regExpression.length() - 1) {
                printf("parseSubExpr: match end\n");
            } else {
                printf("parseSubExpr: match error\n");
            }
            return nullptr;
        }
    }
}

regEngine::RegNode *regEngine::RegTree::parseEscapeChar() {
    printf("parseEscapeChar: begin\n");
    if (matchRange(currentMatchChar, escapeChar)) {
        printf("match '%c'\n", currentMatchChar);
        auto info = escapeCharDef.find(currentMatchChar)->second;
        goNext();
        return new regEngine::RegNode(Element, info);
    } else {
        printf("parseEscapeChar: match error\n");
        return nullptr;
    }
}

bool regEngine::RegTree::matchRange(char ch, const std::unordered_set<char> &charRange) {
    return charRange.count(ch) > 0;
}

