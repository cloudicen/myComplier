#ifndef TREESTRUCTPRINTER_H
#define TREESTRUCTPRINTER_H

template<typename treeNode,int branchCount=2,int maxLevel=64>
class treeStructPrinter
{
private:
    struct backLog
    {
        const treeNode * node=nullptr;
        int nextIndex=0;
        backLog(treeNode*_node=nullptr,int _nextIndex=0):node(_node),nextIndex(_nextIndex){};
    };

    static inline void nbl_push(backLog *nbl, backLog **top, backLog **bottom)
    {
        if (*top - *bottom < maxLevel)
        {
            (*(*top)++) = *nbl;
        }
    }

    static inline backLog *nbl_pop(backLog **top, backLog **bottom)
    {
        return *top > *bottom ? --*top : nullptr;
    }

    static inline bool intnbl_is_empty(backLog *top, backLog *bottom)
    {
        return top == bottom;
    }

public:
    static void printTree(const treeNode * root)
    {
        int level=0;
        const treeNode *node=root;
        backLog nbl;
        backLog *p_nbl = nullptr;
        backLog *top, *bottom, nblStack[maxLevel];
        top = bottom = nblStack;
        while (true)
        {
            if (node != nullptr) {
                //以下两句非常巧妙实现，回到回溯点时不打印回溯点，打印左节点，该过程循环两遍，
                //第一遍不打印，第二遍节点存在，且p_nbl已为空，此时sub_index为RightIndex，会打印
                int sub_index = p_nbl != nullptr ? p_nbl->nextIndex : branchCount-1;
                p_nbl = nullptr;

                //记录回溯点，先保存左再保存右，当前sub_index为RightIndex打印
                if (node->isLeaf() || sub_index == 0) {
                    nbl.node = nullptr;
                    nbl.nextIndex = branchCount-1;
                } else {
                    nbl.node = node;
                    nbl.nextIndex = 0;
                }
                nbl_push(&nbl, &top, &bottom);
                level++;
                if (sub_index == branchCount-1) {
                    for (int i = 1; i < level; ++i) {
                        if (i == level - 1) {
                            printf("%-12s", "+-----------");
                        } else {
                            if (nblStack[i - 1].node != nullptr) {
                                printf("%-17s", "|");
                            } else {
                                printf("%-17s", " ");
                            }
                        }
                    }
                    printf("%s\n",node->toPrintable().toStdString().c_str());
                }
                node = node->children[sub_index];
            } else {
                p_nbl = nbl_pop(&top, &bottom);
                if (p_nbl == nullptr) {
                    break;
                }
                node = p_nbl->node;
                level--;
            }
        }
    }
};

#endif // TREESTRUCTPRINTER_H
