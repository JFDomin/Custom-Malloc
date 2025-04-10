# Custom Malloc Implementation in C

## Overview

This project is a custom memory management library written in C that replicates the behavior of `malloc()` and `free()` by interacting directly with the operating system to manage heap allocation. It supports multiple allocation strategies and simulates how a user-level allocator works under the hood.

## Features

- Implements four heap allocation strategies:
  - **First-Fit**
  - **Next-Fit**
  - **Best-Fit**
  - **Worst-Fit**
- Supports block splitting and memory coalescing to reduce fragmentation
- Tracks and prints runtime statistics for each strategy
- Includes benchmarking tests to compare with standard `malloc()`

### Build the project:
```bash
bash run.sh