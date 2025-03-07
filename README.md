Concurrency Lab Practicals – Solving Synchronisation Challenges
Produced by: Kirubel Temesgen
College ID: C00260396
Date Prepared: 30/09/2024

Overview
This repository contains practical implementations of concurrency problems using Go and C++. 
These tasks test and reinforce an understanding of deadlocks, race conditions, thread synchronisation, and efficient concurrency mechanisms.

The lab includes solutions for:

Dining Philosophers Problem – Preventing deadlocks and ensuring fair access to shared resources.
Producer-Consumer Problem – Efficient synchronisation using semaphores and mutual exclusion.
Features
Safe Resource Sharing – Implements mutex locks, semaphores, and synchronisation techniques.
Avoiding Deadlocks – Ensures fair scheduling in multi-threaded environments.
Efficient Thread Management – Uses concurrency primitives such as channels (Go) and semaphores (C++).
Performance Benchmarks – Measures execution time across different concurrency models.

Installation
1. Prerequisites
Go (Golang) installed for running Go-based solutions.
C++
CMake (optional) for managing dependencies in C++ projects.

2. Clone the Repository
Run the following commands:

git clone https://github.com/KirubelCode/Common-Concurrency-Tasks.git


3. Running the Go-Based Dining Philosophers Problem
Compile and execute the Go program:

  go run Lab6/TEST2.GO

The program will simulate five philosophers attempting to eat while avoiding deadlocks and starvation.

Problem Summaries
Dining Philosophers Problem (Go)
Simulates five philosophers who think, pick up forks, eat, and release forks.
Uses channels and a footman synchronisation strategy to prevent deadlocks.
Limits concurrent eating to four philosophers at a time, ensuring smooth execution.

Implements a thread-safe bounded buffer where producers generate events and consumers process them.
Uses semaphores to synchronise buffer access and prevent data loss.
Optimised with atomic operations for unique event IDs.

Future Enhancements
Extend Dining Philosophers – Introduce alternative synchronisation techniques such as waiters or priorities.
Dynamic Thread Scaling – Adjust the number of producers and consumers dynamically based on system load.
Performance Benchmarking – Measure execution efficiency with varying buffer sizes and thread counts.
