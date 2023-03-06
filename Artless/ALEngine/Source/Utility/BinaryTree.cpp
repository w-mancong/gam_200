/*!
file: BinaryTree.cpp
author: Mohamed Zafir
email: m.zafir\@digipen.edu
brief: This file contains the function definitions for class BinaryTree. Which is a data
       structure to store parent-child relationships.

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*________________________________________________________________________________________*/
#include "pch.h"

namespace ALEngine::Tree
{
    namespace
    {
        void UpdateGlobalCoordinates(Transform& trans)
        {
            if (trans.localPosition != trans.prevPosition || trans.localScale != trans.prevScale || trans.localRotation != trans.prevRotation)
                trans.isDirty = true;
            else if (trans.position != trans.localPosition || trans.scale != trans.localScale || trans.rotation != trans.localRotation)
            {
                trans.localPosition = trans.position;
                trans.localRotation = trans.rotation;
                trans.localScale    = trans.scale;
            }
            if (trans.isDirty)
            {
                trans.position = trans.localPosition;
                trans.rotation = trans.localRotation;
                trans.scale    = trans.localScale;
            }
        }

        void UpdateGlobalCoordinates(Transform& trans, Transform const& parentTrans)
        {
            if (trans.position != trans.prevPosition)
            {
                trans.localPosition = parentTrans.modelMatrix.Inverse() * trans.position;
            }
            if (trans.scale != trans.prevScale)
            {
                trans.localScale = { trans.scale.x / parentTrans.scale.x, trans.scale.y / parentTrans.scale.y };
            }
            if (trans.rotation != trans.prevRotation)
            {
                trans.localRotation = trans.rotation - parentTrans.rotation;
            }
            trans.position = parentTrans.modelMatrix * trans.localPosition;
            trans.scale = Math::mat4::Scale(parentTrans.scale) * Math::vec3(trans.localScale);
            trans.rotation = trans.localRotation + parentTrans.rotation;
        }

        void UpdateLocalCoordinates(Transform& trans)
        {
            trans.localPosition = trans.position;
            trans.localRotation = trans.rotation;
            trans.localScale    = trans.scale;
        }

        void UpdateLocalCoordinates(Transform& trans, [[maybe_unused]] Transform const& parentTrans)
        {
            trans.localPosition = math::mat4::Model({}, { parentTrans.scale.x, parentTrans.scale.y, 1.0f }, parentTrans.rotation).Inverse() * (trans.position - parentTrans.position);
            trans.localRotation = trans.rotation - parentTrans.rotation;
            trans.localScale    = { trans.scale.x / parentTrans.scale.x, trans.scale.y / parentTrans.scale.y };
        }

        void UpdateParentChildrenPos(Tree::BinaryTree::NodeData const& entity)
        {
            Transform& trans = Coordinator::Instance()->GetComponent<Transform>(entity.id);

            if (entity.parent >= 0) // if entity has parent
            {
                Transform const& parentTrans = Coordinator::Instance()->GetComponent<Transform>(entity.parent);
                trans.modelMatrix = parentTrans.modelMatrix * Math::mat4::Model(trans);
            }
            else
            {
                /*
                    Don't have to construct a model matrix using global coordinates because
                    entities that do not have a parent, their local coordinates are their global coordinates
                */
                trans.modelMatrix = Math::mat4::Model(trans);
            }
        }

        void UpdateWorld(Transform& trans, ECS::Entity entity)
        {
            s32 parent = ECS::GetSceneGraph().GetParent(entity);
            Transform parentTrans{};
            if (parent != -1)
                trans.isDirty = ( parentTrans = Coordinator::Instance()->GetComponent<Transform>(parent) ).isDirty;
            if (trans.isDirty)
            {
                if (parent != -1)
                    UpdateLocalCoordinates(trans, parentTrans);
                else
                    UpdateLocalCoordinates(trans);
            }            

            UpdateParentChildrenPos(ECS::GetSceneGraph().GetMap()[entity]);

            for (s32 child : ECS::GetSceneGraph().GetMap()[entity].children)
            {
                Transform& childTrans = Coordinator::Instance()->GetComponent<Transform>(static_cast<u32>(child));
                UpdateGlobalCoordinates(childTrans, trans);
                UpdateWorld(childTrans, child);
            }

            if (parent != -1)
            {
                trans.prevPosition = trans.position;
                trans.prevRotation = trans.rotation;
                trans.prevScale    = trans.scale;
            }
            else
            {
                trans.prevPosition = trans.localPosition;
                trans.prevRotation = trans.localRotation;
                trans.prevScale    = trans.localScale;
            }
            trans.isDirty      = false;
        }
    }

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

    void BinaryTree::Update() const
    {
        for (s32 entity : GetParents())
        {
            // Update entity's global position
            Transform& trans = Coordinator::Instance()->GetComponent<Transform>(static_cast<u32>(entity));
            UpdateGlobalCoordinates(trans);
            UpdateWorld(trans, entity);
        }
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
            node = node->left;
            searchVect.push_back(node);

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
                FindImmediateChildren(x.id);
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

    void BinaryTree::Insert(Node* node, s32 id) const
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

    std::vector<s32> const& BinaryTree::GetChildren() const
    {
        return childrenVect;
    }

    std::vector<s32> BinaryTree::GetParents() const
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
                        FindImmediateChildren(x.id);
                        x.children = GetChildren();
                    }
                }
                return;
            }
            return DestructRight(searchVect[searchVect.size() - 1], id);
        }
    }

    BinaryTree::Node* BinaryTree::GetHead() const
    {
        return head;
    }

    std::vector<BinaryTree::NodeData>const& BinaryTree::GetMap() const
    {
        return map;
    }

    std::vector<BinaryTree::NodeData>& BinaryTree::GetMap()
    {
        return map;
    }

    s32 BinaryTree::GetParent(u32 en) const
    {
        return map[en].parent;
    }

    void BinaryTree::SetParentChildActive(NodeData const& node, b8 activeState) const
    {
        EntityData& ed = Coordinator::Instance()->GetComponent<EntityData>(node.id);

        if (node.parent != -1)
        {
            EntityData const& parent_ed = Coordinator::Instance()->GetComponent<EntityData>(node.parent);
            if (ed.selfActive)
                ed.active = parent_ed.active;
        }
        else
        {
            if (ed.selfActive)
                ed.active = activeState;
        }

        for (s32 en : node.children)
        {
            Tree::BinaryTree::NodeData const& nd = ECS::GetSceneGraph(0).GetMap()[en];
            SetParentChildActive(nd, activeState);
        }
    }

    void BinaryTree::MoveBranch(s32 branch, s32 newParent)
    {
        searchVect.clear();
        Node* branchNode = Find(branch);

        if (prevNode->left == branchNode) // if branchNode is the first child of PrevNode
        {
            if (branchNode && branchNode->right != nullptr)
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
            if (branchNode && branchNode->right == nullptr) // if leaf
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
            newParentNode = head;
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

        if(branchNode)
            branchNode->right = nullptr;
        map[branch].parent = newParent; // assign new parent

        // update map
        for (auto& x : map)
        {
            if (x.active)
            {
                FindImmediateChildren(x.id);
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

                //std::cout << en.tag << " " << en.id << " " << en.parentID << "\n";

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
        std::unordered_map<s32, ECS::Entity> parentsID{};
        for (auto it{ entities.begin() }; it != entities.end(); ++it)
        {
            EntityData& en = Coordinator::Instance()->GetComponent<EntityData>(*it);
            Serial serial;
            serial.id = *it;
            serial.serialID = en.id;
            serial.parentSerialID = en.parentID;
            parentsID[serial.serialID] = *it;
            serialVect.push_back(serial);
        }

        for (auto& x : serialVect)
        {
            if (x.parentSerialID == -1)
            {
                x.flag = true;
                insertedVect.push_back(x.serialID);
                Push(x.parentSerialID, static_cast<s32>(x.id));
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
                        Push(parentsID[it->parentSerialID], static_cast<s32>(it->id));
                        insertedVect.push_back(it->serialID);
                        break;
                    }
                }
            }
        } while (done == false);
    }
} // end of namespace Tree-