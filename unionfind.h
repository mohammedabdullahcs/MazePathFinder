#ifndef UNIONFIND_H 
#define UNIONFIND_H 

#include <vector>

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    UnionFind(int n);
    
    // Find the root of element x with path compression
    int find(int x);
    
    // Union two sets
    bool unite(int x, int y);
    
    // Check if two elements are in same set
    bool connected(int x, int y);
};

#endif // UNIONFIND_H