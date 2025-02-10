#include <cstring>
#include <stdio.h>
#include <unordered_map>
// This file have a tree data structure where each
// node will save one character of path, and leaf node
// will save the build command for that file
// ./src/main.cpp
// ./src/test.cpp
// ./src/myFile.cpp

class Node
{
private:
    bool isRoot = false;
    bool isExcludedFolder = false;
    bool isExcludedFile = false;
    char path_char;
    char *build_cmd;
    std::unordered_map<char, Node *> children;

public:
    void setAsRoot(bool isRoot = true)
    {
        this->isRoot = isRoot;
    }
    void setAsExcludedFolder(bool isExcludedFolder = true)
    {
        this->isExcludedFolder = isExcludedFolder;
    }
    void setAsExcludedFile(bool isExcludedFile = true)
    {
        this->isExcludedFile = isExcludedFile;
    }
    bool isExcludedFolderNode()
    {
        return this->isExcludedFolder;
    }
    bool isExcludedFileNode()
    {
        return this->isExcludedFile;
    }
    bool isRootNode()
    {
        return this->isRoot;
    }
    Node *getChild(char c)
    {
        auto it = children.find(c);
        if (it != children.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void setChild(char c, Node *child)
    {
        children[c] = child;
    }
    char setPathChar(char path_char)
    {
        if (isRoot)
        {
            throw "Root node cannot have path_char";
        }
        this->path_char = path_char;
        return this->path_char;
    }
    const char *setBuildCmd(char *build_cmd)
    {
        if (isRoot)
        {
            throw "Root node cannot have build_cmd";
        }
        this->build_cmd = new char[strlen(build_cmd) + 1];
        strcpy(this->build_cmd, build_cmd);
        return this->build_cmd;
    }
    char getPathChar()
    {
        if (isRoot)
        {
            throw "Root node cannot have path_char";
        }
        return this->path_char;
    }
    const char *getBuildCmd()
    {
        if (isRoot)
        {
            throw "Root node cannot have build_cmd";
        }
        return this->build_cmd;
    }
    std::unordered_map<char, Node *> getChildren()
    {
        return children;
    }
};

class PathTree
{
private:
    Node *root;

private:
    void markExcludedFolder(Node *node)
    {
        if (node == nullptr)
        {
            return;
        }
        node->setAsExcludedFolder();
        for (auto &child : node->getChildren())
        {
            markExcludedFolder(child.second);
        }
    }

public:
    PathTree()
    {
        root = new Node();
        root->setAsRoot();
    }

    Node *getRoot()
    {
        return root;
    }

    void addExcludeFolder(const char *path)
    {
        Node *current = root;
        for (int i = 0; path[i] != '\0'; i++)
        {
            Node *child = current->getChild(path[i]);
            if (child == nullptr)
            {
                child = new Node();
                child->setPathChar(path[i]);
                current->setChild(path[i], child);
            }
            current = child;
        }
        markExcludedFolder(current);
    }

    void addExcludeFile(const char *path)
    {
        Node *current = root;
        for (int i = 0; path[i] != '\0'; i++)
        {
            Node *child = current->getChild(path[i]);
            if (child == nullptr)
            {
                child = new Node();
                child->setPathChar(path[i]);
                current->setChild(path[i], child);
            }
            current = child;
        }
        current->setAsExcludedFile();
    }

    Node *insertPath(const char *path, const char *build_cmd)
    {
        Node *current = root;
        for (int i = 0; path[i] != '\0'; i++)
        {
            Node *child = current->getChild(path[i]);
            if (child == nullptr)
            {
                child = new Node();
                child->setPathChar(path[i]);
                current->setChild(path[i], child);
            }
            current = child;
        }
        current->setBuildCmd((char *)build_cmd);
        return current;
    }

    Node *searchPath(const char *path)
    {
        Node *current = root;
        for (int i = 0; path[i] != '\0'; i++)
        {
            Node *child = current->getChild(path[i]);
            if (child == nullptr)
            {
                return nullptr;
            }
            current = child;
        }
        return current;
    }
};

int main()
{
    PathTree *pathTree = new PathTree();
    pathTree->insertPath("./src/main.cpp", "make main");
    pathTree->insertPath("./src/test.cpp", "make test");
    pathTree->insertPath("./src/myFile.cpp", "make myFile");
    pathTree->insertPath("./bin/bin.cpp", "make build");

    pathTree->addExcludeFolder("./bin");
    pathTree->addExcludeFile("./src/test.cpp");

    Node *node = pathTree->searchPath("./src/test.cpp");
    if (node != nullptr)
    {
        const char *build_cmd = node->getBuildCmd();
        printf("Properties of Node\n");
        printf("Path Char: %c\n", node->getPathChar());
        printf("Build Command: %s\n", build_cmd);
        printf("Is Excluded Folder: %s\n", node->isExcludedFolderNode() ? "true" : "false");
        printf("Is Excluded File: %s\n", node->isExcludedFileNode() ? "true" : "false");

        // printf("Build command for ./src/main.cpp is %s\n", build_cmd);
    }
    else
    {
        printf("Path not found\n");
    }
    return 0;
}