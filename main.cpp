#include <iostream>
#include "regEngine/RegTree.h"
int main() {
    std::string regExpr = "a**|(c|b)";
    regEngine::RegTree tree(regExpr);
    tree.generateRegTree();
    return 0;
}
