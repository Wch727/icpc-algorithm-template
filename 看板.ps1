# 看板：一条命令看清所有进度
# 用法: pwsh -File .\看板.ps1          # 看一次
#       pwsh -File .\看板.ps1 -Watch   # 每 10 秒刷新，Ctrl+C 退出
param([switch]$Watch)

$ws  = $PSScriptRoot
$log = Join-Path $ws '_台账\codex运行日志.txt'
$dash= Join-Path $ws '控制台.md'
$cmd = Join-Path $ws '_台账\指令.md'

function Show-All {
    Clear-Host
    Write-Host ("=" * 78) -ForegroundColor DarkGray
    Write-Host " 进度看板   $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')" -ForegroundColor Cyan
    Write-Host ("=" * 78) -ForegroundColor DarkGray

    # 1. 主控写的看板
    if (Test-Path $dash) {
        Write-Host "`n[ 主控状态 · 控制台.md ]" -ForegroundColor Yellow
        Get-Content $dash -Encoding UTF8 | Select-Object -First 40 | ForEach-Object { "  $_" }
    }

    # 2. 用户写给主控的指令
    if (Test-Path $cmd) {
        $pending = (Get-Content $cmd -Encoding UTF8 | Where-Object { $_ -match '^\s*[-*]\s*\[\s*\]' })
        Write-Host "`n[ 待办指令 · _台账/指令.md ]" -ForegroundColor Magenta
        if ($pending) { $pending | ForEach-Object { "  $_" } } else { "  （暂无未勾选指令）" }
    }

    # 3. codex 实时日志
    Write-Host "`n[ Codex 实时输出 · _台账/codex运行日志.txt ]" -ForegroundColor Green
    if (Test-Path $log) {
        Write-Host ("  日志大小 {0} KB，最近 15 行：" -f [math]::Round((Get-Item $log).Length / 1KB))
        Get-Content $log -Encoding UTF8 -Tail 15 | ForEach-Object { "  │ $_" }
    } else { "  （还没有日志）" }

    # 4. 库规模
    Write-Host "`n[ 模板库规模 ]" -ForegroundColor Yellow
    $dirs = Get-ChildItem $ws -Directory | Where-Object { $_.Name -match '^\d\d-' } | Sort-Object Name
    foreach ($d in $dirs) { "  {0,-16} {1} 个" -f $d.Name, (Get-ChildItem $d.FullName -File -Filter *.cpp).Count }
    $n = (Get-ChildItem $ws -Recurse -File -Filter *.cpp | Where-Object { $_.FullName -match '\\0[1-9]-' }).Count
    Write-Host "  合计 $n 个模板文件" -ForegroundColor Green

    # 5. git 状态
    Write-Host "`n[ Git ]" -ForegroundColor DarkCyan
    Push-Location $ws
    $st = git status --porcelain 2>$null
    "  未提交改动: {0} 个文件" -f ($st | Measure-Object).Count
    "  最近提交: " + (git log --oneline -1 2>$null)
    Pop-Location

    Write-Host ("`n" + "=" * 78) -ForegroundColor DarkGray
    Write-Host " 给主控下指令：编辑 _台账\指令.md（未勾选的 [ ] 条目会被执行）" -ForegroundColor Magenta
    Write-Host " 直接指挥 Codex：codex exec resume --last ""你的补充要求""" -ForegroundColor Green
    Write-Host ("=" * 78) -ForegroundColor DarkGray
}

if ($Watch) { while ($true) { Show-All; Start-Sleep -Seconds 10 } } else { Show-All }
