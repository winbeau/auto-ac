// 2024CCPC-zhengzhou C — 中二病也要打比赛
// 算法：区间合并 + 分组 LIS（Fenwick 维护前缀最大值）
// 复杂度：O(n log n) 时间 / O(n) 空间

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;

int n;
int a[MAXN];
int lpos[MAXN], rpos[MAXN];
int fen[MAXN];

void fen_update(int i, int val) {
    for (; i <= n; i += i & -i) {
        if (fen[i] < val) fen[i] = val;
    }
}

int fen_query(int i) {
    int res = 0;
    for (; i > 0; i -= i & -i) {
        if (fen[i] > res) res = fen[i];
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 1; i <= n; ++i) cin >> a[i];

    for (int v = 1; v <= n; ++v) { lpos[v] = 0; rpos[v] = 0; }
    for (int i = 1; i <= n; ++i) {
        int v = a[i];
        if (lpos[v] == 0) lpos[v] = i;
        rpos[v] = i;
    }

    int distinct_count = 0;
    for (int v = 1; v <= n; ++v) if (lpos[v] > 0) ++distinct_count;

    vector<int> bucket;
    bucket.reserve(n);
    int cur_right = 0;

    for (int i = 1; i <= n; ++i) {
        int v = a[i];
        if (lpos[v] == i) {
            bucket.push_back(v);
            if (rpos[v] > cur_right) cur_right = rpos[v];
        }
        if (i == cur_right) {
            // close component: batch compute then batch update
            int sz = (int)bucket.size();
            vector<int> new_val(sz);
            for (int k = 0; k < sz; ++k) {
                int u = bucket[k];
                new_val[k] = fen_query(u - 1) + 1;
            }
            for (int k = 0; k < sz; ++k) {
                fen_update(bucket[k], new_val[k]);
            }
            bucket.clear();
            cur_right = 0;
        }
    }

    int max_kept = fen_query(n);
    cout << distinct_count - max_kept << "\n";
    return 0;
}
