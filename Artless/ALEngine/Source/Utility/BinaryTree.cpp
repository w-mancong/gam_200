#include "pch.h"

namespace ALEngine::Tree
{
    BinaryTree::BinaryTree() : head{ Memory::DynamicMemory::New<Node>() }
    {
    }

    BinaryTree::~BinaryTree()
    {
        Destruct(0);
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
        if (node->right != nullptr)
            return SearchRight(searchVect[searchVect.size() - 1], id);
        else
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
        if (node->id == id) // if found
        {
            prevNode = searchVect[searchVect.size() - 1];
            return node;
        }

        if (node->right == nullptr) // end of right row
        {
            searchVect.pop_back();
            return SearchRight(searchVect[searchVect.size() - 1], id);
        }

        while (node->right != nullptr) // go down right row
        {
            if (node->right->id == id)
            {
                prevNode = node;
                return node->right;
            }

            node = node->right; // advance down right row
            searchVect[searchVect.size() - 1] = node;
            if (node->left != nullptr) // if got left branch
            {
                if (node->left->id == id)
                {
                    prevNode = node;
                }
                return SearchLeft(node, id);
            }
        }
        searchVect.pop_back();
        return SearchRight(searchVect[searchVect.size() - 1], id);
    }

    BinaryTree::Node* BinaryTree::Find(u32 id)
    {
        searchVect.clear();
        searchVect.push_back(head->right);
        if (head->right->id == id)
            prevNode = GetHead();
        return SearchLeft(head->right, id);
    }

    void BinaryTree::Push(u32 parent, u32 newchild)
    {
        if (parent == 0)
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
        node->right = Memory::DynamicMemory::New<Node>();
        node->right->id = id;
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

    void BinaryTree::FindChildren(u32 parent)
    {
        childrenVect.clear();
        if (parent == 0)
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

    std::vector<u32> const& BinaryTree::GetChildren()
    {
        return childrenVect;
    }

    void BinaryTree::Destruct(u32 id)
    {
        searchVect.clear();
        destructVect.clear();
        prevNode = nullptr;
        if (id == 0 && GetHead()->right != nullptr)
        {
            GetHead()->left = GetHead()->right;
            GetHead()->right = nullptr;

            return DestructLeft(GetHead(), id);

        }
        else if (GetHead()->right != nullptr)
        {
            Node* node = Find(id);

            return DestructLeft(node, id);
        }
        Node* ptr = GetHead();
        Memory::DynamicMemory::Delete(ptr);
    }

    void BinaryTree::DestructLeft(Node* node, u32 id)
    {
        searchVect.push_back(node);
        while (node->left != nullptr)
        {
            node = node->left;
            searchVect.push_back(node);
        }
        return DestructRight(searchVect[searchVect.size() - 1], id);
    }

    void BinaryTree::DestructRight(Node* node, u32 id)
    {
        while (node->right != nullptr)
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
        if (node->right == nullptr) // dead end
        {
            searchVect.pop_back();
            if (searchVect.size() == 1)
            {

                if (id != 0)
                {
                    Node* parent{nullptr};
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
                        else
                            prevNode->left = nullptr;
                    }
                    else if(parent != nullptr) // parent is sandwhiched
                    {
                        if (prevNode->right == parent)
                        {
                            prevNode->right = parent->right;
                        }
                        else
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
                if (id == 0)
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