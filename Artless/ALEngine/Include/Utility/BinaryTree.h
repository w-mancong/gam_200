/*!
file: BinaryTree.h
author: Mohamed Zafir
email: m.zafir@digipen.edu
brief: This file contains the function declarations for class BinaryTree. Which is a data
       structure to store parent-child relationships.

All content :copyright: 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*_______________________________________________________________________________________*/

#ifndef	BINARY_TREE_H
#define BINARY_TREE_H

namespace ALEngine::Tree
{
    class BinaryTree
    {
    public:
        struct Node
        {
            s32 id{};
            Node* right{ nullptr }; // sibling
            Node* left{ nullptr }; // child
            b8 parent{ false };
        };
        struct NodeData
        {
            u32 id{};
            std::vector<s32> children{};
            b8 active{ true };
            s32 parent{-1};
        };
        struct Serial
        {
            s32 serialID, parentSerialID;
            s32 ogID;
            b8 flag{ false }; // for deserialization
        };
    public:
        /*!*********************************************************************************
            \brief
            Constructor for BinaryTree
        ***********************************************************************************/
        BinaryTree();

        /*!*********************************************************************************
            \brief
            Destructor for BinaryTree
        ***********************************************************************************/
        ~BinaryTree();

        /*!*********************************************************************************
            \brief
            Initializes the BinaryTree
        ***********************************************************************************/
        void Init();

        /*!*********************************************************************************
            \brief
            Pushes a new node into the BinaryTree

            \param [in] parent
            ID of parent

            \param [in] newChild
            ID of new child to insert
        ***********************************************************************************/
        void Push(s32 parent, s32 newChild);
        
        /*!*********************************************************************************
            \brief
            Finds a node in the BinaryTree

            \param [in] node
            Node ID

            \return
            Node*
        ***********************************************************************************/
        Node* Find(s32 node);

        /*!*********************************************************************************
            \brief
            Finds the children of a specified parent

            \param [in] parent
            ID of parent
        ***********************************************************************************/
        void FindChildren(s32 parent);

        /*!*********************************************************************************
            \brief
            Finds the immediate children of a specified parent

            \param [in] parent
            ID of parent
        ***********************************************************************************/
        void FindImmediateChildren(s32 parent);

        /*!*********************************************************************************
            \brief
            Destructs a specified parent node and its children in the BinaryTree

            \param [in] parent
            ID of parent
        ***********************************************************************************/
        void Destruct(s32 s32);

        /*!*********************************************************************************
            \brief
            Returns a vector of children after using FindChildren() or 
            FindImmediateChildren()

            \return 
            Vector of node IDs
        ***********************************************************************************/
        std::vector<s32> GetChildren();

        /*!*********************************************************************************
            \brief
            Returns a vector of the first layer of parents in the BinaryTree.

            \return
            Vector of node IDs
        ***********************************************************************************/
        std::vector<s32> GetParents();

        /*!*********************************************************************************
            \brief
            Returns a vector of NodeData of all nodes in the BinaryTree

            \return 
            Vector of NodeData
        ***********************************************************************************/
        std::vector<NodeData>const& GetMap();

        void MoveBranch(s32 branch, s32 newParent);
        std::vector<Serial> SerializeTree();
        void DeserializeTree(std::vector<Serial> serialVect);
        void DeserializeHelper(std::vector<BinaryTree::Serial>& serialVect);

    private:
        /*!*********************************************************************************
            \brief
            Returns the head of the BinaryTree

            \return
            Node*
        ***********************************************************************************/
        Node* GetHead();

        /*!*********************************************************************************
            \brief
            Search the right side of the BinaryTree

            \param [in] node
            Pointer to specified node

            \param [in] id
            ID of node to find

            \return
            Node*
        ***********************************************************************************/
        Node* SearchRight(Node* node, s32 id);

        /*!*********************************************************************************
            \brief
            Search the left side of the BinaryTree

            \param [in] node
            Pointer to specified node

            \param [in] node
            ID of node to find

            \return
            Node*
        ***********************************************************************************/
        Node* SearchLeft(Node* node, s32 id);

        /*!*********************************************************************************
            \brief
            Search the left side of the BinaryTree for children

            \param [in] node
            Pointer to specified node
        ***********************************************************************************/
        void FindLeftChildren(Node* node);

        /*!*********************************************************************************
            \brief
            Search the right side of the BinaryTree for children

            \param [in] node
            Pointer to specified node
        ***********************************************************************************/
        void FindRightChildren(Node* node);

        /*!*********************************************************************************
            \brief
            Insert a node into the BinaryTree

            \param [in] node
            Pointer to specified node

            \param [in] id
            ID of node to insert
        ***********************************************************************************/
        void Insert(Node* node, s32 id);

        /*!*********************************************************************************
            \brief
            Searches the right side of the BinaryTree to destruct the specified node ID.

            \param [in] node
            Pointer to specified node

            \param [in] id
            ID of node to destruct
        ***********************************************************************************/
        void DestructRight(Node* node, s32 id);

        /*!*********************************************************************************
            \brief
            Searches the left side of the BinaryTree to destruct the specified node ID.

            \param [in] node
            Pointer to specified node

            \param [in] id
            ID of node to destruct
        ***********************************************************************************/
        void DestructLeft(Node* node, s32 id);

    private:
        Node* head;
        Node* prevNode{ nullptr };
        std::vector<Node*> searchVect{};
        std::vector<Node*> destructVect{};
        std::vector<s32> childrenVect{};
        std::vector<NodeData> map{};
    }; // end of class BinaryTree
} // end of namespace Tree
#endif