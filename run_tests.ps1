################################################################################
# run_tests.ps1
# 
# PowerShell script for Windows users to compile and run tests without make.
# 
# Usage (in PowerShell):
#   .\run_tests.ps1
# 
# If you get an execution policy error, run this first:
#   Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
# 
# This script will:
#   1. Compile all test files
#   2. Run correctness tests
#   3. Ask if you want to run performance tests
#   4. Optionally run performance tests
################################################################################

Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "  DATA STRUCTURES PERFORMANCE COMPARISON" -ForegroundColor Cyan
Write-Host "  ArrayList vs LinkedList" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""

# Check if g++ is available
Write-Host "Checking for g++ compiler..." -ForegroundColor Yellow
$gppPath = Get-Command g++ -ErrorAction SilentlyContinue
if (-not $gppPath) {
    Write-Host ""
    Write-Host "❌ ERROR: g++ compiler not found!" -ForegroundColor Red
    Write-Host ""
    Write-Host "g++ is required to compile C++ code." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Please install a C++ compiler. Options:" -ForegroundColor Yellow
    Write-Host "  1. MinGW: https://sourceforge.net/projects/mingw/" -ForegroundColor Cyan
    Write-Host "  2. MSYS2: https://www.msys2.org/" -ForegroundColor Cyan
    Write-Host "  3. WSL (Windows Subsystem for Linux)" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "See WINDOWS_SETUP.md for detailed installation instructions." -ForegroundColor Yellow
    Write-Host ""
    exit 1
}

Write-Host "✓ g++ found: $($gppPath.Source)" -ForegroundColor Green
Write-Host "Compiling tests..." -ForegroundColor Yellow

# Compile LinkedList tests
Write-Host "Compiling LinkedList tests..."
g++ --std=c++17 test_linkedlist.cpp ArrayList.cpp LinkedList.cpp -o test_linkedlist.exe
if ($LASTEXITCODE -ne 0) {
    Write-Host "[X] Compilation failed for LinkedList tests" -ForegroundColor Red
    Write-Host "    Make sure all .cpp and .h files are in this directory" -ForegroundColor Red
    exit 1
}

# Compile performance tests
Write-Host "Compiling performance tests..."
g++ --std=c++17 test_performance.cpp ArrayList.cpp LinkedList.cpp -o test_performance.exe
if ($LASTEXITCODE -ne 0) {
    Write-Host "❌ Compilation failed for performance tests" -ForegroundColor Red
    exit 1
}

# Compile tail optimization test
Write-Host "Compiling tail optimization test..."
g++ --std=c++17 test_tail_optimization.cpp ArrayList.cpp LinkedList.cpp -o test_tail_optimization.exe
if ($LASTEXITCODE -ne 0) {
    Write-Host "❌ Compilation failed for tail optimization test" -ForegroundColor Red
    exit 1
}

Write-Host "✅ All files compiled successfully!" -ForegroundColor Green
Write-Host ""

# Run LinkedList tests
Write-Host "Running LinkedList tests..." -ForegroundColor Yellow
.\test_linkedlist.exe

if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "[!] LinkedList tests failed. Fix your LinkedList implementation first." -ForegroundColor Yellow
    Write-Host "    Review the errors above to see which functions need fixing." -ForegroundColor Yellow
    exit 1
}

Write-Host ""
Write-Host "[OK] All LinkedList tests passed!" -ForegroundColor Green

# Ask before running performance tests
Write-Host ""
$response = Read-Host "[OK] Ready for performance tests. Continue? (y/n)"
if ($response -eq "y" -or $response -eq "Y") {
    .\test_performance.exe
} else {
    Write-Host "Performance tests skipped." -ForegroundColor Gray
    Write-Host ""
    Write-Host "To run performance tests later, use:" -ForegroundColor Gray
    Write-Host "  .\test_performance.exe" -ForegroundColor Gray
    Write-Host ""
    Write-Host "To test tail optimization (after implementing it), use:" -ForegroundColor Gray
    Write-Host "  .\test_tail_optimization.exe" -ForegroundColor Gray
}

Write-Host ""
Write-Host "🎉 Done!" -ForegroundColor Green