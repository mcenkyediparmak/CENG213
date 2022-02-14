#ifndef BST_H
#define BST_H

#include <iostream>

#include "BSTNode.h"

enum TraversalPlan
{
    preorder,
    inorder,
    postorder
};

template <class T>
class BST
{
public: // DO NOT CHANGE THIS PART.
    BST();
    BST(const BST<T> &obj);

    ~BST();

    BSTNode<T> *getRoot() const;
    bool isEmpty() const;
    bool contains(BSTNode<T> *node) const;

    void insert(const T &data);

    void remove(const T &data);
    void removeAllNodes();

    BSTNode<T> *search(const T &data) const;
    BSTNode<T> *getSuccessor(BSTNode<T> *node, TraversalPlan tp) const;

    void print(TraversalPlan tp = inorder) const;

    BST<T> &operator=(const BST<T> &rhs);

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void print(BSTNode<T> *node, TraversalPlan tp) const;
    void copyConstructor(BSTNode<T> *root, BSTNode<T> *base);
    BSTNode<T> *destructor(BSTNode<T> *root);
    bool contains(BSTNode<T> *root, BSTNode<T> *node) const;
    BSTNode<T> *insert(BSTNode<T> *root, const T &data);
    BSTNode<T> *search(BSTNode<T> *root, const T &data) const;
    BSTNode<T> *remove(BSTNode<T> *root, const T &data);
    BSTNode<T> *findMin(BSTNode<T> *root) const;
    BSTNode<T> *getSuccessor(BSTNode<T> *root, BSTNode<T> *node, BSTNode<T> *successor, TraversalPlan tp) const;

private: // DO NOT CHANGE THIS PART.
    BSTNode<T> *root;
};

#endif // BST_H

template <class T>
BST<T>::BST()
{
    root = NULL;
}

template <class T>
void BST<T>::copyConstructor(BSTNode<T> *root, BSTNode<T> *base)
{
    if (base == NULL)
    {
        root = NULL;
    }
    else
    {

        this->insert(base->data);
        if (base->left)
        {
            copyConstructor(root, base->left);
        }
        if (base->right)
        {
            copyConstructor(root, base->right);
        }
    }
}

template <class T>
BST<T>::BST(const BST<T> &obj)
{
    destructor(root);
    copyConstructor(root, obj.getRoot());
}
template <class T>
BSTNode<T> *BST<T>::destructor(BSTNode<T> *node)
{
    if (node == NULL)
    {
        return NULL;
    }
    destructor(node->left);
    destructor(node->right);
    delete node;

    return NULL;
}
template <class T>
BST<T>::~BST()
{
    root = destructor(root);
}

template <class T>
BSTNode<T> *BST<T>::getRoot() const
{
    if (root)
    {
        return root;
    }
    return NULL;
}

template <class T>
bool BST<T>::isEmpty() const
{
    if (root)
    {
        return false;
    }
    return true;
}

template <class T>
bool BST<T>::contains(BSTNode<T> *node) const
{
    if (root)
        return contains(this->getRoot(), node);
    return false;
}
template <class T>
bool BST<T>::contains(BSTNode<T> *root, BSTNode<T> *node) const
{
    if (root == node)
        return true;
    else if (root == NULL)
        return false;
    else
        return (this->contains(root->left, node) || this->contains(root->right, node));
}

template <class T>
void BST<T>::insert(const T &data)
{
    root = insert(root, data);
}
template <class T>
BSTNode<T> *BST<T>::insert(BSTNode<T> *root, const T &x)
{
    if (root == NULL)
    {
        root = new BSTNode<T>(x, NULL, NULL);
    }
    else if (x < root->data)
        root->left = insert(root->left, x);
    else if (x > root->data)
        root->right = insert(root->right, x);
    return root;
}
template <class T>
BSTNode<T> *BST<T>::findMin(BSTNode<T> *root) const
{
    if (root == NULL)
    {
        return NULL;
    }
    else if (root->left == NULL)
    {
        return root;
    }
    else
    {
        return findMin(root->left);
    }
}
template <class T>
void BST<T>::remove(const T &data)
{
    root = remove(root, data);
}

template <class T>
BSTNode<T> *BST<T>::remove(BSTNode<T> *root, const T &data)
{
    BSTNode<T> *temp;
    if (root == NULL)
    {
        return NULL;
    }
    else if (data < root->data)
    {
        root->left = remove(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = remove(root->right, data);
    }
    else
    {
        if (root->left == NULL && root->right == NULL)
        {
            delete root;
            root = NULL;
        }
        else if (root->left && root->right)
        {
            temp = findMin(root->right);
            root->data = temp->data;
            root->right = remove(root->right, temp->data);
        }
        else
        {
            if (root->left)
            {
                temp = root;
                root = root->left;
                delete temp;
            }
            else
            {
                temp = root;
                root = root->right;
                delete temp;
            }
        }
    }
    return root;
}

template <class T>
void BST<T>::removeAllNodes()
{
    root = destructor(root);
}
template <class T>
BSTNode<T> *BST<T>::search(BSTNode<T> *node, const T &data) const
{
    if (node == NULL)
    {
        return NULL;
    }
    else if (data < node->data)
    {
        return search(node->left, data);
    }
    else if (data > node->data)
    {
        return search(node->right, data);
    }
    else if (data == node->data)
    {
        return node;
    }
}

template <class T>
BSTNode<T> *BST<T>::search(const T &data) const
{
    return search(root, data);
}

template <class T>
BSTNode<T> *BST<T>::getSuccessor(BSTNode<T> *node, TraversalPlan tp) const
{
    if (root)
    {
        if (tp == inorder)
        {
            return getSuccessor(root, node, NULL, tp);
        }
        else if (tp == preorder)
        {
            return getSuccessor(root, node, NULL, tp);
        }
        else if (tp == postorder)
        {
            return getSuccessor(root, node, NULL, tp);
        }

        return NULL;
    }
    else
        return NULL;
}
template <class T>
BSTNode<T> *BST<T>::getSuccessor(BSTNode<T> *root, BSTNode<T> *node, BSTNode<T> *successor, TraversalPlan tp) const
{

    if (tp == inorder)
    {
        if (node->right)
        {
            return findMin(node->right);
        }
        else
        {
            if (node->data < root->data)
            {
                return getSuccessor(root->left, node, root, tp);
            }
            else if (node->data > root->data)
            {
                return getSuccessor(root->right, node, successor, tp);
            }
        }
        return successor;
    }
    else if (tp == preorder)
    {
        if (node->left)
        {
            return node->left;
        }
        else if (node->right)
        {
            return node->right;
        }
        else
        {
            if (node->data < root->data)
            {
                if (root->right)
                {
                    successor = root->right;
                }
                return getSuccessor(root->left, node, successor, tp);
            }
            else if (node->data > root->data)
            {
                return getSuccessor(root->right, node, successor, tp);
            }
        }
        return successor;
    }
    else if (tp == postorder)
    {
        if (node == root->left)
        {
            if (root->right)
            {
                successor = findMin(root->right);
                while (successor->right)
                {
                    successor = findMin(successor->right);
                }
                return successor;
            }
            else
                return root;
        }
        if (node == root->right)
        {
            return root;
        }
        else
        {
            if (node->data < root->data)
            {
                successor = root;
                return getSuccessor(root->left, node, successor, tp);
            }
            else if (node->data > root->data)
            {
                return getSuccessor(root->right, node, successor, tp);
            }
        }
        return successor;
    }
    return successor;
}

template <class T>
void BST<T>::print(TraversalPlan tp) const
{

    if (tp == inorder)
    {
        // check if the tree is empty?
        if (isEmpty())
        {
            // the tree is empty.
            std::cout << "BST_inorder{}" << std::endl;
            return;
        }

        // the tree is not empty.

        // recursively output the tree.
        std::cout << "BST_inorder{" << std::endl;
        print(root, inorder);
        std::cout << std::endl
                  << "}" << std::endl;
    }
    else if (tp == preorder)
    {
        if (isEmpty())
        {
            // the tree is empty.
            std::cout << "BST_preorder{}" << std::endl;
            return;
        }

        // the tree is not empty.

        // recursively output the tree.
        std::cout << "BST_preorder{" << std::endl;
        print(root, preorder);
        std::cout << std::endl
                  << "}" << std::endl;
    }
    else if (tp == postorder)
    {
        if (isEmpty())
        {
            // the tree is empty.
            std::cout << "BST_postorder{}" << std::endl;
            return;
        }

        // the tree is not empty.

        // recursively output the tree.
        std::cout << "BST_postorder{" << std::endl;
        print(root, postorder);
        std::cout << std::endl
                  << "}" << std::endl;
    }
}

template <class T>
BST<T> &BST<T>::operator=(const BST<T> &rhs)
{
    destructor(root);
    copyConstructor(root, rhs.getRoot());
    return *this;
}

template <class T>
void BST<T>::print(BSTNode<T> *node, TraversalPlan tp) const
{

    // check if the node is NULL?
    if (node == NULL)
        return;

    if (tp == inorder)
    {
        print(node->left, inorder);
        if (node->left)
        {
            std::cout << "," << std::endl;
        }

        std::cout << "\t" << node->data;

        if (node->right)
        {
            std::cout << "," << std::endl;
        }
        print(node->right, inorder);
    }
    else if (tp == preorder)
    {
        std::cout << "\t" << node->data;

        if (node->left)
        {
            std::cout << "," << std::endl;
        }
        print(node->left, preorder);
        if (node->right)
        {
            std::cout << "," << std::endl;
        }
        print(node->right, preorder);
    }
    else if (tp == postorder)
    {

        print(node->left, postorder);
        if (node->left)
        {
            std::cout << "," << std::endl;
        }
        print(node->right, postorder);
        if (node->right)
        {
            std::cout << "," << std::endl;
        }

        std::cout << "\t" << node->data;
    }
}
