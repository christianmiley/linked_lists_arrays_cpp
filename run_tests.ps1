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
Write-Host "Compiling tests..." -ForegroundColor Yellow

# Compile correctness tests
Write-Host "Compiling correctness tests..."
g++ --std=c++17 test_correctness.cpp ArrayList.cpp LinkedList.cpp -o test_correctness.exe
if ($LASTEXITCODE -ne 0) {
    Write-Host "❌ Compilation failed for correctness tests" -ForegroundColor Red
    Write-Host "   Make sure all .cpp and .h files are in this directory" -ForegroundColor Red
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

# Run correctness tests
Write-Host "Running correctness tests..." -ForegroundColor Yellow
.\test_correctness.exe

if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "⚠️  Correctness tests failed. Fix your LinkedList implementation first." -ForegroundColor Yellow
    exit 1
}

# Ask before running performance tests
Write-Host ""
$response = Read-Host "✅ Correctness tests passed! Run performance tests? (y/n)"
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