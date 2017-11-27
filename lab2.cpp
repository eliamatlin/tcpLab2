#include "lab2.h"

BinaryTree::BinaryTree()
{

}

Node * BinaryTree::delete_tree_r(struct Node ** tree)
{
    if (*tree != NULL)
    {
        struct Node *tmp = *tree;
        tmp->left = delete_tree_r(&((*tree)->left));
        tmp->right = delete_tree_r(&((*tree)->right));
        free(tmp); *tree = NULL; return (tmp);
    }
    return nullptr;
}




Node * BinaryTree::search_insert_r(struct Node ** root, const int d)
{
    struct Node * pnew = NULL;
    if (!(*root))
    {
        pnew = (struct Node *)malloc(sizeof(struct Node));
        pnew->d = d;
        pnew->left = pnew->right = NULL;
        *root = pnew;
    }
    else
    {
        if ((*root)->d == d) {
            QMessageBox::information(0, "Внимание!", "Данный элемент уже находится в дереве.");
            return ((*root));
        }
        if (d < (*root)->d) search_insert_r(&((*root)->left), d);
        if (d >(*root)->d) search_insert_r(&((*root)->right), d);
    }
    return (pnew);
}

void BinaryTree::print_tree(const struct Node * root, std::string st)
{

    std::string str = st;
    if (root)
    {
        results[str] = root->d;
        str.append("L");
        print_tree(root->left, str);
        str.erase(str.end() - 1);
        str.append("R");
        print_tree(root->right, str);
    }
    else if(str.length()) str.erase(str.end() - 1);
}


bool BinaryTree::delete_node(struct Node ** root, const int d)
{
    int found = 0;
    struct Node * pv = *root, *prev = NULL;
    l_r way = no_move;

    while (pv && !found)
    {
        if (d == pv->d) { found = 1; }
        else if (d < pv->d)
        {
            prev = pv;
            pv = pv->left;
            way = mLeft;
        }
        else
        {
            prev = pv;
            pv = pv->right;
            way = mRight;
        }
    }

    if (found)
    {
        if (pv->left == NULL && pv->right == NULL)
        {
            free(pv); pv = NULL;
            if (way == mLeft)     prev->left = NULL;
            else if (way == mRight)    prev->right = NULL;
            else    *root = NULL;
        }
        else if (pv->right == NULL)
        {
            if (way == mLeft)    prev->left = pv->left;
            else if (way == mRight)   prev->right = pv->left;
            else    *root = pv->left;
            free(pv);
        }
        else if (pv->left == NULL)
        {
            if (way == mLeft)  prev->left = pv->right;
            else if (way == mRight) prev->right = pv->right;
            else    *root = pv->right;
            free(pv);
        }
        else
        {
            struct Node * ptmp = NULL;
            prev = ptmp = pv->right;
            while (ptmp->left)
            {
                prev = ptmp;
                ptmp = ptmp->left;
            }
            pv->d = ptmp->d;

            prev->left = ptmp->right;

            if ((ptmp == prev))
            {
                if (!prev->right)
                {
                    pv->right = NULL;
                }
                else
                {
                    pv->right = prev->right;
                }
            }

            free(ptmp);

        }
        return (true);
    }
    return (false);
}
