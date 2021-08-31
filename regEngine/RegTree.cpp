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

bool regEngine::RegTree::match(char ch) { return currentMatchChar == ch; }

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
  LOGGER->debug("parseRegExpr: begin");
  if (match('.') ||
      (isprint(currentMatchChar) && !matchRange(currentMatchChar, keptChar))) {
    LOGGER->debug("match: '" + std::string(&currentMatchChar) + "'");
    auto cur = new regEngine::RegNode(Element, std::string({currentMatchChar}));
    goNext();
    auto f = parseSubExpr(cur);
    if (f != nullptr) {
      return f;
    } else {
      return cur;
    }
  } else if (match('\\')) {
    LOGGER->debug("match: '\\'");
    goNext();
    auto cur = parseEscapeChar();
    auto f = parseSubExpr(cur);
    if (f != nullptr) {
      return f;
    } else {
      return cur;
    }
  } else if (match('(')) {
    LOGGER->debug("match: '('");
    matchPair_push(currentMatchChar);
    goNext();
    auto cur = parseRegExpr();
    if (matchPair_match(currentMatchChar)) {
      LOGGER->debug("match: ')'");
      auto f = parseSubExpr(cur);
      if (f != nullptr) {
        return f;
      } else {
        return cur;
      }
    } else {
      LOGGER->error("reg syntax error, missing ')'");
      return nullptr;
    }
  } else {
    if (currentMatchPos == regExpression.length() - 1) {
      LOGGER->debug("parseRegExpr: match end");
    } else {
      LOGGER->error("parseRegExpr: match error");
    }
    return nullptr;
  }
}

regEngine::RegNode *regEngine::RegTree::parseSubExpr(RegNode *regExprNode) {
  LOGGER->debug("parseSubExpr: begin");
  if (match('|')) {
    LOGGER->debug("match '|'");
    goNext();
    auto r = parseRegExpr();
    auto cur =
        new regEngine::RegNode(Alternate, "alternate",
                               std::unique_ptr<regEngine::RegNode>(regExprNode),
                               std::unique_ptr<regEngine::RegNode>(r));
    auto f = parseSubExpr(cur);
    if (f != nullptr) {
      return f;
    } else {
      return cur;
    }
  } else if (match('*')) {
    LOGGER->debug("match '*'");
    goNext();
    auto cur = new regEngine::RegNode(
        Closure, "closure", std::unique_ptr<regEngine::RegNode>(regExprNode));
    auto f = parseSubExpr(cur);
    if (f != nullptr) {
      return f;
    } else {
      return cur;
    }
  } else {
    auto cur = parseRegExpr();
    if (cur != nullptr) {
      return new regEngine::RegNode(
          Concat, "concat", std::unique_ptr<regEngine::RegNode>(regExprNode),
          std::unique_ptr<regEngine::RegNode>(cur));
    } else {
      if (currentMatchPos == regExpression.length() - 1) {
        LOGGER->debug("parseSubExpr: match end");
      } else {
        LOGGER->error("parseSubExpr: match error");
      }
      return nullptr;
    }
  }
}

regEngine::RegNode *regEngine::RegTree::parseEscapeChar() {
  LOGGER->debug("parseEscapeChar: begin\n");
  if (matchRange(currentMatchChar, escapeChar)) {
    LOGGER->debug("match '" + std::string(&currentMatchChar) + "'");
    auto info = escapeCharDef.find(currentMatchChar)->second;
    goNext();
    return new regEngine::RegNode(Element, info);
  } else {
    LOGGER->error("parseEscapeChar: match error\n");
    return nullptr;
  }
}

bool regEngine::RegTree::matchRange(char ch,
                                    const std::unordered_set<char> &charRange) {
  return charRange.count(ch) > 0;
}
