---
name: import-problems
description: 把一份 CCPC / ICPC / 邀请赛 PDF 提题单导入本仓库，按 docs/ 规范创建比赛目录、重命名 PDF、批量生成题目骨架（problem.md / analysis.md / ac.cpp）。题数 N 由 PDF 解析得到，不固定。题面落盘前必须由 LLM 按规范化 checklist 校对（数学公式 / 断行 / 表格 / 样例 / 特殊符号），不允许直接 dump pdftotext 的原始文本。触发场景：用户说"整理题目"、"导入题目"、"PDF 转目录"、"导入比赛"、"PDF 转规范结构"、"scaffold contest"、"import problems"，或扔过来一份比赛提题单 PDF 让你按规范归档时。
---

# import-problems

把一份比赛 PDF 转成本仓库规范的 `<contest-id>/{<contest-id>.pdf, A/, B/, …, X/}` 目录结构。题数 X 来自 PDF 解析，**不假设固定为 13 / M**。

## 何时使用

用户给一份比赛 PDF 并要求按规范导入归档，例如：

- "把这个 PDF 整理成题目目录"
- "导入这场比赛 / 导入题目"
- "PDF 转目录 / PDF 转规范结构"
- "import problems from xxx.pdf"
- 直接发一份 `<某场比赛>.pdf` 让你建结构

## 前置依赖

- `pdftotext`、`pdfinfo`（poppler-utils）。若 `which pdftotext` 找不到，提示用户在本机安装 `apt-get install poppler-utils`；**不要**在共享 VPS 自行装大包（参见全局 CLAUDE.md）。
- 仓库根存在 `CLAUDE.md` 和 `docs/{problem-template,analysis-template,code-style}.md`。规范以 `docs/` 为准，本 skill 不复述。

## 工作流

### 1. 确定 PDF
- 用户给路径就用该路径；否则 `ls *.pdf`，多于一个时让用户选。
- `pdftotext -layout <pdf> /tmp/contest.txt` 抽取文本备用；同时 `pdfinfo` 看页数。

### 2. 从文本解析比赛元数据
通常 PDF 首页 + "题目概况" 表给出：

- **年份**（4 位）、**比赛系列**（CCPC / ICPC / CCPC-final / ICPC-EC / ICPC-WF 等）
- **地点**（中文 → 拼音；英文照搬；多词用连字符）
- **题目列表**：题号 / 题名 / 时间限制 / 空间限制

**题数 N 严格按 PDF 列出来的来**——可能是 8 / 10 / 11 / 12 / 13 等，邀请赛常见 11~13 题，省赛可能更少。不要默认 13，也不要假设题号一定连续到 M。把解析出的题目列表完整列给用户确认后再继续。

若 PDF 无法解析（扫描件 / 加密 / 排版怪 / OCR 后乱码），告诉用户并请求贴文本，不要硬猜题数和题名。

### 3. 规范化比赛目录名
格式：`<年份><系列>-<地点>`，全小写连字符，不含中文 / 空格 / 下划线。例：

- `2024CCPC-zhengzhou`、`2024ICPC-shenyang`、`2023CCPC-final-harbin`

**必须**把候选目录名给用户确认（拼音容易错，如 xi'an / xian / xi-an）。

### 4. 迁移 PDF
```bash
mkdir -p <contest-id>
git mv <原 PDF 路径> <contest-id>/<contest-id>.pdf
```
PDF 不在 git 跟踪下就用普通 `mv`。

### 5. 批量生成题目骨架
用本 skill 同目录下的 [scaffold.sh](scaffold.sh)。CSV 行数 = 实际题数，不要补到 M：

```bash
cat >/tmp/problems.csv <<'EOF'
A|Once In My Life|1000
B|扫雷 1|1000
…
EOF

.claude/skills/import-problems/scaffold.sh <contest-id> /tmp/problems.csv
```

脚本对 CSV 每一行 `<letter>|<title>|<time_ms>` 生成 `problem.md` / `analysis.md` / `ac.cpp`，标题 / 题号 / 时间限制从 CSV 回填，正文字段写 `TODO`。空间限制默认 `1 GB`；PDF 中有例外（如 512 MB）需手改对应 problem.md。已存在的文件**不会被覆盖**。

### 6. 题面填充 + 规范化（关键步骤，不可省）

scaffold.sh 只生成骨架，**题面正文需要 LLM 在审核后填入**。pdftotext 输出经常脏：公式断成空格、中文被强行换行、表格错位、特殊符号丢失。**不允许把 pdftotext 的原始片段直接 dump 进 problem.md。**

对每一题（或按用户偏好一次填一题 / 一次填全部），流程：

1. 从 `/tmp/contest.txt` 定位该题区段（按 `Problem X.` 或题名锚点）
2. 按 `docs/problem-template.md` 结构整理
3. 按下面 **规范化 checklist** 逐项过一遍
4. 用 `Edit` 工具覆盖 `problem.md` 的 TODO 字段（不是整体重写，保留骨架字段名和顺序）

#### 规范化 checklist

| 类别 | 要做什么 |
|---|---|
| 数学公式 | pdftotext 会把 `10⁵` 输出成 `10 5` / `105`。所有数学量改 KaTeX：`$10^5$`、`$a_i$`、`$\sum_{i=1}^{n}$`、比较符 `\le` `\ge` `\neq` 等 |
| 特殊符号 | `∼ → ⋅ × ÷ ≤ ≥ ≠ ∈ ∉ ⊂ ∪ ∩ ∞` 保留或转 KaTeX；点状省略号 `…` 保留 |
| 中文断行 | pdftotext `-layout` 会按 PDF 行断行，需重排回自然段，**句中不留多余换行** |
| 表格 | 题面里的二维表（如评分表）按 markdown 表格重排；不要保留 pdftotext 的对齐空格残骸 |
| 样例 IO | 从 `standard input` / `standard output` 段抽取，放进 `problem.md` 的"### 样例 N"代码块（语言留空），输入输出严格按 PDF 字符（多空格 / 换行符）保留 |
| 数据范围 | 每条范围一行 `- $1 \le n \le 10^5$` 风格；多组数据题写明 $\sum n$ 上界 |
| 图 | pdftotext 不抽图。若题面引用图（"如图"、"下图所示"），在 problem.md 标注 `图：见原 PDF 第 X 页`，并提示用户后续把图截到 `<contest-id>/<letter>/img/fig-1.png` 再改引用 |
| 标题与编号 | `# X. <题名>` 题号大写字母，与目录一致；不要在 problem.md 里改题名（与 CSV / PDF 保持一致） |
| 多组数据标注 | 题面里"本题测试点包含多组数据"这类强调要保留 |
| 英文原题 | 若题面是英文，**原文照搬到正文，不翻译**；如有官方中文版可在原文下方加 `## 中文翻译` 节 |

#### 自检

落盘前自己问一遍：

- 公式有没有还是裸数字？比如 `n <= 10^5` 应该是 `$n \le 10^5$`
- 段落里有没有不必要的换行？
- 样例和输出是不是分开了，没串到一起？
- 数据范围是不是齐全（n, 各变量, 多组上界）？

任何一条不过关，**不要落盘**，回去整理。

### 7. 收尾
- `git status` 列出改动给用户看
- **建议**而非自动 commit；推荐分段：
  1. `chore: scaffold <contest-id> — rename PDF and add A–? problem skeletons`
  2. 题面填充后：`<contest-id>: fill problem statements for X..Y`（或一题一 commit）
  3. 后续解题：`<contest-id>: add solution for X (<title>)`
- 不要 `git push`（默认分支保护），等用户决定

## 不要做的事

- 不要假设题数 / 题号上限（不要默认 M）；按 PDF 实际题目列表来
- 不要把 pdftotext 的原始文本直接写进 problem.md，必须先按 checklist 规范化
- 不要解题，不要写 `analysis.md` 题解正文或 `ac.cpp` 实际算法（保留 TODO，等用户单独要求）
- 本 skill 范围内不编译运行 `ac.cpp`（骨架是 TODO 不可能 AC）；解题阶段允许 compile + 跑 PDF 样例验证，详见 CLAUDE.md
- 不要修改 `CLAUDE.md` 或 `docs/` 模板规范；规范不够用就告诉用户改模板
- 不要创建非规范目录（`solutions/`、`tests/` 等）
