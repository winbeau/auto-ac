// 2024CCPC-zhengzhou L — Toxel 与 PCPC II
// 算法：DP + 段长上界剪枝（每段 bug 数 <= 22）
// 复杂度：O(m * K) 时间 / O(m) 空间，K = 30

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> a(m + 1);
    for (int i = 1; i <= m; ++i) cin >> a[i];

    // dp[j] = 修完前 j 个 bug 的最小总耗时
    const ll INF = (ll)4e18;
    vector<ll> dp(m + 1, INF);
    dp[0] = 0;

    // 段长上界：任意 x >= 22 的段都可被拆成两半得到更优解，所以最优分段中每段 <= 22。
    // 取 K = 30 留出余量。
    const int K = 30;

    for (int j = 1; j <= m; ++j) {
        ll best = INF;
        int lo = max(0, j - K);
        for (int k = lo; k < j; ++k) {
            ll d = j - k;
            ll d2 = d * d;
            ll d4 = d2 * d2;
            ll cand = dp[k] + d4;
            if (cand < best) best = cand;
        }
        dp[j] = best + a[j];
    }

    cout << dp[m] << '\n';
    return 0;
}
