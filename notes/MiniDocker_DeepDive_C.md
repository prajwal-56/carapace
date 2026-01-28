# Mini-Docker (The "Container") - From First Principles

**Target Audience:** Beginner to Intermediate.  
**Goal:** Understanding "Containers" without buzzwords.  
**Implementation Language:** C.  
**Environment:** Linux (via WSL2).

---

## 1. The Core Concept: The "Great Lie"

To understand Docker, you must unlearn the idea that a "Container" is a physical box or a virtual machine. It is not real.

A Container is just a **normal process** (like your web browser or music player) that the Operating System (OS) has been instructed to **lie to**.

### The Metaphor: The Truman Show
Imagine a person named "Truman" (our process).
1.  **Normal Process:** Truman lives in the real world. He can see his neighbors (other processes), he can walk into the town square (the root file system `/`), and he can use as much water/electricity as he wants (RAM/CPU).
2.  **Containerized Process:** We wake Truman up in a movie set.
    *   **The Walls (Namespaces):** When he looks out the window, we project a fake image. He thinks he is alone. He cannot see the neighbors.
    *   **The House (Chroot/Pivot Root):** If he tries to leave his house, he hits a locked door. He thinks his house is the entire world.
    *   **The Allowance (Cgroups):** We only give him 500ml of water a day. If he asks for more, the tap runs dry.

**Our Project Goal:** We are going to write the program that builds this movie set.

---

## 2. The Three Pillars of Deception

To build this "movie set" in C, we need to manipulate three specific features of the Linux Kernel.

### Pillar 1: Isolation (Namespaces) - "The Tinted Windows"
*   **The Reality:** Your computer runs hundreds of processes (PID 1, PID 100, PID 5432).
*   **The Lie:** The container thinks it is **Process ID 1** (The Boss). It looks around and sees *no other processes*.
*   **How we do it:**
    *   Standard way to start a process: `fork()` (Copies the parent).
    *   The Container way: `clone()` (Copies the parent *but* sets specific flags).
    *   **Key Flag:** `CLONE_NEWPID`. This tells the Kernel: "Start a new list of Process IDs for this child. Make it #1 in that new list."

### Pillar 2: The Jail (Root Filesystem) - "The Locked Door"
*   **The Reality:** Your computer has a hard drive with folders like `/Windows`, `/Users`, `/bin`.
*   **The Lie:** The container thinks a specific folder (e.g., `/home/prajwal/my-container`) is actually `/`. It cannot go higher than that folder.
*   **How we do it:**
    *   **`chroot` (Change Root):** An old system call that says "For this process, treat this folder as the root directory."
    *   **`pivot_root`:** The modern, more secure version used by Docker. It swaps the "real" root with our "fake" root so effectively that the process can't ever find the way back.

### Pillar 3: Limits (Cgroups) - "The Allowance"
*   **The Reality:** Your computer has 16GB of RAM. If a program wants 16GB, it takes it (and crashes the PC).
*   **The Lie:** The container thinks the computer only has 512MB of RAM. If it tries to use 513MB, the OS kills it.
*   **How we do it:**
    *   **Control Groups (cgroups):** This is a feature exposed by the kernel as a *filesystem*.
    *   To limit memory, we don't write complex code. We literally write a number into a file.
    *   Example: `echo "5000000" > /sys/fs/cgroup/memory/my_container/memory.limit_in_bytes`
    *   The Kernel reads that file and enforces the law.

---

## 3. Architecture & Workflow

We are building a C program named `mini-docker`.

### The Flow of Data
```mermaid
graph TD
    User[User] -->|Run Command| CLI[mini-docker CLI (C Program)]
    CLI -->|1. Setup Configuration| Config[Setup Config]
    Config -->|2. Create Cgroup| CG[Write to /sys/fs/cgroup]
    Config -->|3. Clone Process| Child[Child Process Created]
    
    subgraph "The Host (Real World)"
        CLI
        CG
    end

    subgraph "The Container (The Lie)"
        Child -->|4. Change Hostname| HN[Set Hostname]
        Child -->|5. Change Root| CR[chroot / pivot_root]
        Child -->|6. Mount Proc| MP[Mount /proc]
        Child -->|7. Execute User Cmd| EXEC[exec() /bin/bash]
    end
```

### Detailed Workflow

#### Phase 1: Setup (The Environment)
We will use **WSL2 (Windows Subsystem for Linux)**.
*   **Why?** Because `namespaces`, `cgroups`, and `chroot` are **Linux Kernel features**. Windows has its own containers, but they work differently. You cannot build a Linux container on the Windows Kernel directly.
*   **Distro:** Debian or Ubuntu (Kali is fine, but Ubuntu is more standard for dev).

#### Phase 2: The Skeleton (C Code)
We will write a C program that takes arguments.
*   Command: `./mini-docker run <image_folder> <command>`
*   Concept: `argc` and `argv` parsing.

#### Phase 3: The Split (Clone)
We replace the standard `fork()` with `clone()`.
*   We will allocate a "stack" (memory for the child process) manually.
*   We will pass flags like `CLONE_NEWUTS` (new hostname) and `CLONE_NEWPID` (new process IDs).

#### Phase 4: The Jail (Filesystem)
We need a "Root Filesystem". This is just a folder that *looks* like a Linux install (it has `/bin`, `/lib`, `/usr`).
*   We will download a minimal "Alpine Linux" filesystem (it's tiny, ~5MB).
*   Our C code will tell the child process to `chroot` into that folder.

#### Phase 5: The Enforcement (Cgroups)
Before the child starts, the parent will:
1.  Create a folder: `/sys/fs/cgroup/memory/demo`
2.  Write the child's PID into: `/sys/fs/cgroup/memory/demo/tasks`
3.  Write the limit into: `/sys/fs/cgroup/memory/demo/memory.limit_in_bytes`

---

## 4. Why C?
In Rust, we used a library (`nix`) to hide the scary details.
In C, we use **System Calls** directly.

*   **`#include <sched.h>`**: Gives us `clone()`.
*   **`#include <unistd.h>`**: Gives us `exec()` and `chroot()`.
*   **`#include <sys/mount.h>`**: Gives us `mount()`.

You are talking to the Kernel in its native language. There is no translator.

## 5. What You Need To Know (Prerequisites)
Since we are explaining "First Principles", here is the only pre-knowledge assumed:
1.  **Variables:** You know what an integer and a string (char array) are.
2.  **Pointers:** You know that `*ptr` points to a memory address (we will explain this heavily when we use it).
3.  **Command Line:** You know how to open a terminal.

Everything else (Syscalls, PID, Mounts) will be explained as we build it.
