
/**
Consider a two player game on a graph with a specified vertex (root).
In each turn, a player eliminates one edge.
Then, if a subgraph that is disconnected from the root, it is removed.
If a player cannot select an edge (i.e., the graph is singleton),
he will lose.
Compute the Grundy number of the given graph.

We will use two principles:
1. Colon Principle:
    Grundy number of a tree is the xor of
    Grundy number of child subtrees.

2. Fusion Principle:
    Consider a pair of adjacent vertices u, v
    that has another path (i.e., they are in a cycle). Then,
    we can contract u and v without changing Grundy number.

We first decompose graph into two-edge connected components.
Then, by contracting each components by using Fusion Principle,
we obtain a tree (and many self loops) that has the same Grundy
number to the original graph. By using Colon Principle, we can
compute the Grundy number.

Complexity: O(m + n).

Verified:
SPOJ 1477: Play with a Tree
IPSC 2003 G: Got Root?
**/
#include<bits/stdc++.h>
using namespace std;
const int N = 3e5 + 9;

// Graph adjacency list and helper variables
vector<int> g[N];
int n, T, low[N], dis[N];

/**
 * DFS function to calculate the Grundy number for the graph.
 * @param u - Current vertex
 * @param pre - Parent vertex (to avoid backtracking)
 * @return Grundy number of the subtree rooted at u
 */
int dfs (int u, int pre = 0) {
    dis[u] = low[u] = ++T;  // Set discovery and low-link time for the current node
    int ans = 0;  // Initialize Grundy number for the current subtree

    for (auto v : g[u]) { // Iterate through neighbors of u
        if (v == pre) {  // Skip the edge to the parent node
            pre += 2 * n;  // Temporarily adjust pre to avoid direct backtracking
            continue;
        }
        if (dis[v] == 0) {  // If the neighbor is unvisited
            int res = dfs(v, u);  // Recur for the neighbor
            low[u] = min(low[u], low[v]);  // Update low-link value for u

            if (low[v] > dis[u])  // If v forms a bridge
                ans ^= (1 + res) ^ 1;  // Update Grundy number for a bridge
            else  // Non-bridge edge
                ans ^= res;  // Update Grundy number directly
        } else {
            low[u] = min(low[u], dis[v]);  // Update low-link for back edges
        }
    }

    if (pre > n) pre -= 2 * n;  // Reset pre after temporary adjustment

    for (auto v : g[u]) {  // Handle remaining edges
        if (v != pre && dis[u] <= dis[v]) ans ^= 1;  // Add 1 for valid cycle edges
    }

    return ans;  // Return the Grundy number for this subtree
}

// Array to track ground nodes (fixed vertices like the root)
int ground[N];

int32_t main() {
    int t;
    scanf("%d", &t);  // Read the number of test cases
    
    while(t--) {
        int m;
        scanf("%d%d", &n, &m);  // Read the number of vertices and edges

        for (int i = 1; i <= n; i++) ground[i] = 0;  // Reset ground nodes

        int root = 1;  // Arbitrarily choose the root node
        ground[root] = 1;  // Mark the root as a ground node

        int ans = 0;  // Initialize the final Grundy number
        T = 0;  // Reset the global DFS timer

        for (int i = 0; i < m; ++i) {
            int u, v;
            scanf("%d %d", &u, &v);  // Read an edge

            if (ground[u]) u = root;  // Replace ground node u with root
            if (ground[v]) v = root;  // Replace ground node v with root

            if (u == v) ans ^= 1;  // Self-loop contributes 1 to the Grundy number
            else {
                g[u].push_back(v);  // Add edge to the graph
                g[v].push_back(u);
            }
        }

        ans ^= dfs(root);  // Compute the Grundy number starting from the root

        if (ans) puts("Alice");  // Alice wins if the final Grundy number is non-zero
        else puts("Bob");  // Bob wins if the Grundy number is zero

        for (int i = 1; i <= n; i++) {
            g[i].clear();  // Clear adjacency list for the next test case
            low[i] = dis[i] = 0;  // Reset discovery and low-link values
        }
    }

    return 0;
}
