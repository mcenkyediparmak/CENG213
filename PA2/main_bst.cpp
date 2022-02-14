#include <iostream>

#include "BST.h"

int main()
{
    BST<int> tree;
    BST<int> tree1;
    tree.print(inorder);
    tree.print(preorder);
    tree.print(postorder);
    tree.insert(4);
    tree.insert(2);
    tree.insert(6);
    tree.insert(1);
    tree.insert(3);
    tree.insert(5);
    tree.insert(7);
    tree1 = tree;
    tree.remove(4);
    tree1.print(inorder);
    tree1.print(preorder);
    tree.print(postorder);
    std::cout << tree.getSuccessor(tree.getRoot()->right, postorder)->data;

    return 0;
}
