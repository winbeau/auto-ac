# `ac.cpp` 代码风格

## 编译

```bash
g++ -O2 -std=c++17 ac.cpp -o ac
```

- 必须能用上面这一条命令一次编译通过
- 默认无 warning；如必须抑制，用 `#pragma` 并在旁边注释原因
- 不依赖任何非标准库（`bits/stdc++.h` 允许）

## 文件头

每个 `ac.cpp` 顶部至少三行注释：

```cpp
// 2024CCPC-zhengzhou X — <题目标题>
// 算法：<DP / 图论 / 数据结构 / …>
// 复杂度：O(n log n) 时间 / O(n) 空间
```

可选追加：参考做法来源、提交 OJ 与 RecordID。

## 风格约束

- 单文件，不拆头文件
- `using namespace std;` 允许；也允许 `using ll = long long;` 等常见别名
- IO：`ios::sync_with_stdio(false); cin.tie(nullptr);` 或纯 `scanf/printf`，二选一保持一致，不混用
- 不留任何调试输出（`cout << "dbg"`、`cerr <<`、`fprintf(stderr, …)`、注释掉的 `printf` 全部清掉）
- 全局变量小写下划线（`adj`, `n`, `dp`），常量大写下划线（`MOD`, `MAXN`），类型别名小写（`ll`, `pii`）
- 行宽建议 ≤ 120

## 多组数据 / 模运算 / 范围

- 多组数据：所有全局数组在每组开头显式清零，不要依赖上一组结尾状态
- 模数题：统一 `const int MOD = 998244353;` 或 `1e9 + 7`，并用 `ll` 做中间乘法
- 数据范围 ≥ $2 \cdot 10^9$ 一律 `long long`；位运算时小心 `1LL << k`

## 文件骨架建议

```cpp
// 2024CCPC-zhengzhou X — <题目标题>
// 算法：<…>
// 复杂度：O(…)

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    while (T--) {
        // solve()
    }
    return 0;
}
```

## 反例

下面这些写法 **不允许** 出现在 `ac.cpp`：

- `// cout << x << endl;` 注释掉的调试
- `freopen("in.txt", "r", stdin);` 本地调试用的重定向
- `#define int long long` + `signed main()` 混用却没保持一致
- 把多种解法都留在文件里靠 `#if 0` 切换
