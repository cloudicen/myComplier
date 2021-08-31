#include "regEngine/RegTree.h"
#include "logger/LoggerFactory.h"
int main() {
    std::string regExpr = "a**|(c|b)";
    regEngine::RegTree tree(regExpr);
    tree.generateRegTree();
    return 0;
}
