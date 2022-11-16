/*!
file: BinaryTree.cpp
author: Mohamed Zafir
email: m.zafir@digipen.edu
brief: This file contains the function definitions for class BinaryTree. Which is a data
       structure to store parent-child relationships.

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*________________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Tree
{
    BinaryTree::BinaryTree() : head{ nullptr }
    {
    }

    BinaryTree::~BinaryTree()
    {
    }

    void BinaryTree::Init(void)
    {
        head = Memory::DynamicMemory::New<Node>();
        head->id = -1;
    }

    BinaryTree::Node* BinaryTree::SearchLeft(Node* node, s32 id)
    {
        if (node == nullptr)
            return nullptr;

        if (node->id == id)
        {
            return node;
        }

        while (node->left != nullptr)
        {
            prevNode = node;
            searchVect.push_back(node);
            node = node->left;

            if (node->id == id)
            {
                return node;
            }
        }
        if (node->right != nullptr) // if have right branch
        {
            return SearchRight(node, id);
        }
        else // if no right branch
        {
            searchVect.pop_back();
            return SearchRight(searchVect[searchVect.size() - 1], id);
        }
    }

    BinaryTree::Node* BinaryTree::SearchRight(Node* node, s32 id)
    {
        if (node == nullptr)
            return nullptr;

        if (node->id == id) // if found
        {
            prevNode = searchVect[searchVect.size() - 1];
            return node;
        }

        searchVect[searchVect.size() - 1] = node;
        while (node->right != nullptr) // go down right row
        {
            prevNode = node;
            node = node->right; // advance down right row
            if (node->id == id)
            {
                return node;
            }
            searchVect[searchVect.size() - 1] = node;
            if (node->left != nullptr) // if got left branch
            {
                return SearchLeft(node, id);
            }

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

    BinaryTree::Node* BinaryTree::Find(s32 id)
    {
        searchVect.clear();

        if (GetHead()->right == nullptr)
            return nullptr;

        searchVect.push_back(head);

        return SearchRight(head, id);
    }

    void BinaryTree::Push(s32 parent, s32 newChild)
    {
        if (parent == -1)
        {
            Insert(head, newChild);
        }
        else
        {
            Node* p = Find(parent);
            if (p == nullptr)
                return;

            if (p->left == nullptr)
            {
                p->parent = true;
            }

            if (p->left == nullptr) // first child
            {
                p->left = Memory::DynamicMemory::New<Node>();
                p->left->id = newChild;
            }
            else // already have child
            {
                Insert(p->left, newChild);
            }
        }

        // update map
        for (auto &x : map)
        {
            if (x.active)
            {
                FindChildren(x.id);
                x.children = GetChildren();
            }
        }
        // new node data
        NodeData newData;
        newData.id = newChild;
        newData.active = true;
        newData.parent = parent;

        if (newChild < map.size())
        {
            map[newChild] = newData;
            return;
        }
        else if (newChild > map.size())
        {
            map.resize(++newChild);
            map[--newChild] = newData;
            return;
        }
        else if(newChild == map.size())
            map.push_back(newData);
    }

    void BinaryTree::Insert(Node* node, s32 id)
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

    void BinaryTree::FindImmediateChildren(s32 parent)
    {
        childrenVect.clear();
        if (parent == -1)
        {
            return; // root cannot be parent
        }
        Node* node = Find(parent);
        if (node == nullptr || node->left == nullptr)
            return;

        node = node->left;
        childrenVect.push_back(node->id);
        while (node != nullptr && node->right != nullptr) // check if parent has children
        {
            node = node->right;
            childrenVect.push_back(node->id);
        }
    }

    void BinaryTree::FindChildren(s32 parent)
    {
        childrenVect.clear();
        if (parent == -1)
        {
            return; // root cannot be parent
        }
        Node* node = Find(parent);
        if (node == nullptr)
            return;

        if (node->left != nullptr) // check if parent has children
        {
            searchVect.clear();
            FindLeftChildren(node);
        }
    }

    std::vector<s32> BinaryTree::GetChildren()
    {
        return childrenVect;
    }

    std::vector<s32> BinaryTree::GetParents()
    {
        Node* node = GetHead()->right;
        std::vector<s32> vect;
        while (node != nullptr)
        {
            vect.push_back(node->id);
            node = node->right;
        }
        return vect;
    }

    void BinaryTree::Destruct(s32 id)
    {
        searchVect.clear();
        destructVect.clear();
        if (id == -1 && GetHead() != nullptr && GetHead()->right != nullptr)
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

    void BinaryTree::DestructLeft(Node* node, s32 id)
    {
        searchVect.push_back(node);
        while (node != nullptr && node->left != nullptr)
        {
            node = node->left;
            searchVect.push_back(node);
        }
        return DestructRight(searchVect[searchVect.size() - 1], id);
    }

    void BinaryTree::DestructRight(Node* node, s32 id)
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
                        {
                            prevNode->left = nullptr;
                            prevNode->parent = false;
                        }
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
                        map[x->id].active = false;
                        Memory::DynamicMemory::Delete(x); // delete parent
                        break; // stop when parent is destructed
                    }
                    map[x->id].active = false;
                    Memory::DynamicMemory::Delete(x);
                }
                //if (id == -1)
                //{
                //    Node* ptr = GetHead();
                //    Memory::DynamicMemory::Delete(ptr); // delete root
                //}

                //update map
                for (auto& x : map)
                {
                    if (x.active)
                    {
                        FindChildren(x.id);
                        x.children = GetChildren();
                    }
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

    std::vector<BinaryTree::NodeData>const& BinaryTree::GetMap()
    {
        return map;
    }

    void BinaryTree::MoveBranch(s32 branch, s32 newParent)
    {
        searchVect.clear();
        Node* branchNode = Find(branch);

        if (prevNode->left == branchNode) // if branchNode is the first child of PrevNode
        {
            if (branchNode->right != nullptr)
            {
                prevNode->left = branchNode->right;
            }
            else
            {
                prevNode->left = nullptr;
            }
        }
        else // branchNode is sibling of prevNode
        {
            if (branchNode->right == nullptr) // if leaf
            {
                prevNode->right = nullptr;
            }
            else // sandwhiched
            {
                prevNode->right = branchNode->right;
            }
        }

        Node* newParentNode;
        if (newParent == -1)
        {
            newParentNode = head->right;
        }
        else
        {
            searchVect.clear();
            newParentNode = Find(newParent);
        }

        if (newParentNode->left == nullptr && newParent != -1)
        {
            newParentNode->left = branchNode;
        }
        else
        {
            if(newParent != -1)
                newParentNode = newParentNode->left;
            while (newParentNode->right != nullptr)
                newParentNode = newParentNode->right;

            newParentNode->right = branchNode;
        }

        branchNode->right = nullptr;
        map[branch].parent = newParent; // assign new parent

        // update map
        for (auto& x : map)
        {
            if (x.active)
            {
                FindChildren(x.id);
                x.children = GetChildren();
            }
        }
    }

    void BinaryTree::SerializeTree()
    {
        std::vector<Serial> serialVect;
        std::vector<s32> conversionTable{};
        s32 newID{};
        for (u32 i{}; i < map.size(); ++i)
        {
            if (map[i].active)
            {
                EntityData& en = Coordinator::Instance()->GetComponent<EntityData>(map[i].id);
                Serial newSerial{};
                newSerial.serialID = newID;
                conversionTable.push_back(newID);
                newSerial.parentSerialID = map[i].parent; // old parent
                serialVect.push_back(newSerial);

                en.id = newSerial.serialID;
                en.parentID = newSerial.parentSerialID;

                ++newID;
            }
            else
            {
                conversionTable.push_back(-1);
            }
        }

        ECS::EntityList const& entities = Coordinator::Instance()->GetEntities();
        for (auto it{ entities.begin() }; it != entities.end(); ++it)
        {
            EntityData& data = Coordinator::Instance()->GetComponent<EntityData>(*it);
            if(data.parentID != -1)
                data.parentID = conversionTable[data.parentID];
        }
    }

    void BinaryTree::DeserializeTree()
    {
        ECS::EntityList const& entities = Coordinator::Instance()->GetEntities();
        std::vector<Serial> serialVect;
        std::vector<s32> insertedVect;
        for (auto it{ entities.begin() }; it != entities.end(); ++it)
        {
            EntityData& en = Coordinator::Instance()->GetComponent<EntityData>(*it);
            Serial serial;
            serial.serialID = *it;
            serial.parentSerialID = en.parentID;
            serialVect.push_back(serial);
        }

        for (auto& x : serialVect)
        {
            if (x.parentSerialID == -1)
            {
                x.flag = true;
                insertedVect.push_back(x.serialID);
                Push(x.parentSerialID, x.serialID);
            }
        }

        s32 done{ true };
        do
        {
            done = true;
            for (std::vector<Serial>::iterator it{ serialVect.begin() }; it != serialVect.end(); ++it)
            {
                if (it->flag == true) // if already inserted
                    continue;

                done = false;
                for (auto& x : insertedVect)
                {
                    if (it->parentSerialID == x)
                    {
                        it->flag = true;
                        Push(it->parentSerialID, it->serialID);
                        insertedVect.push_back(it->serialID);
                        break;
                    }
                }
            }
        } while (done == false);
    }

} // end of namespace Tree