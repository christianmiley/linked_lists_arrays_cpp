# Quick Compilation Commands

This is a quick reference for compiling and running tests without using `make`.

## Quick Start (Easiest)

**Mac/Linux:**
```bash
chmod +x run_tests.sh   # First time only
./run_tests.sh
```

**Windows (PowerShell):**
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser # First time only
.\run_tests.ps1
```

**Windows (Git Bash):**
```bash
bash run_tests.sh
```

---

## Individual Commands

### Mac/Linux

Copy and paste these into your terminal:

#### Test Your Implementation
```bash
g++ --std=c++17 test_correctness.cpp ArrayList.cpp LinkedList.cpp -o test_correctness.out && ./test_correctness.out
```

#### Run Performance Tests
```bash
g++ --std=c++17 test_performance.cpp ArrayList.cpp LinkedList.cpp -o test_performance.out && ./test_performance.out
```

#### Test Tail Optimization (After Implementing It)
```bash
g++ --std=c++17 test_tail_optimization.cpp ArrayList.cpp LinkedList.cpp -o test_tail_optimization.out && ./test_tail_optimization.out
```

### Windows (PowerShell)

Copy and paste these into PowerShell:

#### Test Your Implementation
```powershell
g++ --std=c++17 test_correctness.cpp ArrayList.cpp LinkedList.cpp -o test_correctness.exe
if ($?) { .\test_correctness.exe }
```

#### Run Performance Tests
```powershell
g++ --std=c++17 test_performance.cpp ArrayList.cpp LinkedList.cpp -o test_performance.exe
if ($?) { .\test_performance.exe }
```

#### Test Tail Optimization (After Implementing It)
```powershell
g++ --std=c++17 test_tail_optimization.cpp ArrayList.cpp LinkedList.cpp -o test_tail_optimization.exe
if ($?) { .\test_tail_optimization.exe }
```

---

## Clean Up Generated Files

**Mac/Linux:**
```bash
rm -f *.out *.csv *.png
```

**Windows (PowerShell):**
```powershell
Remove-Item *.exe, *.csv, *.png -ErrorAction SilentlyContinue
```

---

## VSCode Integration

If you're using VSCode, you can use the keyboard shortcuts:

1. **Test Correctness**: Press `Cmd+Shift+B` (Mac) or `Ctrl+Shift+B` (Windows/Linux)
2. **Other Tasks**: Press `Cmd+Shift+P` → type "Tasks: Run Task" → choose task

Available tasks:
- Test Correctness (default)
- Test Performance
- Test Tail Optimization
- Clean

---

## Troubleshooting

**"Command not found" error?**
- Make sure g++ is installed: `g++ --version`
- On Mac: Install Xcode Command Line Tools
- On Linux: `sudo apt-get install g++`

**"No such file or directory" error?**
- Make sure you're in the correct directory
- Run `ls` to see if all .cpp and .h files are present

**Compilation errors?**
- Read the error message carefully
- Make sure you've implemented all methods in LinkedList.cpp
- Check that your method signatures match LinkedList.h