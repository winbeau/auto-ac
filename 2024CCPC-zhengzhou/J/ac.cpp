// 2024CCPC-zhengzhou J — 排列与合数
// 算法：埃氏筛预处理 + 枚举数位排列
// 复杂度：O(N log log N + T * 120) 时间 / O(N) 空间，N = 1e5

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int MAXN = 100000;
bool is_prime[MAXN + 1];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 埃氏筛：is_prime[i] = true 表示 i 是素数
    fill(is_prime + 2, is_prime + MAXN + 1, true);
    for (int i = 2; (ll)i * i <= MAXN; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= MAXN; j += i) {
                is_prime[j] = false;
            }
        }
    }

    int T;
    cin >> T;
    while (T--) {
        string s;
        cin >> s;
        sort(s.begin(), s.end());
        int ans = -1;
        do {
            if (s[0] == '0') continue; // 跳过前导零
            int v = 0;
            for (char c : s) v = v * 10 + (c - '0');
            if (!is_prime[v]) { // v 在 [10000, 99999]，非素数即合数
                ans = v;
                break;
            }
        } while (next_permutation(s.begin(), s.end()));
        cout << ans << '\n';
    }
    return 0;
}
