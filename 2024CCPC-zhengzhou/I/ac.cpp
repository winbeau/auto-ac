// 2024CCPC-zhengzhou I — 378QAQ 和字符串
// 算法：字符串周期 + 倍数性质（只枚举 p ∈ (n/4, n/2]）+ 多项式哈希 LCP 跳过相等段
// 复杂度：O(n · k · log n) 时间 / O(n) 空间（每个测试点）

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

// 双哈希：mod1 = 2^61 - 1（Mersenne），mod2 = 1e9 + 7
// 这里采用两套不同模/底的多项式哈希，避免极端构造导致的碰撞

static const ull MOD61 = (1ULL << 61) - 1;
static const ull BASE1 = 131; // 任意合理底

static inline ull mod_mul(ull a, ull b) {
    __uint128_t c = (__uint128_t)a * b;
    ull hi = (ull)(c >> 61), lo = (ull)(c & MOD61);
    ull res = hi + lo;
    if (res >= MOD61) res -= MOD61;
    return res;
}

static inline ull mod_add(ull a, ull b) {
    a += b;
    if (a >= MOD61) a -= MOD61;
    return a;
}

static inline ull mod_sub(ull a, ull b) {
    return a >= b ? a - b : a + MOD61 - b;
}

static const int MOD2 = 1'000'000'007;
static const int BASE2 = 137;

vector<ull> h1, pw1;
vector<int> h2, pw2;

void build_hash(const string& s) {
    int n = (int)s.size();
    h1.assign(n + 1, 0);
    pw1.assign(n + 1, 1);
    h2.assign(n + 1, 0);
    pw2.assign(n + 1, 1);
    for (int i = 0; i < n; ++i) {
        h1[i + 1] = mod_add(mod_mul(h1[i], BASE1), (ull)(s[i] - 'a' + 1));
        pw1[i + 1] = mod_mul(pw1[i], BASE1);
        h2[i + 1] = (int)(((ll)h2[i] * BASE2 + (s[i] - 'a' + 1)) % MOD2);
        pw2[i + 1] = (int)(((ll)pw2[i] * BASE2) % MOD2);
    }
}

// 返回 s[l..l+len-1] 的双哈希值（pair）
static inline pair<ull, int> get_hash(int l, int len) {
    ull v1 = mod_sub(h1[l + len], mod_mul(h1[l], pw1[len]));
    int v2 = (int)(((ll)h2[l + len] - (ll)h2[l] * pw2[len] % MOD2 + (ll)MOD2 * MOD2) % MOD2);
    return {v1, v2};
}

static inline bool eq_substr(int a, int b, int len) {
    return get_hash(a, len) == get_hash(b, len);
}

// 计算 s[a..] 与 s[b..] 的最长公共前缀长度（受限于 n - max(a,b)）
int lcp(int a, int b, int n) {
    int maxLen = n - max(a, b);
    if (maxLen <= 0) return 0;
    int lo = 0, hi = maxLen;
    while (lo < hi) {
        int mid = (lo + hi + 1) >> 1;
        if (eq_substr(a, b, mid)) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

// 计算组大小 sz∈{2,3,4} 时的多数元素出现次数
static inline int majority_count(const char* c, int sz) {
    if (sz == 2) {
        return (c[0] == c[1]) ? 2 : 1;
    }
    if (sz == 3) {
        bool e01 = (c[0] == c[1]);
        bool e12 = (c[1] == c[2]);
        bool e02 = (c[0] == c[2]);
        if (e01 && e12) return 3;
        if (e01 || e12 || e02) return 2;
        return 1;
    }
    // sz == 4
    int cnt[4] = {1, 1, 1, 1};
    for (int i = 0; i < 4; ++i)
        for (int j = i + 1; j < 4; ++j)
            if (c[i] == c[j]) { ++cnt[i]; ++cnt[j]; }
    return max({cnt[0], cnt[1], cnt[2], cnt[3]});
}

void solve() {
    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;

    // 边界：n = 2 时唯一的 p = 1；通用算法仍能处理。
    build_hash(s);

    bool found = false;
    int p_start = n / 4 + 1; // 只需枚举 p ∈ (n/4, n/2]：若更小的 p 可行，其倍数（落在该区间内）也可行
    int p_end = n / 2;

    // 临时缓冲
    vector<int> mismatch_pos;
    mismatch_pos.reserve(2 * k + 4);
    vector<int> aff_r;
    aff_r.reserve(2 * k + 4);

    for (int p = p_start; p <= p_end && !found; ++p) {
        // 第一阶段：用 LCP 跳过相等段，统计相邻失配数 diff
        // 关键不等式：cost(p) >= ceil(diff/2)，故若 diff > 2k 必有 cost > k，可剪枝
        mismatch_pos.clear();
        int diff = 0;
        int i = p;
        bool prune = false;
        while (i < n) {
            int L = lcp(i, i - p, n);
            i += L;
            if (i >= n) break;
            // s[i] != s[i-p]
            mismatch_pos.push_back(i);
            ++diff;
            if (diff > 2 * k) { prune = true; break; }
            ++i;
        }
        if (prune) continue;
        if (diff == 0) { found = true; break; } // 已经是周期 p

        // 第二阶段：仅对失配位置所在的余数类计算精确 cost
        aff_r.clear();
        for (int pos : mismatch_pos) aff_r.push_back(pos % p);
        sort(aff_r.begin(), aff_r.end());
        aff_r.erase(unique(aff_r.begin(), aff_r.end()), aff_r.end());

        ll cost = 0;
        for (int r : aff_r) {
            char chars[5];
            int sz = 0;
            for (int j = r; j < n; j += p) chars[sz++] = s[j];
            int mx = majority_count(chars, sz);
            cost += sz - mx;
            if (cost > k) break;
        }
        if (cost <= k) { found = true; break; }
    }

    cout << (found ? "Yes" : "No") << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) solve();
    return 0;
}
