#include <cstring>
// This file have a tree data structure where each
// node will save one character of path, and leaf node
// will save the build command for that file
// ./src/main.cpp
// ./src/test.cpp
// ./src/myFile.cpp

class Node
{
public:
    virtual ~Node() = default;
    Node *children;
    virtual void AddChild(char ch) = 0;
    virtual void AddChild(char *cmd) = 0;
};

class RootNode : Node
{
};

class InnerNode : public Node
{
private:
    char character;

public:
    InnerNode(char c) : character(c)
    {
    }
};

class LeafNode : public Node
{
private:
    char *buildCmd = nullptr;

public:
    LeafNode(char *buildCmd)
    {
        this->buildCmd = new char[strlen(buildCmd) + 1];
        strcpy(this->buildCmd, buildCmd);
    }
    ~LeafNode()
    {
        delete[] buildCmd;
    }
};

class Tree
{
    Node *root = nullptr;
};

// Visual Representaion
// (root)
//   │
//   ├── s
//   │   ├── r
//   │   │   ├── c
//   │   │   │   ├── /
//   │   │   │   │   ├── m
//   │   │   │   │   │   ├── a
//   │   │   │   │   │   │   ├── i
//   │   │   │   │   │   │   │   ├── n
//   │   │   │   │   │   │   │   │   ├── .
//   │   │   │   │   │   │   │   │   │   ├── c
//   │   │   │   │   │   │   │   │   │   │   ├── p
//   │   │   │   │   │   │   │   │   │   │   │   ├── p  (build command for main.cpp)
//   │   │   │   │   ├── t
//   │   │   │   │   │   ├── e
//   │   │   │   │   │   │   ├── s
//   │   │   │   │   │   │   │   ├── t
//   │   │   │   │   │   │   │   │   ├── .
//   │   │   │   │   │   │   │   │   │   ├── c
//   │   │   │   │   │   │   │   │   │   │   ├── p
//   │   │   │   │   │   │   │   │   │   │   │   ├── p  (build command for test.cpp)
//   │   │   │   │   ├── m
//   │   │   │   │   │   ├── y
//   │   │   │   │   │   │   ├── F
//   │   │   │   │   │   │   │   ├── i
//   │   │   │   │   │   │   │   │   ├── l
//   │   │   │   │   │   │   │   │   │   ├── e
//   │   │   │   │   │   │   │   │   │   │   ├── .
//   │   │   │   │   │   │   │   │   │   │   │   ├── c
//   │   │   │   │   │   │   │   │   │   │   │   │   ├── p
//   │   │   │   │   │   │   │   │   │   │   │   │   │   ├── p  (build command for myFile.cpp)
