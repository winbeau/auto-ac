// 2024CCPC-zhengzhou E — 保卫城邦
// 算法：树形 DP（best-effort：每次操作后整树重算）
// 复杂度：O((n + m) * n) 时间 / O(n) 空间   ——大数据范围下会 TLE，需替换为 LCT / Top Tree 维护动态 DP

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = (ll)1e18;

int n, m;
vector<int> adj[200005];

// f0[v]: v 未驻军，父提供 2，子树最小代价（无需子节点贡献）
// f1[v]: v 未驻军，父提供 1，需子节点贡献和 >= 1
// f2[v]: v 未驻军，父提供 0，需子节点贡献和 >= 2
// g1[v]: v 驻 1 支，父提供 1
// g2[v]: v 驻 2 支，父提供 2
ll f0[200005], f1[200005], f2[200005], g1[200005], g2[200005];

int parent_of[200005];
int order_arr[200005];

void build_order(int root) {
    // BFS / iterative DFS 得到拓扑序，避免递归爆栈
    int head = 0, tail = 0;
    order_arr[tail++] = root;
    parent_of[root] = 0;
    while (head < tail) {
        int u = order_arr[head++];
        for (int v : adj[u]) {
            if (v != parent_of[u]) {
                parent_of[v] = u;
                order_arr[tail++] = v;
            }
        }
    }
}

void compute_dp() {
    // 倒序处理（先叶子）
    for (int i = n - 1; i >= 0; i--) {
        int u = order_arr[i];
        int par = parent_of[u];

        // 子节点贡献
        ll s_g1 = 0, s_g2 = 0, s_f0 = 0;
        for (int c : adj[u]) {
            if (c == par) continue;
            s_g1 += min({f1[c], g1[c], g2[c]});
            s_g2 += min({f0[c], g1[c], g2[c]});
            s_f0 += min({f2[c], g1[c], g2[c]});
        }
        g1[u] = 1 + s_g1;
        g2[u] = 2 + s_g2;
        f0[u] = s_f0;

        // f1[u]: 父 = 1，需子贡献和 >= 1
        {
            bool has_child = false;
            bool has_contributor = false;
            ll s = 0;
            ll best_upgrade = INF;
            for (int c : adj[u]) {
                if (c == par) continue;
                has_child = true;
                ll b = f1[c];                       // contrib 0
                ll a = min(g1[c], g2[c]);           // contrib >= 1
                ll mn = min(a, b);
                s += mn;
                if (a <= b) has_contributor = true;
                if (a < INF / 2 && b < INF / 2)
                    best_upgrade = min(best_upgrade, a - b);
                else if (a < INF / 2)
                    best_upgrade = min(best_upgrade, a);
            }
            if (!has_child) {
                f1[u] = INF;
            } else if (has_contributor) {
                f1[u] = s;
            } else if (best_upgrade >= INF / 2) {
                f1[u] = INF;
            } else {
                f1[u] = s + best_upgrade;
            }
        }

        // f2[u]: 父 = 0，需子贡献和 >= 2
        {
            ll dp[3] = {0, INF, INF};
            bool has_child = false;
            for (int c : adj[u]) {
                if (c == par) continue;
                has_child = true;
                ll b = f2[c];   // contrib 0
                ll a = g1[c];   // contrib 1
                ll d = g2[c];   // contrib 2
                ll nd[3] = {INF, INF, INF};
                for (int k = 0; k < 3; k++) {
                    if (dp[k] >= INF / 2) continue;
                    if (b < INF / 2)
                        nd[k] = min(nd[k], dp[k] + b);
                    int nk = min(2, k + 1);
                    if (a < INF / 2)
                        nd[nk] = min(nd[nk], dp[k] + a);
                    int nk2 = min(2, k + 2);
                    if (d < INF / 2)
                        nd[nk2] = min(nd[nk2], dp[k] + d);
                }
                dp[0] = nd[0]; dp[1] = nd[1]; dp[2] = nd[2];
            }
            f2[u] = has_child ? dp[2] : INF;
        }
    }
}

ll solve() {
    build_order(1);
    compute_dp();
    // 根节点没有父，等价于父贡献 = 0：
    // root 未驻军 -> f2[1]; 驻 1 -> g1[1]; 驻 2 -> g2[1]
    return min({f2[1], g1[1], g2[1]});
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    vector<multiset<int>> ms(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        ms[u].insert(v);
        ms[v].insert(u);
    }
    while (m--) {
        int u, v, a, b;
        cin >> u >> v >> a >> b;
        ms[u].erase(ms[u].find(v));
        ms[v].erase(ms[v].find(u));
        ms[a].insert(b);
        ms[b].insert(a);
        for (int i = 1; i <= n; i++) {
            adj[i].clear();
            for (int x : ms[i]) adj[i].push_back(x);
        }
        cout << solve() << '\n';
    }
    return 0;
}
