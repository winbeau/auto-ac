# D. 距离之比 — 题解

## 算法标签
计算几何、坐标变换（曼哈顿 → 切比雪夫）、排序 + 相邻对

## 关键观察
- 观察 1：所求比值
  $f(\Delta x, \Delta y) = \dfrac{|\Delta x| + |\Delta y|}{\sqrt{\Delta x^2 + \Delta y^2}}$
  只依赖差向量 $(\Delta x, \Delta y)$ 的**方向**（分子分母同 1 次齐次）。值域 $[1, \sqrt 2]$，在 $|\Delta x| = |\Delta y|$（与 $\pm 45^\circ$ 对齐）时取到最大 $\sqrt 2$。
- 观察 2：恒等式 $|\Delta x| + |\Delta y| = \max(|\Delta x + \Delta y|,\ |\Delta x - \Delta y|)$。令 $u = x + y$、$v = x - y$（Manhattan → Chebyshev 标准变换），则
  $f = \max\!\left(\dfrac{|\Delta u|}{\lVert P_iP_j\rVert_2},\ \dfrac{|\Delta v|}{\lVert P_iP_j\rVert_2}\right).$
  即：在 $(u, v)$ 平面中寻找一对点，使其连线**尽量水平**或**尽量竖直**（即贴近某条坐标轴方向）。
- 观察 3（核心）：**最优点对在 $(u, v)$ 平面上，按 $u$ 排序或按 $v$ 排序时一定有相邻关系。**

  证明（按 $v$ 排序的方向）：WLOG 设最优对 $(a, c)$ 满足 $|\Delta u| \ge |\Delta v|$，即在最小化 $s = |\Delta v|/|\Delta u|$。若存在第三点 $b$ 使 $v_b$ 严格夹在 $v_a, v_c$ 之间：
  - 若 $u_b$ 也夹在 $u_a, u_c$ 之间，则 $\Delta v_{ac} = \Delta v_{ab} + \Delta v_{bc}$、$\Delta u_{ac} = \Delta u_{ab} + \Delta u_{bc}$（同号），$s_{ac}$ 是 $s_{ab}, s_{bc}$ 的正权平均，故 $\min(s_{ab}, s_{bc}) \le s_{ac}$，可用 $(a, b)$ 或 $(b, c)$ 替换 $(a, c)$ 而比值不变差（且 $|\Delta v|$ 严格变小）。
  - 若 $u_b$ 不在 $u_a, u_c$ 之间，则 $(a, b)$ 或 $(b, c)$ 中至少一个 $|\Delta u|$ 严格变大、$|\Delta v|$ 严格变小，$s$ 严格更小——与 $(a,c)$ 最优矛盾。

  归纳：每次替换 $|\Delta v|$ 严格下降，有限步后达到 $v$-相邻对。对称地，若 $|\Delta v| \ge |\Delta u|$，则最优对在 $u$-排序中相邻。

  反例提醒：**仅** 检查 4 个极值点 $\{u_{\max}, u_{\min}, v_{\max}, v_{\min}\}$ 之间两两的比值是不够的（例如 P1=(0,5), P2=(100,95), P3=(50,50), P4=(51,51)，最优对是 P3-P4，两端都不是极值）。

## 思路
1. 把每个点映射成 $(u_i, v_i) = (x_i + y_i, x_i - y_i)$。
2. 按 $u$ 升序排序，扫相邻对，在原 $(x, y)$ 空间算真实比值，更新答案。
3. 按 $v$ 升序排序，再扫一遍相邻对。
4. 输出 12 位小数。

## 实现要点
- 两次 `sort`，无需凸包 / 平衡树。
- 坐标范围：$|x|, |y| \le 10^9$，$|\Delta x|, |\Delta y| \le 2 \cdot 10^9$，$\Delta x^2 + \Delta y^2 \le 8 \cdot 10^{18}$，仍在 `long long` 范围内。
- 用 `long double` 算 `sqrtl`；输出 `fixed << setprecision(12)`，满足 $10^{-9}$ 精度。
- 多组数据：每组开头清空 `vector`。
- $n \ge 2$ 且点互不相同，欧氏距离恒 $> 0$，无需特判 0。

## 复杂度
- 时间：每组 $O(n \log n)$；总 $O(\sum n \log n)$，约 $4 \times 10^6$。
- 空间：$O(n)$。

## 易错点
- 用 `int` 算 $\Delta x^2 + \Delta y^2$ 会溢出，必须 `long long`。
- 只检查 $u$-相邻对（漏 $v$-相邻）会 WA。
- 仅枚举 4 个极值点会 WA（如上反例）。
- 输出位数太少（< 10 位）会被 $10^{-9}$ 精度卡。

## 参考
- 经典转化：Manhattan ↔ Chebyshev，$(x, y) \mapsto (x+y, x-y)$。
- "最小斜率点对" 标准技巧（min/max slope 出现在某种排序的相邻对中）。
