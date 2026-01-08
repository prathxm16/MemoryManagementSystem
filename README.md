# System Simulator: Memory & Cache

A C++ simulation of a computer system's physical memory management and cache hierarchy. This project demonstrates core operating system concepts including contiguous memory allocation, fragmentation handling, and multi-level cache coherence.

## Features

### 1. Physical Memory Allocator
* **Contiguous Allocation:** Simulates RAM as a contiguous byte array.
* **Allocation Strategies:** Supports dynamic switching between:
    * **First Fit:** Fastest allocation.
    * **Best Fit:** Minimizes internal fragmentation.
    * **Worst Fit:** Leaves larger holes to avoid external fragmentation.
* **Fragmentation Handling:**
    * **Alignment:** Automatically aligns all allocations to 4-byte boundaries.
    * **Coalescing:** Automatically merges adjacent free blocks upon deallocation.

### 2. Cache Simulator
* **Two-Level Hierarchy:** Simulates L1 and L2 caches backed by Main Memory (RAM).
* **Configurable Architecture:** Allows runtime configuration of cache size, block size, and associativity.
* **Policies:**
    * **Replacement:** FIFO (First-In, First-Out) eviction policy.
    * **Write Policy:** Write-Allocate (fetches data on write miss).
* **Performance Metrics:** Tracks Hits, Misses, and calculates Average Memory Access Time (AMAT) based on configurable latencies (L1=1, L2=10, RAM=100 cycles).

## Project Structure

```text
.
├── include/        # Header files (Interface)
├── src/            # Source code (Implementation)
│   ├── allocator/  # Physical Memory logic
│   ├── cache/      # Cache Hierarchy logic
│   └── main.cpp    # CLI Driver
├── tests/          # Test input scripts
└── Makefile        # Build configuration
```
## Supported Commands

| Command | Arguments | Description |
| :--- | :--- | :--- |
| **Memory** | | |
| `init` | `<size>` | Initialize physical memory with specific size. |
| `set` | `allocator <mode>` | Set strategy: `first_fit`, `best_fit`, or `worst_fit`. |
| `malloc` | `<size>` | Allocate memory block of size bytes. |
| `free` | `<id>` | Free the memory block with the given ID. |
| `dump` | | Print the current memory map (Used/Free blocks). |
| `stats` | | Show fragmentation statistics. |
| **Cache** | | |
| `read` | `<hex_addr>` | Simulate a read operation at address (e.g., `read 0x1A`). |
| `write` | `<hex_addr>` | Simulate a write operation at address. |
| `cache_stats` | | Show Hits, Misses, and AMAT. |
| `cache_config`| `<lvl> <sz> <blk> <assoc>` | Reconfigure L1 or L2 parameters at runtime. |
| `exit` | | Exit the simulator. |

## How to Run

### Run with Test Inputs 
To execute the simulator using the pre-defined test workload inside the tests folder paste the given command in project directory:

- To compile:
```powershell
make
```
- To run with test scripts:

(Linux/MacOS)
```powershell
./bin/memsim < tests/physicalmemory_test.txt
./bin/memsim < tests/cache_test.txt
```
(Windows)
```powershell
Get-Content tests\physicalmemory_test.txt | .\bin\memsim.exe
Get-Content tests\cache_test.txt | .\bin\memsim.exe
```
### Run manually
To run the simulator in interactive mode

```powershell
make run
```
