#include "unionfind.h"

UnionFind::UnionFind(int n) {
    parent.resize(n);
    rank.resize(n, 0);
    
    // Each element is its own parent initially
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
}

int UnionFind::find(int x) {
    // Path compression: make x point directly to root
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

bool UnionFind::unite(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    
    // Already in same set
    if (rootX == rootY) {
        return false;
    }
    
    // Union by rank: attach smaller tree under larger
    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
    
    return true;
}

bool UnionFind::connected(int x, int y) {
    return find(x) == find(y);
}