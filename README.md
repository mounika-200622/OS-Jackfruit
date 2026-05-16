# Multi-Container Runtime (OS Jackfruit)

## Student Details

* Name: Pavana P
* SRN: PES1UG24CS320

---

## Project Overview

This project implements a lightweight multi-container runtime in C. It allows execution of multiple isolated containers using Linux system calls. A supervisor process manages container lifecycle, and a kernel module is used for monitoring system-level behavior.

---

## Objectives

* Implement container creation and management
* Support multiple containers simultaneously
* Demonstrate process scheduling using `nice` values
* Perform CPU and memory stress testing
* Integrate a kernel module for monitoring

---

## System Setup

* OS: Ubuntu 22.04/24.04
* Language: C
* Tools: gcc, make
* Kernel headers installed
* Secure Boot disabled

---

## Implementation and Experiments

### 1. Root Filesystem Creation

A minimal Alpine Linux root filesystem was downloaded and extracted:

```bash
mkdir rootfs-base
wget https://dl-cdn.alpinelinux.org/alpine/v3.20/releases/x86_64/alpine-minirootfs-3.20.3-x86_64.tar.gz
tar -xzf alpine-minirootfs-3.20.3-x86_64.tar.gz -C rootfs-base
```

Two writable container filesystems were created:

```bash
cp -a rootfs-base rootfs-alpha
cp -a rootfs-base rootfs-beta
```

---

### 2. Supervisor Start

```bash
sudo ./engine supervisor rootfs-alpha/
```

The supervisor runs continuously and listens for container commands.

---

### 3. First Container Execution

```bash
./engine start c1 rootfs-alpha/ "sleep 40"
```

This starts a container running a simple process.

---

### 4. Metadata Tracking and Stop

```bash
ps -o pid,ni,cmd | grep sleep
./engine stop c1
```

The process is verified and then terminated.

---

### 5. Container Stop Verification

After stopping, running `ps` confirms the process is no longer active.

---

### 6. Multi-Container Execution and Logs

```bash
./engine start c1 rootfs-alpha/ "sleep 40"
./engine start c2 rootfs-beta/ "nice -n 10 sleep 40"
./engine logs c1
```

Two containers run simultaneously with different priorities.

---

### 7. Stress Testing (Memory)

A memory-intensive workload is executed to observe system behavior under heavy memory usage.

---

### 8. Scheduler Experiment

Two containers were executed:

* Default priority (`nice = 0`)
* Lower priority (`nice = 10`)

Verification:

```bash
ps -o pid,ni,cmd | grep sleep
```

Observation:
The process with lower nice value (0) receives more CPU time compared to the one with higher nice value (10).

---

### 9. CPU Usage Experiment

```bash
./engine start c3 rootfs-alpha/ "yes > /dev/null"
top
```

Observation:
CPU usage increases significantly (around 90%), demonstrating scheduler behavior under load.

---

### 10. Memory Monitoring (Soft and Hard Limits)

The kernel module is used to monitor memory usage of container processes. Soft and hard limits help restrict excessive memory consumption.

---

### 11. Foreground Execution

```bash
./engine start c4 rootfs-alpha/ "sleep 5"
```

The container exits automatically after execution, demonstrating foreground behavior.

---

### 12. Zombie Process Check

```bash
ps aux | grep Z
```

No zombie processes were observed, indicating proper cleanup.

---

### 13. Echo Container Test

```bash
./engine start c5 rootfs-alpha/ "echo hello"
```

This verifies command execution inside the container.

---

## Features Implemented

* Multi-container runtime
* Supervisor-based architecture
* Container lifecycle management
* Logging system
* CPU and memory stress testing
* Scheduling control using `nice`
* Kernel module integration

---

## Design Decisions

* Used `fork()` and `exec()` for process creation
* Supervisor handles all container operations
* Separate root filesystems ensure isolation
* `nice` values used to demonstrate scheduling
* Modular structure for maintainability

---

## Kernel Module

Loaded using:

```bash
sudo insmod monitor.ko
lsmod | grep monitor
```

Purpose:

* Monitor container resource usage
* Provide system-level insights

---

## Challenges Faced

* Managing multiple containers simultaneously
* Debugging process lifecycle issues
* Ensuring correct scheduling behavior
* Handling system resource monitoring

---

## Screenshots

(Add screenshots of your terminal outputs here in the repository)

---

## Conclusion

This project successfully demonstrates the implementation of a multi-container runtime system in Linux. It shows how process scheduling, isolation, and resource monitoring can be achieved using system-level programming.

---

## References

* Linux manual pages
* Alpine Linux documentation
* Course project guide

