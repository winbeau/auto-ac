#!/usr/bin/env bash
#
# scaffold.sh — 批量生成单场比赛 A..? 题目骨架
#
# 用法:
#   scaffold.sh <contest-id> <problems-csv>
#
# <contest-id>     已存在的比赛目录名，如 2024CCPC-zhengzhou
# <problems-csv>   CSV 文件，每行格式: <letter>|<title>|<time_limit_ms>
#                  例:
#                    A|Once In My Life|1000
#                    B|扫雷 1|1000
#                    C|中二病也要打比赛|2000
#
# 行为:
#   对每行 <letter>，在 <contest-id>/<letter>/ 下创建:
#     problem.md   — 按 docs/problem-template.md 结构，标题/题号/时间限制回填，其余 TODO
#     analysis.md  — 按 docs/analysis-template.md 结构，全部 TODO
#     ac.cpp       — 按 docs/code-style.md 头注释 + 最小 main 骨架
#
# 已存在的文件 **不会** 被覆盖（避免覆盖用户已写好的内容）。
#
# 不做:
#   - 不创建比赛目录本身（外层先 mkdir + git mv PDF）
#   - 不 git add / commit
#   - 不编译 ac.cpp

set -euo pipefail

if [[ $# -ne 2 ]]; then
  echo "usage: $0 <contest-id> <problems-csv>" >&2
  exit 1
fi

contest_id="$1"
csv="$2"

if [[ ! -d "$contest_id" ]]; then
  echo "error: contest dir '$contest_id' does not exist" >&2
  echo "  先 mkdir -p '$contest_id' 并 git mv 把 PDF 放进去" >&2
  exit 1
fi
if [[ ! -f "$csv" ]]; then
  echo "error: csv '$csv' not found" >&2
  exit 1
fi

created=0
skipped=0

write_if_absent() {
  local path="$1"
  if [[ -e "$path" ]]; then
    skipped=$((skipped + 1))
    return 1
  fi
  return 0
}

while IFS='|' read -r letter title tl; do
  # 跳过空行 / 注释
  [[ -z "${letter// }" ]] && continue
  [[ "${letter:0:1}" == "#" ]] && continue

  letter="${letter// /}"
  if ! [[ "$letter" =~ ^[A-Z]$ ]]; then
    echo "warn: skipping invalid letter '$letter' (expected single uppercase A-Z)" >&2
    continue
  fi
  if ! [[ "$tl" =~ ^[0-9]+$ ]]; then
    echo "warn: skipping '$letter' — invalid time limit '$tl' (expected integer ms)" >&2
    continue
  fi

  dir="$contest_id/$letter"
  mkdir -p "$dir"

  if write_if_absent "$dir/problem.md"; then
    cat >"$dir/problem.md" <<EOF
# $letter. $title

## 题目来源
$contest_id · $letter

## 限制
- 时间限制：${tl} ms
- 空间限制：1 GB

## 题目描述
TODO: 从 PDF 抽取 / 整理题面

## 输入格式
TODO

## 输出格式
TODO

## 样例

### 样例 1
输入：
\`\`\`
TODO
\`\`\`
输出：
\`\`\`
TODO
\`\`\`

## 数据范围
TODO

## 备注
-
EOF
    created=$((created + 1))
  fi

  if write_if_absent "$dir/analysis.md"; then
    cat >"$dir/analysis.md" <<EOF
# $letter. $title — 题解

## 算法标签
TODO

## 关键观察
- TODO

## 思路
TODO

## 实现要点
- TODO

## 复杂度
- 时间：TODO
- 空间：TODO

## 易错点
- TODO

## 参考
-
EOF
    created=$((created + 1))
  fi

  if write_if_absent "$dir/ac.cpp"; then
    cat >"$dir/ac.cpp" <<EOF
// $contest_id $letter — $title
// 算法：TODO
// 复杂度：TODO

// TODO: AC 代码

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // TODO
    return 0;
}
EOF
    created=$((created + 1))
  fi
done < "$csv"

echo "done: $created created, $skipped skipped (already existed)"
