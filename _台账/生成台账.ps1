# 合并各抓取分片，生成题目台账 + 标签/难度统计
# 用法: pwsh -File _台账\生成台账.ps1
$ErrorActionPreference = 'Continue'
$root = Split-Path -Parent $PSScriptRoot   # 库根目录
$dir  = $PSScriptRoot

$ids = @{}
Get-Content (Join-Path $dir 'IDs_P.txt')     -Encoding UTF8 | Where-Object { $_ } | ForEach-Object { $ids[$_.Trim()] = 'P' }
Get-Content (Join-Path $dir 'IDs_other.txt') -Encoding UTF8 | Where-Object { $_ } | ForEach-Object { $ids[$_.Trim()] = 'other' }

$rows = @{}
Get-ChildItem $dir -File -Filter 'shard_*.csv' | ForEach-Object {
    Import-Csv $_.FullName -Encoding UTF8 | ForEach-Object {
        if ($_.id) { $rows[$_.id.Trim().ToUpper()] = $_ }
    }
}

# 只保留确实在我的题号表里的条目
$final = foreach ($k in $rows.Keys) { if ($ids.ContainsKey($k)) { $rows[$k] } }
$final = $final | Sort-Object { if ($_.id -match '^P(\d+)$') { [int]$matches[1] } else { 999999 } }, id

$out = Join-Path $dir '题目台账.csv'
$final | Select-Object id, difficulty, tags | Export-Csv -Path $out -NoTypeInformation -Encoding UTF8

$total = $ids.Count
$got   = $final.Count
Write-Host ("题号 {0} 个，已查到 {1} 个，缺失 {2} 个" -f $total, $got, ($total - $got))
$miss = $ids.Keys | Where-Object { -not $rows.ContainsKey($_) }
if ($miss) { Write-Host ("缺失: " + (($miss | Sort-Object) -join ' ')) }

Write-Host "`n=== 难度分布 ==="
$final | Group-Object difficulty | Sort-Object Name | ForEach-Object { "{0,-16} {1}" -f $_.Name, $_.Count }

Write-Host "`n=== 算法标签 TOP 60 ==="
$tagCount = @{}
foreach ($r in $final) {
    foreach ($t in ($r.tags -split '[;；,，]')) {
        $t = $t.Trim(); if ($t) { $tagCount[$t] = 1 + ($tagCount[$t] | ForEach-Object { $_ }) }
    }
}
$tagCount.GetEnumerator() | Sort-Object Value -Descending | Select-Object -First 60 |
    ForEach-Object { "{0,5}  {1}" -f $_.Value, $_.Key }
