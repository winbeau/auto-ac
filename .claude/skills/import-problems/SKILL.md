---
name: import-problems
description: 把一份 CCPC / ICPC / 邀请赛 PDF 提题单导入本仓库，按 docs/ 规范创建比赛目录、重命名 PDF、批量生成 A..M 题目骨架（problem.md / analysis.md / ac.cpp）。触发场景：用户说"整理题目"、"导入题目"、"PDF 转目录"、"导入比赛"、"PDF 转规范结构"、"scaffold contest"、"import problems"，或扔过来一份比赛提题单 PDF 让你按规范归档时。本 skill 只做归档骨架，不解题、不填题面正文。
---

# import-problems

把一份比赛 PDF 转成本仓库规范的 `<contest-id>/{<contest-id>.pdf, A/, B/, …, M/}` 目录结构。

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
- `pdftotext -layout <pdf> /tmp/contest.txt` 抽取文本备用。

### 2. 从文本解析比赛元数据
通常 PDF 首页 + "题目概况" 表给出：

- **年份**（4 位）、**比赛系列**（CCPC / ICPC / CCPC-final / ICPC-EC / ICPC-WF 等）
- **地点**（中文 → 拼音；英文照搬；多词用连字符）
- **题号 / 题名 / 时间限制 / 空间限制** 表

若 PDF 无法解析（扫描件 / 加密），告诉用户并请求贴文本，不要硬猜。

### 3. 规范化比赛目录名
格式：`<年份><系列>-<地点>`，全小写连字符，不含中文 / 空格 / 下划线。例：

- `2024CCPC-zhengzhou`、`2024ICPC-shenyang`、`2023CCPC-final-harbin`

**必须**把候选目录名给用户确认（拼音容易错，比如 xi'an / xian / xi-an）。

### 4. 迁移 PDF
```bash
mkdir -p <contest-id>
git mv <原 PDF 路径> <contest-id>/<contest-id>.pdf
```
PDF 不在 git 跟踪下就用普通 `mv`。

### 5. 批量生成题目骨架
用本 skill 同目录下的 [scaffold.sh](scaffold.sh)：

```bash
# 在 /tmp 写一份 CSV：每行 <letter>|<title>|<time_limit_ms>
cat >/tmp/problems.csv <<'EOF'
A|Once In My Life|1000
B|扫雷 1|1000
…
M|有效算法|2000
EOF

.claude/skills/import-problems/scaffold.sh <contest-id> /tmp/problems.csv
```

脚本会在 `<contest-id>/X/` 下创建 `problem.md` / `analysis.md` / `ac.cpp`，标题、题号、时间限制按 PDF 回填，其它字段写 `TODO`。空间限制默认 `1 GB`；若 PDF 中存在异常项（如部分题 512 MB），手动改对应 problem.md。

### 6. 收尾
- `git status` 列出改动给用户看
- **建议**而非自动 commit；推荐两段：
  1. `chore: scaffold <contest-id> — rename PDF and add A–? problem skeletons`
  2. 后续按题填内容时一题一 commit：`<contest-id>: add problem X (<title>)`
- 不要 `git push`（默认分支保护），等用户决定

## 不要做的事

- 不要尝试解题、不要填写实际题面 / 题解 / AC 代码（这是后续单题工作）
- 不要在本机编译运行 `ac.cpp`（参见 CLAUDE.md）
- 不要修改 `CLAUDE.md` 或 `docs/` 模板规范；如规范不够用，告诉用户去改模板
- 不要创建非规范目录（`solutions/`、`tests/` 等）
