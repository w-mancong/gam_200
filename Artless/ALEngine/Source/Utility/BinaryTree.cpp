#include "pch.h"

namespace ALEngine::Tree
{
    BinaryTree::BinaryTree() : head{ Memory::DynamicMemory::New<Node>() }
    {
        head->id = -1;
    }

    BinaryTree::~BinaryTree()
    {
        Destruct(-1);
    }

    BinaryTree::Node* BinaryTree::SearchLeft(Node* node, u32 id)
    {
        if (node->id == id)
        {
            return node;
        }

        while (node->left != nullptr)
        {
            searchVect.push_back(node->left);
            if (node->left->id == id)
            {
                prevNode = node;
            }
            node = node->left;
            if (node->id == id)
                return node;
        }
        if (node->right != nullptr) // if have right branch
        {
            return SearchRight(searchVect[searchVect.size() - 1], id);
        }
        else // if no right branch
        {
            if (searchVect.size() > 1)
            {
                searchVect.pop_back();
            }
            return SearchRight(searchVect[searchVect.size() - 1], id);
        }
    }

    BinaryTree::Node* BinaryTree::SearchRight(Node* node, u32 id)
    {
        if (node != nullptr && node->id == id) // if found
        {
            prevNode = searchVect[searchVect.size() - 1];
            return node;
        }

        while (node != nullptr && node->right != nullptr) // go down right row
        {
            prevNode = node;
            if (node->right->id == id)
            {
                return node->right;
            }
            if (node->left != nullptr) // if got left branch
            {
                return SearchLeft(node, id);
            }

            node = node->right; // advance down right row
            searchVect[searchVect.size() - 1] = node;
        }
        // end of right branch
        if (searchVect.size() > 1)
        {
            searchVect.pop_back();
            return SearchRight(searchVect[searchVect.size() - 1], id);
        }
        else
        {
            return nullptr;
        }
    }

    BinaryTree::Node* BinaryTree::Find(u32 id)
    {
        searchVect.clear();
        searchVect.push_back(head->right);
        if (GetHead()->right != nullptr && GetHead()->right->id == id)
            prevNode = GetHead();
        return SearchLeft(head->right, id);
    }

    void BinaryTree::Push(u32 parent, u32 newchild)
    {
        if (parent == -1)
        {
            Insert(head, newchild);
        }
        else
        {
            Node* p = Find(parent);
            if (p->left == nullptr) // first child
            {
                p->left = Memory::DynamicMemory::New<Node>();
                p->left->id = newchild;
            }
            else // already have child
            {
                Insert(p->left, newchild);
            }
        }
    }

    void BinaryTree::Insert(Node* node, u32 id)
    {
        while (node->right != nullptr)
        {
            node = node->right;
        }
        Node* newNode = Memory::DynamicMemory::New<Node>();
        node->right = newNode;
        newNode->id = id;
        newNode->right = nullptr;
        newNode->left = nullptr;
    }

    void BinaryTree::FindLeftChildren(Node* node)
    {
        searchVect.push_back(node);
        while (node->left != nullptr)
        {
            node = node->left;
            searchVect.push_back(node);
        }
        return FindRightChildren(searchVect[searchVect.size() - 1]);
    }

    void BinaryTree::FindRightChildren(Node* node)
    {
        while (node->right != nullptr)
        {
            childrenVect.push_back(node->id);
            node = node->right;
            searchVect[searchVect.size() - 1] = node;
            if (node->left != nullptr) // if have left bramch
            {
                return FindLeftChildren(node->left);
            }
        }
        childrenVect.push_back(node->id);
        if (node->right == nullptr) // dead end
        {
            searchVect.pop_back();
            if (searchVect.size() == 1)
            {
                return;
            }
            return FindRightChildren(searchVect[searchVect.size() - 1]);
        }
    }

    void BinaryTree::FindImmediateChildren(u32 parent)
    {
        childrenVect.clear();
        if (parent == -1)
        {
            return; // root cannot be parent
        }
        Node* node = Find(parent)->left;

        while (node->right != nullptr) // check if parent has children
        {
            childrenVect.push_back(node->id);
            node = node->right;
        }
    }

    void BinaryTree::FindChildren(u32 parent)
    {
        childrenVect.clear();
        if (parent == -1)
        {
            return; // root cannot be parent
        }
        Node* node = Find(parent);

        if (node->left != nullptr) // check if parent has children
        {
            searchVect.clear();
            FindLeftChildren(node);
        }
    }

    std::vector<u32> BinaryTree::GetChildren()
    {
        return childrenVect;
    }

    std::vector<u32> BinaryTree::GetParents()
    {
        Node* node = GetHead()->right;
        std::vector<u32> vect;
        while (node != nullptr)
        {
            vect.push_back(node->id);
            node = node->right;
        }
        return vect;
    }

    void BinaryTree::Destruct(u32 id)
    {
        searchVect.clear();
        destructVect.clear();
        if (id == -1 && GetHead()->right != nullptr)
        {
            GetHead()->left = GetHead()->right;
            GetHead()->right = nullptr;

            return DestructLeft(GetHead(), id);

        }
        else if (GetHead()->right != nullptr)
        {
            prevNode = nullptr;
            Node* node = Find(id);
            if (prevNode == nullptr)
            {
                std::cerr << "Previous node detection failed for " << id << "\n";
            }
            return DestructLeft(node, id);
        }
    }

    void BinaryTree::DestructLeft(Node* node, u32 id)
    {
        searchVect.push_back(node);
        while (node != nullptr && node->left != nullptr)
        {
            node = node->left;
            searchVect.push_back(node);
        }
        return DestructRight(searchVect[searchVect.size() - 1], id);
    }

    void BinaryTree::DestructRight(Node* node, u32 id)
    {
        if (node == nullptr)
        {
            std::cerr << "ERORR: node " << id << " is NULL";
            return;
        }
        while (node != nullptr && node->right != nullptr)
        {
            destructVect.push_back(node);
            node = node->right;
            searchVect[searchVect.size() - 1] = node;
            if (node->left != nullptr) // if have left bramch
            {
                return DestructLeft(node->left, id);
            }
        }
        destructVect.push_back(node);
        if (node != nullptr && node->right == nullptr) // dead end
        {
            searchVect.pop_back();
            if (searchVect.size() == 1)
            {

                if (id != -1)
                {
                    Node* parent{ nullptr };
                    if (prevNode->right != nullptr && prevNode->right->id == id)
                    {
                        parent = prevNode->right;
                    }
                    else if (prevNode->left != nullptr && prevNode->left->id == id)
                    {
                        parent = prevNode->left;
                    }
                    if (parent != nullptr && parent->right == nullptr) // parent is leaf
                    {
                        if (prevNode->right == parent)
                        {
                            prevNode->right = nullptr;
                        }
                        else if (prevNode->left == parent)
                            prevNode->left = nullptr;
                    }
                    else if (parent != nullptr) // parent is sandwhiched
                    {
                        if (parent->right != nullptr && prevNode->right == parent)
                        {
                            prevNode->right = parent->right;

                        }
                        else if (parent->right != nullptr && prevNode->left == parent)
                            prevNode->left = parent->right;
                    }
                }
                for (auto x : destructVect) // delete all parents and children
                {
                    if (x->id == id)
                    {
                        Memory::DynamicMemory::Delete(x);
                        break; // stop when parent is destructed
                    }
                    Memory::DynamicMemory::Delete(x);
                }
                if (id == -1)
                {
                    Node* ptr = GetHead();
                    Memory::DynamicMemory::Delete(ptr); // delete root
                }
                return;
            }
            return DestructRight(searchVect[searchVect.size() - 1], id);
        }
    }

    BinaryTree::Node* BinaryTree::GetHead()
    {
        return head;
    }
} // end of namespace Tree