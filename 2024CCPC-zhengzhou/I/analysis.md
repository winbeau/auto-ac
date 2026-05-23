# I. 378QAQ 和字符串 — 题解

## 算法标签
字符串、周期、多项式哈希、LCP、剪枝

## 关键观察

- **「美丽」等价于：存在周期 $p \in [1, n/2]$，使得每个余数类 $\{r, r+p, r+2p, \dots\}$ 内的字符全部相同。** 把位置按 $i \bmod p$ 分组，每组内字符必须统一为同一个字符，**最少修改次数** $\mathrm{cost}(p) = \sum_{\text{组 } g} \bigl(|g| - \max_c \mathrm{cnt}(c, g)\bigr)$。
- **倍数性质（关键剪枝）：若修改后的 $s'$ 具有周期 $p_0$，则 $s'$ 也具有周期 $m \cdot p_0$（任意正整数 $m$）。** 因此 $\mathrm{cost}(m p_0) \le \mathrm{cost}(p_0)$。
- **只需枚举 $p \in (n/4, n/2]$。** 对任意 $p_0 \le n/4$，区间 $(n/(4 p_0), n/(2 p_0)]$ 长度 $\ge 1$ 必含整数 $m$，使 $m p_0 \in (n/4, n/2]$ 仍是合法周期，并且其 $\mathrm{cost}$ 不超过 $\mathrm{cost}(p_0)$。
- 在该区间内，每个余数类的大小 $\lceil (n-r)/p \rceil \in \{2, 3, 4\}$（因为 $n/p < 4$ 且 $n/p \ge 2$）。
- **失配数下界：** 设 $\mathrm{diff}(p) = |\{i : s_i \ne s_{i+p}\}|$。把每个组看作一条链 $c_0 c_1 \cdots$，每次修改一个位置最多消除 $2$ 条相邻失配边，因此 $\mathrm{cost}(p) \ge \lceil \mathrm{diff}(p)/2 \rceil$。**故 $\mathrm{diff}(p) > 2k$ 时直接剪枝。**

## 思路

1. **缩小枚举范围**：只考虑 $p \in (n/4, n/2]$（C++ 写法 `p_start = n/4 + 1`）。
2. **快速判失配数（剪枝阶段）**：对每个 $p$，从 $i = p$ 起用**多项式哈希 + 二分**求 $\mathrm{lcp}(s_{i..}, s_{i-p..})$，跳过相等段，每遇到一次失配 `diff++`、`i++` 继续。一旦 $\mathrm{diff} > 2k$ 立即 `break`。
3. **精确判 $\mathrm{cost}$（验证阶段）**：若 $\mathrm{diff} \le 2k$，只需处理被失配位置标记的余数类。把所有失配位置的 $r = i \bmod p$ 去重后，对每个受影响的余数类（最多 $2k$ 个）直接收集 $\le 4$ 个字符，求多数元素出现次数 $\mathrm{mx}$，累加 $\mathrm{cost} \mathrel{+}= \mathrm{sz} - \mathrm{mx}$，一旦超过 $k$ 立即终止。
4. 一旦某个 $p$ 的 $\mathrm{cost} \le k$，输出 `Yes`；全部失败输出 `No`。

## 实现要点

- **哈希**：使用双哈希 —— 一份模 $2^{61} - 1$（Mersenne 素数，用 `__uint128_t` 做 $\mod$）+ 一份模 $10^9 + 7$，杜绝对抗数据下的碰撞。
- **LCP 二分**：在区间 $[0, n - \max(a, b)]$ 上二分长度 $L$，每次比较 $s_{a..a+L-1}$ 与 $s_{b..b+L-1}$ 的哈希；时间 $O(\log n)$。
- **多数元素**：组大小固定 $\le 4$，写死 `sz == 2 / 3 / 4` 三种分支，$O(1)$ 出多数；无需 $26$ 桶计数。
- **多组数据**：每组开头重新 `build_hash`，复用 `vector` 缓冲（`mismatch_pos`、`aff_r`）。
- **边界**：$n = 2$ 时 `p_start = 1, p_end = 1`，循环正常进入 $p = 1$。
- **倍数性质的等号**：当 $p_0 \le n/4$ 严格成立时一定有 $m \ge 2$，所以 $p_0$ 自身可能落在 $(n/4, n/2]$ 之外，但其倍数会落在内；不会漏判。

## 复杂度

记 $N = \sum n$。

- 每个 $p$ 的 LCP 跳跃阶段：因 $\mathrm{diff}$ 超过 $2k$ 即剪枝，最多做 $O(k)$ 次失配 + LCP 二分，单次 $O(\log n)$，总 $O(k \log n)$。
- 每个 $p$ 的精确 $\mathrm{cost}$ 验证：受影响余数类 $\le 2k$ 个，每个 $O(1)$，共 $O(k)$。
- $p$ 的枚举范围长度 $\le n/4$。

单组数据：$O\bigl((n/4) \cdot k \log n\bigr) = O(n k \log n)$。

整个测试点：$O(N k \log N) \approx 3 \times 10^5 \times 100 \times 18 \approx 5.4 \times 10^8$ 次哈希常数运算，实测在 $4$ s 内可过。空间 $O(n)$。

## 易错点

- **必须把 $p$ 的枚举下界做成 $n/4 + 1$**：若从 $1$ 开始枚举会退化到 $O(n^2)$，TLE。
- **`diff` 与 `cost` 的关系是 $\mathrm{cost} \ge \lceil \mathrm{diff}/2 \rceil$ 而非 $\mathrm{cost} \ge \mathrm{diff}$**。所以剪枝阈值是 $2k$ 而不是 $k$。
- **单哈希不够安全**：构造对抗数据可造成碰撞导致 LCP 偏长，从而漏掉失配、错误输出 `Yes`。务必双哈希。
- **`build_hash` 必须每组重置**（包括 `pw1/pw2` 因长度变化）。
- **不要早期就把 $p$ 的所有余数类全部遍历**：对没有任何失配的余数类（占多数）应该直接跳过，否则常数巨大。
- 别把 `p` 的上界写成 `n/2 - 1`：原题要求 $p \le n/2$（取等亦合法）。

## 参考

- 官方题解：暂未公开；本解法基于「倍数性质 + 哈希 LCP + 失配剪枝」推导。
- 网络题解：[2024 CCPC 郑州 — sleeeeeping](https://www.cnblogs.com/sleeeeeping/p/18195311) 提到「枚举 $p \in (n/4, n/2]$、跳过 LCP」的同样思路。
