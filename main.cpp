#include "regEngine/RegTree.h"
#include "logger/LoggerFactory.h"
int main() {
    std::string regExpr = "a**|(c|b)";
    regEngine::RegTree tree(regExpr);
    tree.generateRegTree();
    auto logger = LoggerFactory::getLogger("main","sub1.sub2");
    logger->debug("debug msg");
    return 0;
}
