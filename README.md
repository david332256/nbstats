# NBStats — Newcomb-Benford Statistical Analysis

A C++ command-line tool that analyzes numeric data and tests whether it follows **Benford's Law** — a statistical principle used in fraud detection, forensic accounting, and data integrity checks.

## What It Does

- Reads numbers from a **file** or **stdin**
- Calculates descriptive statistics (mean, median, mode, variance, std deviation)
- Analyzes the **first-digit distribution** against Benford's Law
- Renders an **ASCII bar chart** comparing expected vs. actual percentages
- Classifies the strength of the Benford relationship

## Technologies Used

- **C++20** — Modern standard library
- **STL** — `vector`, `algorithm`, `fstream`, `iostream`, `iomanip`
- **Math library** — `cmath` for logarithms and standard deviation

## How to Compile & Run

```bash
g++ -std=c++20 NBStats.cpp -o nbstats
./nbstats sample-data.txt
```

./nbstats
# Type numbers, then Ctrl+Z (Windows) or Ctrl+D (Mac/Linux) to end

Usage
nbstats.exe [filename] [--skipbad] [--help]

  filename    - Optional file to read numbers from
  --skipbad   - Skip non-numeric input instead of terminating
  --help      - Display help message

Example Output
Range: [5, 25]
Mean: 12.417
Median: 15.5
Variance: 75.74
Standard Deviation: 8.703

Benford's Law Analysis:
Digit  Expected%  Actual%   Bar
-----  ---------  --------  ----------
  1      30.10%    14.29%   *******
  2      17.61%    28.57%   **************
  ...

 Author
Oluwadarasimi Adufe

 Course
Computer Programming & Analysis (Co-op)
Fanshawe College


