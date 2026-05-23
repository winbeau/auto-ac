// 2024CCPC-zhengzhou D — 距离之比
// 算法：坐标变换 (u,v)=(x+y,x-y)；分别按 u 排序和按 v 排序，最优对必在两种排序的相邻对中
// 复杂度：O(n log n) 时间 / O(n) 空间，每组数据

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;

        vector<ll> xs(n), ys(n);
        for (int i = 0; i < n; ++i) {
            cin >> xs[i] >> ys[i];
        }

        // 候选索引序：分别按 u=x+y 和 v=x-y 升序，取相邻对
        vector<int> ord(n);
        iota(ord.begin(), ord.end(), 0);

        auto eval = [&](int i, int j) -> ld {
            ll dx = xs[i] - xs[j];
            ll dy = ys[i] - ys[j];
            ll l1 = (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);
            ld l2 = sqrtl((ld)dx * dx + (ld)dy * dy);
            return l2 > 0 ? (ld)l1 / l2 : (ld)0;
        };

        ld ans = 0.0L;

        // 按 u = x+y 排序，扫相邻对
        sort(ord.begin(), ord.end(), [&](int a, int b) {
            return xs[a] + ys[a] < xs[b] + ys[b];
        });
        for (int i = 0; i + 1 < n; ++i) {
            ld r = eval(ord[i], ord[i + 1]);
            if (r > ans) ans = r;
        }

        // 按 v = x-y 排序，扫相邻对
        sort(ord.begin(), ord.end(), [&](int a, int b) {
            return xs[a] - ys[a] < xs[b] - ys[b];
        });
        for (int i = 0; i + 1 < n; ++i) {
            ld r = eval(ord[i], ord[i + 1]);
            if (r > ans) ans = r;
        }

        cout << fixed << setprecision(12) << (double)ans << '\n';
    }
    return 0;
}
