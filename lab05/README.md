# Multi-Threaded Web Log Analyzer

A high-performance, parallelized log processing tool written in **C**. This project demonstrates the power of multi-threading and efficient data structures by analyzing millions of web server records to extract key statistics in seconds.

## 🚀 Features

-   **Parallel Processing**: Utilizes the `pthread` library to divide a single large log file into chunks, processing them simultaneously across multiple CPU cores.
-   **Lock-Free Scalability**: Implements a "Shared-Nothing" architecture where each thread maintains its own local statistics, eliminating the need for expensive Mutex locks during the heavy processing phase.
-   **High-Speed Data Structures**: Uses custom-built **Hash Tables with Chaining** for O(1) average lookup and insertion of unique IP addresses and URLs.
-   **Map-Reduce Architecture**: Follows a classic Map-Reduce pattern where worker threads (Map) generate local results that are merged by the main thread (Reduce) at the end.

## 📊 Analyzed Metrics

The tool extracts the following information from `access.log` files:
1.  **Total Unique IPs**: Counts how many distinct visitors accessed the server.
2.  **Most Visited URL**: Identifies the most popular endpoint and the number of hits it received.
3.  **HTTP Error Count**: Summarizes all requests that returned a 4xx or 5xx status code.

## 🏗️ Architecture Overview

The program operates in three distinct phases:

1.  **Partitioning**: The main thread calculates the file size and assigns byte-offsets to each worker thread.
2.  **Parallel Processing**: Workers seek to their assigned offset and parse log lines into their private hash tables.
3.  **Merging**: Once all workers finish, the main thread traverses the local hash tables and merges the results into a global structure for final reporting.

> **Note on Data Integrity**: The analyzer includes logic to skip partial lines at the start of a chunk and read past the end of a chunk to ensure no log entry is counted twice or missed.

## 📂 Project Structure

```text
├── main_linux.c      # Thread orchestration and file partitioning
├── log_processor.c   # Log parsing, hash table logic, and merging
├── log_processor.h   # Data structures and function prototypes
├── Makefile          # Automated build system
├── run.sh            # Helper script to clean, build, and run
└── access.log        # Sample log file for testing
```

## 🛠️ How to Run

### Prerequisites
-   A Linux environment (or WSL on Windows).
-   `gcc` compiler.
-   `make` utility.

### Compilation
Build the project using the provided Makefile:
```bash
make
```

### Execution
Run the analyzer by providing the path to a log file:
```bash
./log_analyzer access.log
```

Alternatively, use the helper script:
```bash
./run.sh
```

## 📈 Performance Tip
You can adjust the number of threads by changing the `NUM_THREADS` macro in `main_linux.c` to match your CPU's core count for maximum throughput.

---
*Developed as part of the CC7 Operating Systems Laboratory (Lab05).*
