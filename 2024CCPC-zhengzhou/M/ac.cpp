// 2024CCPC-zhengzhou M — 有效算法
// 算法：二分答案 + 区间交可行性判定
// 复杂度：O((Σn) · log V) 时间 / O(n) 空间，V = 1e9

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    while (T--) {
        int n; cin >> n;
        vector<ll> a(n), b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < n; ++i) cin >> b[i];

        // check(k)：所有区间 [a_i - k*b_i, a_i + k*b_i] 是否有公共点。
        auto check = [&](ll k) {
            ll lo = LLONG_MIN / 2, hi = LLONG_MAX / 2;
            for (int i = 0; i < n; ++i) {
                ll d = k * b[i];           // k <= 2e9, b_i <= 1e9 → d <= 2e18，仍在 ll 内
                lo = max(lo, a[i] - d);
                hi = min(hi, a[i] + d);
                if (lo > hi) return false; // 提前剪枝
            }
            return lo <= hi;
        };

        // 二分最小可行 k。上界用 2e9 足够覆盖 a 差 <= 1e9、b_min = 1 的最坏情形。
        ll lo = 0, hi = 2'000'000'000LL;
        while (lo < hi) {
            ll mid = lo + (hi - lo) / 2;
            if (check(mid)) hi = mid;
            else lo = mid + 1;
        }
        cout << lo << '\n';
    }
    return 0;
}
