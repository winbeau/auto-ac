# auto-ac — CCPC/ICPC 题目仓库

本仓库归档 CCPC / ICPC 邀请赛真题。每场比赛保存：原版提题单 PDF、按题号组织的题面、自写或整理的题解、AC 代码（C++）。

详细规范见 [docs/](docs/)：

- 题面模板：[docs/problem-template.md](docs/problem-template.md)
- 题解模板：[docs/analysis-template.md](docs/analysis-template.md)
- AC 代码风格：[docs/code-style.md](docs/code-style.md)

## 目录结构

```
auto-ac/
├── CLAUDE.md                          # 本文件，仓库总索引
├── docs/                              # 规范与模板
│   ├── problem-template.md
│   ├── analysis-template.md
│   └── code-style.md
├── 2024CCPC-zhengzhou/                # 一场比赛 = 一个目录
│   ├── 2024CCPC-zhengzhou.pdf         # 规范命名后的原始提题单
│   ├── A/
│   │   ├── problem.md                 # 题目内容
│   │   ├── analysis.md                # 题目分析
│   │   └── ac.cpp                     # AC 代码
│   ├── B/
│   ├── …
│   └── M/                             # 视实际题量而定
└── 2024ICPC-shenyang/                 # 同上
```

## 比赛目录命名

格式：`<年份><比赛系列>-<地名拼音或英文>`，全部小写连字符，不允许中文 / 空格 / 下划线。

- 年份：4 位数字，比赛举办年份
- 比赛系列：`CCPC` / `ICPC` / `CCPC-final` / `ICPC-EC` / `ICPC-WF` 等
- 地名：拼音或英文（`zhengzhou`、`shenyang`、`hangzhou`、`xian`…），多词用连字符

示例：

- `2024CCPC-zhengzhou`
- `2024ICPC-shenyang`
- `2023CCPC-final-harbin`

每个比赛目录下放一份与目录同名的 PDF：`<目录名>.pdf`。仓库当前的 `2024CCPC郑州.pdf` 后续应按此规范重命名并移入 `2024CCPC-zhengzhou/`。

## 题目子目录命名

- 题号用单个大写字母：`A`、`B`、`C`、…、`M`，按实际题数创建，不补空目录
- 每个子目录固定三个文件：

| 文件 | 内容 | 模板 |
|---|---|---|
| `problem.md` | 题面（中文整理或英文原文 + 翻译） | [docs/problem-template.md](docs/problem-template.md) |
| `analysis.md` | 题解（思路 + 复杂度 + 易错点） | [docs/analysis-template.md](docs/analysis-template.md) |
| `ac.cpp` | 单文件 AC 代码 | [docs/code-style.md](docs/code-style.md) |

- 题面 / 题解 / 代码任一项暂未补全时，仍要建好对应文件并在开头标注 `TODO:`，便于追踪进度
- 题面若含图，统一放到 `X/img/` 下，文件名小写连字符（`fig-1.png`）；`problem.md` 用相对路径引用

## AC 代码硬性约束

- 单文件，可直接编译：`g++ -O2 -std=c++17 ac.cpp -o ac`
- 仅依赖标准库；不输出调试信息
- 文件首部注释写明：题目来源、算法标签、复杂度
- 详细风格见 [docs/code-style.md](docs/code-style.md)

## 添加新比赛的流程

1. 拿到原始 PDF 提题单
2. `mkdir <year><series>-<city>`
3. PDF 重命名为 `<目录名>.pdf` 放入
4. 按题数创建 `A/`…`?/`，各放好三个文件（即使只写了 TODO 也要建）
5. `git commit`：粒度可以「一场比赛一次」或「一题一次」

## 添加单题的流程

1. 在对应比赛目录下建 `X/`
2. 从 PDF 抽取或翻译题面 → `X/problem.md`
3. 写题解 → `X/analysis.md`
4. 写 AC 代码 → `X/ac.cpp`，在 OJ 跑过再提交
5. commit message 建议：`2024CCPC-zhengzhou: add problem A (<标题>)`

## 与 Claude 协作约定

- 服务器上 **不跑 OJ 题代码、不跑评测**（参见全局 CLAUDE.md 的资源约束），Claude 改完后由用户在本地编译运行 / 提交 OJ
- 大段 PDF 抽取若超过 Claude 直读能力，用户用 `pdftotext` / OCR 处理后把文本贴回来
- Claude 可以做：
  - 按模板撰写 / 补全 `problem.md`、`analysis.md`、`ac.cpp`
  - 把已有零散笔记结构化进模板
  - Review 思路、复杂度、边界条件
  - 检查目录 / 命名是否符合本文件约定
- Claude 不应擅自：
  - 创建非本文件约定的目录（如 `solutions/`、`tests/`）
  - 在 `ac.cpp` 里留调试输出 / 注释掉的代码
  - 修改已有比赛目录的命名（先和用户确认）
