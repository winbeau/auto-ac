// 2024CCPC-zhengzhou F — 优秀字符串
// 算法：模拟（条件判定）
// 复杂度：O(sum |S_i|) 时间 / O(1) 额外空间

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int ans = 0;
    string s;
    for (int i = 0; i < n; ++i) {
        cin >> s;
        if ((int)s.size() != 5) continue;
        if (s[2] != s[4]) continue;
        // 前四个字符互不相同
        bool ok = true;
        for (int a = 0; a < 4 && ok; ++a) {
            for (int b = a + 1; b < 4 && ok; ++b) {
                if (s[a] == s[b]) ok = false;
            }
        }
        if (ok) ++ans;
    }

    cout << ans << '\n';
    return 0;
}
