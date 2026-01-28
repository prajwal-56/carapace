# Mini-Docker (Crab-Can) Project Specification

**Project Name:** Crab-Can (Rust-based Container Runtime)
**Subject:** Operating Systems (S4)
**Type:** Systems Programming / Microproject
**Language:** Rust
**Target Environment:** Linux Kernel (WSL2, Native Linux, or VM)

---

## 1. Project Overview
This project involves building a minimal container runtime from scratch. Unlike using Docker, which abstracts the underlying mechanisms, this project will interact directly with the Linux Kernel to create isolated environments for processes.

**The Goal:**
To build a program that can "jail" a child process, convincing it that it is running on its own private computer with its own file system, process ID space, and resource limits.

**Core Philosophy:**
> "A container is not a real object; it is a normal process that the Kernel has been instructed to lie to."

---

## 2. Theoretical Foundation (Syllabus Mapping)
This project directly implements the core concepts taught in the Operating Systems course:

| OS Concept | Project Implementation |
| :--- | :--- |
| **System Calls** | Direct use of `clone`, `pivot_root`, `mount`, `exec` via Rust's `nix` crate. |
| **Process Management** | Creating and managing child processes, handling PIDs, and `wait` calls. |
| **Process Isolation** | Using **Linux Namespaces** to hide other processes and network interfaces. |
| **Resource Management** | Using **Cgroups (Control Groups)** to limit RAM and CPU usage. |
| **File Systems** | Implementing **chroot/pivot_root** to swap the file system view for the process. |

---

## 3. Architecture: The 3 Pillars of Isolation

To successfully containerize a process, the application must construct three distinct "walls":

### Pillar 1: Visibility (Namespaces)
*   **Problem:** By default, a child process inherits its parent's view of the system (sees all processes, same network IP, same hostname).
*   **Solution:** Use the `clone` syscall with specific flags to create new **Namespaces**.
    *   `PID Namespace`: The process becomes PID 1 inside the container.
    *   `UTS Namespace`: The process can have its own hostname.
    *   `Mount Namespace`: The process can have separate mount points.
    *   `Network Namespace`: The process has its own loopback interface (localhost).

### Pillar 2: Access (Roots & Mounts)
*   **Problem:** Even if isolated, the process can browse the host's root file system (`/`).
*   **Solution:** Change the root directory.
    *   **Old Way:** `chroot` (change root).
    *   **Modern Way:** `pivot_root`. This swaps the root filesystem for the process, making the old root inaccessible and the new directory the absolute top of the tree.

### Pillar 3: Resources (Cgroups)
*   **Problem:** An isolated process can still crash the host by consuming 100% CPU or leaking memory.
*   **Solution:** Linux **Control Groups (cgroups)**.
    *   The program will interact with the kernel's cgroup interface (usually at `/sys/fs/cgroup`).
    *   It will create a new group (e.g., `/sys/fs/cgroup/memory/crab-can`) and write resource limits (e.g., `5000000` bytes) into the configuration files.
    *   The child process's PID is added to the `cgroup.procs` file before execution.

---

## 4. Execution Workflow

When the `crab-can` binary is executed, the following sequence occurs:

1.  **Parse Arguments:**
    *   User inputs command: `crab-can run <image_path> <command>`
    *   Rust uses `clap` to parse these arguments.

2.  **Setup Cgroups (The Fence):**
    *   Create a new cgroup directory.
    *   Set limits (e.g., Max RAM).

3.  **The Clone (The Birth):**
    *   Call `sched::clone` with namespace flags (`CLONE_NEWPID`, `CLONE_NEWUTS`, etc.).
    *   This spawns the child process *before* it starts running the user's command.

4.  **Child Configuration (Inside the Jail):**
    *   **Set Hostname:** Change hostname to `container-id`.
    *   **Mount Pseudo-filesystems:** Mount `/proc` (crucial for `ps` to work correctly).
    *   **Pivot Root:** Swap the OS root to the provided `<image_path>`.
    *   **Environment:** Clear environment variables and set new ones (PATH, TERM).

5.  **The Execution:**
    *   Call `exec` (specifically `execvp` or similar).
    *   This replaces the running Rust code with the user's requested command (e.g., `/bin/sh`).
    *   The user is now dropped into a shell inside the container.

6.  **Cleanup (The Parent):**
    *   The parent process waits for the child to exit.
    *   Once finished, the parent deletes the cgroup directory.

---

## 5. Technology Stack & Requirements

*   **Language:** Rust (Stable)
*   **Operating System:** Linux (Required).
    *   *Windows Users:* Must use **WSL2** (Windows Subsystem for Linux) or a Virtual Machine.
*   **Key Crates (Libraries):**
    *   `nix`: For safe wrappers around unsafe Linux system calls (`clone`, `mount`, `sched`).
    *   `clap`: For building the Command Line Interface.
    *   `anyhow`: For robust error handling.

## 6. Project Roadmap

1.  **Phase 1: The Shell:** Build a basic CLI that can spawn a child process.
2.  **Phase 2: Isolation:** Implement `clone` flags to give the child its own PID namespace. Verify with `ps`.
3.  **Phase 3: The Jail:** Implement `pivot_root` to trap the process in a folder.
4.  **Phase 4: The Fence:** Implement Cgroups to limit memory usage. Verify by trying to crash it.

---

**Notes:**
*   This is a "Deep Dive" project. It requires reading Linux `man` pages.
*   Development must be done in a Linux environment where you have `sudo` (root) privileges, as namespaces and cgroups require root.
