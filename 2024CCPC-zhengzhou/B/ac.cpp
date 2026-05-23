// 2024CCPC-zhengzhou B — 扫雷 1
// 算法：反悔贪心 + 大根堆（同一轮可多次购买）
// 复杂度：O(n log n) 时间 / O(n) 空间

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    priority_queue<ll> heap; // 大根堆：已购探测器的单价
    ll bal = 0;              // 当前余额 = 累计获得币数 - 已购总花费
    int ans = 0;

    for (int i = 1; i <= n; ++i) {
        ll c;
        cin >> c;
        bal += 1; // 第 i 轮开始，先获得 1 枚币

        // 同一轮内可重复购买：能买就买，否则尝试与堆顶最贵者反悔置换
        while (true) {
            if (bal >= c) {
                heap.push(c);
                bal -= c;
                ++ans;
            } else if (!heap.empty() && heap.top() > c) {
                ll top = heap.top();
                heap.pop();
                heap.push(c);
                bal += top - c;
                // 不增加 ans；继续循环看能否再多买一个
            } else {
                break;
            }
        }
    }

    cout << ans << '\n';
    return 0;
}
