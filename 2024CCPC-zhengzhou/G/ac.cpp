// 2024CCPC-zhengzhou G — 扫雷 2
// 算法：构造（L 形外壳归约 + 反对角埋雷 + 小常数特判）
// 复杂度：O(n^2) 每组，总和 O(sum n^2) <= 1e6

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 在工作区左上角埋下 num 个雷：主反对角 + 内反对角，偶数补右下角 (k,k)
static void bury(vector<string>& ans, int num, int k) {
    int len = (num + 1) / 2;
    for (int i = 0; i < len; ++i) {
        ans[i][len - i - 1]++;
    }
    for (int i = 0; i < len - 1; ++i) {
        ans[i][len - i - 2]++;
    }
    if (num % 2 == 0) {
        ans[k][k]++;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<string> ans(n, string(n, '0'));

        if (m <= 2 * n) {
            if (n == 5 && m == 10) {
                // 阶梯三角填法
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < i; ++j) {
                        ans[j][n - i - 1]++;
                    }
                }
            } else {
                bury(ans, m, n - 1);
            }
        } else {
            // L 形外壳归约
            int now = n;
            while (m >= 2 * now + 1 && now) {
                m -= 2 * now - 1;
                now--;
                for (int i = 0; i < now; ++i) {
                    ans[i][now]++;
                    ans[now][i]++;
                }
                ans[now][now]++;
            }
            // 若剩余 m 在 [2, 2*now-2]，先预放 (0,now-1) 与 (now-1,0) 两颗雷
            if (m >= 2 && m < 2 * now - 1) {
                ans[0][now - 1]++;
                ans[now - 1][0]++;
                m -= 2;
            }
            if (m) {
                if (now == 5 && m == 10) {
                    for (int i = 0; i < now; ++i) {
                        for (int j = 0; j < i; ++j) {
                            ans[j][now - i - 1]++;
                        }
                    }
                } else if (now == 3 && m == 1) {
                    ans[now - 1][now - 1]++;
                } else if (now == 4 && m == 8) {
                    bury(ans, m, now - 1);
                    ans[0][0]++;
                    ans[now - 1][now - 1]--;
                } else {
                    // 撤回先前预放的两颗雷（仅在这几种小 case）
                    if ((now == 4 && (m == 3 || m == 4)) ||
                        (now == 3 && m == 2) ||
                        (now == 5 && m == 5)) {
                        ans[0][now - 1]--;
                        ans[now - 1][0]--;
                        if (now == 4 && m == 4) {
                            ans[now - 1][now - 1]--;
                            ans[0][0]++;
                        }
                        m += 2;
                    }
                    if (now >= 5) {
                        if (now - (m + 1) / 2 == 2) {
                            ans[now - 2][now - 1]++;
                            ans[now - 1][now - 2]++;
                            m -= 2;
                        }
                        if ((m & 1) && !(now == 5 && m == 9)) {
                            m += 1;
                            ans[n - 1][n - 1]--;
                        }
                    }
                    if (m) {
                        bury(ans, m, now - 1);
                    }
                }
            } else if (now == 3 || now == 2) {
                // 残留两颗预放雷需要重摆
                ans[0][now - 1]--;
                ans[now - 1][0]--;
                ans[1][1]++;
                ans[(now == 3) ? 2 : 0][(now == 3) ? 2 : 0]++;
            }
        }

        cout << "Yes" << '\n';
        for (int i = 0; i < n; ++i) {
            cout << ans[i] << '\n';
        }
    }
    return 0;
}
