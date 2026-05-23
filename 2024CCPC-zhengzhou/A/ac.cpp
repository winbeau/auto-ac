// 2024CCPC-zhengzhou A — Once In My Life
// 算法：数论构造（构造 n·k = P·10^M + R）
// 复杂度：O(T) 时间 / O(1) 空间

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    while (T--) {
        ll n; int d;
        cin >> n >> d;

        // 构造 10 位幸运前缀 P：取数字 1..9 各一个，再额外放一个 d，升序排列。
        int digs[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, d};
        sort(digs, digs + 10);
        ll P = 0;
        for (int x : digs) P = P * 10 + x;

        // M = 满足 10^M >= n 的最小非负整数，使得余数 R (< n) 可填入末 M 位而不影响 P 的数位。
        ll pow10 = 1;
        while (pow10 < n) pow10 *= 10;

        // n*k = P * 10^M + R, 其中 R = (-P * 10^M) mod n.
        ll Pmod = P % n;
        ll p10mod = pow10 % n;
        ll rem = (Pmod * p10mod) % n;
        ll R = (n - rem) % n;

        ll nk = P * pow10 + R;
        ll k = nk / n;

        cout << k << '\n';
    }
    return 0;
}
