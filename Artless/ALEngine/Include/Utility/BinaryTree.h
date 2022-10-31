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
    public:
        BinaryTree();
        ~BinaryTree(); // destruct
        void Init();
        void Push(s32 parent, s32 newchild);
        Node* Find(s32 node);
        void FindChildren(s32 parent);
        void FindImmediateChildren(s32 parent);
        void Destruct(s32 s32);
        std::vector<s32> GetChildren();
        Node* GetHead();
        std::vector<s32> GetParents();
        std::vector<NodeData>const& GetMap();

    private:
        Node* SearchRight(Node* node, s32 id);
        Node* SearchLeft(Node* node, s32 id);
        void FindLeftChildren(Node* node);
        void FindRightChildren(Node* node);
        void Insert(Node* node, s32 id);
        void DestructRight(Node* node, s32 id);
        void DestructLeft(Node* node, s32 id);

        Node* head;
        Node* prevNode{ nullptr };
        std::vector<Node*> searchVect{};
        std::vector<Node*> destructVect{};
        std::vector<s32> childrenVect{};
        std::vector<NodeData> map{};
    }; // end of class BinaryTree
} // end of namespace Tree
#endif