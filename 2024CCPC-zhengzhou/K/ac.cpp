// 2024CCPC-zhengzhou K — 树上问题
// 算法：换根 DP
// 复杂度：每组 O(n) 时间 / O(n) 空间

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n + 1);
        for (int i = 1; i <= n; ++i) cin >> a[i];
        vector<vector<int>> adj(n + 1);
        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // 第一遍：迭代 DFS 以 1 为根，求 parent / 拓扑序，并算 bad[1]
        vector<int> parent(n + 1, 0), order;
        order.reserve(n);
        vector<int> stk;
        stk.reserve(n);
        stk.push_back(1);
        parent[1] = 0;
        vector<char> visited(n + 1, 0);
        visited[1] = 1;
        while (!stk.empty()) {
            int u = stk.back();
            stk.pop_back();
            order.push_back(u);
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = 1;
                    parent[v] = u;
                    stk.push_back(v);
                }
            }
        }

        vector<int> bad(n + 1, 0);
        // bad[1]：把所有「父 u → 子 v」方向上违规的边数 cnt 起来
        int total_bad = 0;
        for (int v = 1; v <= n; ++v) {
            int u = parent[v];
            if (u == 0) continue;
            // 父 u → 子 v 合法 iff 2*a[v] >= a[u]
            if (2 * a[v] < a[u]) ++total_bad;
        }
        bad[1] = total_bad;

        // 第二遍：按拓扑序换根
        // 边 (u, v): 原父 u → 子 v；换根后 v 是父、u 是子
        // bad[v] = bad[u] - [2*a[v] < a[u]] + [2*a[u] < a[v]]
        for (int u : order) {
            for (int v : adj[u]) {
                if (v == parent[u]) continue;
                int delta = 0;
                if (2 * a[v] < a[u]) --delta;  // 原本父 u→子 v 违规，换根后这条不再 bad
                if (2 * a[u] < a[v]) ++delta;  // 现在父 v→子 u 是否违规
                bad[v] = bad[u] + delta;
            }
        }

        int ans = 0;
        for (int r = 1; r <= n; ++r) {
            if (bad[r] == 0) ++ans;
        }
        cout << ans << '\n';
    }
    return 0;
}
