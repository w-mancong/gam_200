#ifndef	BINARY_TREE_H
#define BINARY_TREE_H

namespace ALEngine::Tree
{
    class BinaryTree
    {
    public:
        struct Node
        {
            u32 id{};
            Node* right{ nullptr }; // sibling
            Node* left{ nullptr }; // child
        };
    public:
        BinaryTree();
        ~BinaryTree(); // destruct
        void Push(u32 parent, u32 newchild);
        Node* Find(u32 id);
        void FindChildren(u32 parent);
        void Destruct(u32 id);
        std::vector<u32> const& GetChildren();
        Node* GetHead();
        std::vector<u32> GetParents();
    private:
        Node* SearchRight(Node* node, u32 id);
        Node* SearchLeft(Node* node, u32 id);
        void FindLeftChildren(Node* node);
        void FindRightChildren(Node* node);
        void Insert(Node* node, u32 id);
        void DestructRight(Node* node, u32 id);
        void DestructLeft(Node* node, u32 id);

        Node* head;
        Node* prevNode{ nullptr };
        std::vector<Node*> searchVect{};
        std::vector<Node*> destructVect{};
        std::vector<u32> childrenVect{};
    }; // end of class BinaryTree
} // end of namespace Tree
#endif