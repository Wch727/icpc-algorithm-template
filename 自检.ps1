# 算法库自检脚本：对每个模板文件做「编译 -> 运行 -> 风格检查」
# 用法: pwsh -File .\自检.ps1            # 全部检查
#       pwsh -File .\自检.ps1 04-图论    # 只查某个目录
param([string]$Only = '')

$ErrorActionPreference = 'Continue'
$root = $PSScriptRoot
$gxx  = 'C:\mingw64\bin\g++.exe'
$bin  = Join-Path $root 'bin'
New-Item -ItemType Directory -Force -Path $bin | Out-Null

$dirs = Get-ChildItem -Path $root -Directory |
        Where-Object { $_.Name -match '^\d\d-' -and ($Only -eq '' -or $_.Name -eq $Only) } |
        Sort-Object Name

$files = foreach ($d in $dirs) { Get-ChildItem -Path $d.FullName -Recurse -File -Filter *.cpp }

$rows = @()
foreach ($f in $files) {
    $rel  = $f.FullName.Substring($root.Length + 1)
    $exe  = Join-Path $bin '_chk.exe'
    Remove-Item $exe -Force -ErrorAction SilentlyContinue

    # --- 编译 ---
    $cout = & $gxx $f.FullName -std=c++2b -O2 -Wall -o $exe 2>&1 | Out-String
    if (-not (Test-Path $exe)) {
        $rows += [pscustomobject]@{ 文件 = $rel; 编译 = 'FAIL'; 运行 = '-'; 风格 = ''; 备注 = ($cout -split "`n" | Select-Object -First 1) }
        continue
    }

    # --- 运行（5 秒超时，喂空输入）---
    $run = 'OK'; $note = ''
    try {
        $psi = New-Object System.Diagnostics.ProcessStartInfo
        $psi.FileName = $exe
        $psi.RedirectStandardInput = $true
        $psi.RedirectStandardOutput = $true
        $psi.RedirectStandardError = $true
        $psi.UseShellExecute = $false
        $p = [System.Diagnostics.Process]::Start($psi)
        $p.StandardInput.Close()
        if (-not $p.WaitForExit(15000)) { $p.Kill(); $run = 'TIMEOUT' }
        else {
            $o = $p.StandardOutput.ReadToEnd()
            $e = $p.StandardError.ReadToEnd()
            if ($p.ExitCode -ne 0) { $run = "RE($($p.ExitCode))" }
            $note = (($o + $e) -replace '\s+', ' ').Trim()
            if ($note.Length -gt 60) { $note = $note.Substring(0, 60) + '...' }
        }
    } catch { $run = 'RUNFAIL' }

    # --- 风格检查 ---
    $src = Get-Content $f.FullName -Raw -Encoding UTF8
    $bad = @()
    if ($src -notmatch '#include\s*<bits/stdc\+\+\.h>') { $bad += '无bits头' }
    if ($src -notmatch 'using namespace std;')            { $bad += '无using' }
    if ($src -notmatch 'return 0;')                       { $bad += '无return0' }
    if ($src -match "(?m)^\t")                            { $bad += '有Tab缩进' }
    if ($src -match 'std::')                              { $bad += '有std::' }
    $style = if ($bad.Count -eq 0) { 'OK' } else { $bad -join ',' }

    $rows += [pscustomobject]@{ 文件 = $rel; 编译 = 'OK'; 运行 = $run; 风格 = $style; 备注 = $note }
    Remove-Item $exe -Force -ErrorAction SilentlyContinue
}

$rows | Format-Table -AutoSize | Out-String -Width 200 | Write-Host
$fail = $rows | Where-Object { $_.编译 -ne 'OK' -or $_.运行 -ne 'OK' -or $_.风格 -ne 'OK' }
Write-Host ("总计 {0} 个文件，异常 {1} 个" -f $rows.Count, $fail.Count)
