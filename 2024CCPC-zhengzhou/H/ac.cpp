// 2024CCPC-zhengzhou H — 随机栈
// 算法：概率期望 + multiset/map 维护多重集 + 费马小定理求模逆元
// 复杂度：O(n (log n + log MOD)) 时间 / O(n) 空间

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int MOD = 998244353;

ll qpow(ll a, ll b, ll mod) {
    ll res = 1 % mod;
    a %= mod;
    if (a < 0) a += mod;
    while (b > 0) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

ll inv(ll a) { return qpow(a, MOD - 2, MOD); }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    map<int, int> cnt;  // 多重集频次表，key 自动有序
    int size = 0;       // 多重集当前大小
    int last_popped = -1;
    ll ans = 1;
    bool dead = false;

    for (int i = 0; i < 2 * n; ++i) {
        int x;
        cin >> x;
        if (x == -1) {
            if (dead) continue;
            int m = cnt.begin()->first;
            if (m < last_popped) {
                ans = 0;
                dead = true;
                continue;
            }
            int cm = cnt.begin()->second;
            ans = ans * (ll)cm % MOD * inv(size) % MOD;
            last_popped = m;
            if (--cnt.begin()->second == 0) cnt.erase(cnt.begin());
            --size;
        } else {
            ++cnt[x];
            ++size;
        }
    }

    cout << ans << '\n';
    return 0;
}
