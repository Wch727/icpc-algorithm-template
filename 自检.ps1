# 算法库自检（新架构）：模板只编译，测试编译+运行+查 FAIL
#   0X-*/*.cpp              -> 纯模板，用 g++ -c 检查能否独立编译
#   测试/0X-*/*_test.cpp    -> 测试与对拍，编译 + 运行 + 检查输出里的 FAIL/FAILED
# 用法: pwsh -File .\自检.ps1            # 全部
#       pwsh -File .\自检.ps1 04-图论    # 只查某个目录
param([string]$Only = '')

$ErrorActionPreference = 'Continue'
$root = $PSScriptRoot
$gxx  = 'C:\mingw64\bin\g++.exe'
$bin  = Join-Path $root 'bin'
New-Item -ItemType Directory -Force -Path $bin | Out-Null

function Style-Check($path) {
    $src = Get-Content $path -Raw -Encoding UTF8
    $bad = @()
    if ($src -notmatch '#include\s*<bits/stdc\+\+\.h>') { $bad += '无bits头' }
    if ($src -notmatch 'using namespace std;')            { $bad += '无using' }
    if ($src -match "(?m)^\t")                            { $bad += '有Tab缩进' }
    if ($src -match 'std::')                              { $bad += '有std::' }
    if ($bad.Count -eq 0) { return 'OK' } else { return ($bad -join ',') }
}

$rows = @()

# ---------- A. 纯模板：只编译 ----------
$dirs = Get-ChildItem $root -Directory | Where-Object { $_.Name -match '^\d\d-' -and ($Only -eq '' -or $_.Name -eq $Only) } | Sort-Object Name
foreach ($d in $dirs) {
    foreach ($f in Get-ChildItem $d.FullName -File -Filter *.cpp) {
        $obj = Join-Path $bin '_tpl.o'
        Remove-Item $obj -Force -ErrorAction SilentlyContinue
        $out = & $gxx -c $f.FullName -std=c++2b -O2 -Wall -o $obj 2>&1 | Out-String
        $comp = if (Test-Path $obj) { 'OK' } else { 'FAIL' }
        Remove-Item $obj -Force -ErrorAction SilentlyContinue
        $note = if ($comp -eq 'OK') { '' } else { (($out -split "`n" | Where-Object { $_ -match 'error' } | Select-Object -First 1) -replace '\s+', ' ').Trim() }
        if ($note.Length -gt 70) { $note = $note.Substring(0, 70) + '...' }
        $rows += [pscustomobject]@{ 类型 = '模板'; 文件 = "$($d.Name)\$($f.Name)"; 编译 = $comp; 运行 = '-'; 风格 = (Style-Check $f.FullName); 备注 = $note }
    }
}

# ---------- B. 测试：编译 + 运行 ----------
$tdir = Join-Path $root '测试'
if (Test-Path $tdir) {
    $tgroups = Get-ChildItem $tdir -Directory | Where-Object { $Only -eq '' -or $_.Name -eq $Only } | Sort-Object Name
    $i = 0
    foreach ($g in $tgroups) {
        foreach ($f in Get-ChildItem $g.FullName -File -Filter *.cpp) {
            $i++
            $exe = Join-Path $bin ('_test{0}.exe' -f $i)
            Remove-Item $exe -Force -ErrorAction SilentlyContinue
            $out = & $gxx $f.FullName -std=c++2b -O2 -Wall -o $exe 2>&1 | Out-String
            if (-not (Test-Path $exe)) {
                $note = (($out -split "`n" | Where-Object { $_ -match 'error' } | Select-Object -First 1) -replace '\s+', ' ').Trim()
                if ($note.Length -gt 70) { $note = $note.Substring(0, 70) + '...' }
                $rows += [pscustomobject]@{ 类型 = '测试'; 文件 = "$($g.Name)\$($f.Name)"; 编译 = 'FAIL'; 运行 = '-'; 风格 = '-'; 备注 = $note }
                continue
            }
            $run = 'OK'; $note = ''
            try {
                $psi = New-Object System.Diagnostics.ProcessStartInfo
                $psi.FileName = $exe
                $psi.RedirectStandardInput = $true; $psi.RedirectStandardOutput = $true; $psi.RedirectStandardError = $true
                $psi.UseShellExecute = $false
                $p = [System.Diagnostics.Process]::Start($psi)
                $p.StandardInput.Close()
                if (-not $p.WaitForExit(15000)) { $p.Kill(); $run = 'TIMEOUT' }
                else {
                    $all = $p.StandardOutput.ReadToEnd() + $p.StandardError.ReadToEnd()
                    if ($p.ExitCode -ne 0) { $run = "RE($($p.ExitCode))" }
                    elseif ($all -match 'FAILED|\bFAIL\b|失败\s*[1-9]|不通过|答案错误') { $run = 'FAILED-OUT' }
                    $note = ($all -replace '\s+', ' ').Trim()
                    if ($note.Length -gt 70) { $note = $note.Substring(0, 70) + '...' }
                }
            } catch { $run = 'RUNFAIL' }
            $rows += [pscustomobject]@{ 类型 = '测试'; 文件 = "$($g.Name)\$($f.Name)"; 编译 = 'OK'; 运行 = $run; 风格 = '-'; 备注 = $note }
            Remove-Item $exe -Force -ErrorAction SilentlyContinue
        }
    }
}

$bad = { $_.编译 -ne 'OK' -or ($_.类型 -eq '测试' -and $_.运行 -ne 'OK') -or ($_.类型 -eq '模板' -and $_.风格 -ne 'OK') }
$rows | Where-Object $bad | Format-Table -AutoSize | Out-String -Width 200 | Write-Host
$fail = $rows | Where-Object $bad
Write-Host ("模板 {0} 个 + 测试 {1} 个，异常 {2} 个" -f `
    ($rows | Where-Object { $_.类型 -eq '模板' }).Count, ($rows | Where-Object { $_.类型 -eq '测试' }).Count, $fail.Count)
