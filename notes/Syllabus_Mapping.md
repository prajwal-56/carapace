# Syllabus Mapping: Mini-Docker vs. Operating Systems (S4)

This document explains how your **"Mini-Docker" (Crab-Can)** project directly proves your mastery of the concepts in your **Operating Systems (KTU S4)** syllabus.

---

## 🔗 The Direct Link

**Project Goal:** Build a Container Runtime (Process Isolation Tool).
**Syllabus Goal:** Understand how the OS manages processes, memory, and resources.

This project is not "extra" work; it is a **practical implementation** of the theory.

---

## 📚 Module-by-Module Breakdown

### Module 1: Process Management
*   **Syllabus Topic:** "Process Creation, Process States, Process API (fork, exec)."
*   **Project Implementation:**
    *   **The "Clone" Call:** Instead of the basic `fork()` you learn in class, you will use `clone()`. This is the modern, powerful version of fork that allows for namespaces.
    *   **The "Exec" Call:** You will use `exec()` to replace your Rust program with the user's command (like `/bin/bash`) inside the container.
    *   **Wait & Exit:** Your parent process must `wait()` for the child container to finish, handling "Zombie Processes" (a core exam topic).

### Module 1: System Calls
*   **Syllabus Topic:** "Operating System Services, System Boot Sequence."
*   **Project Implementation:**
    *   You are bypassing the standard library for many things and talking directly to the Kernel via **System Calls** (`sched`, `mount`, `pivot_root`).
    *   This demonstrates you understand the boundary between **User Mode** (your app) and **Kernel Mode** (the OS internals).

### Module 2: Concurrency & Synchronization
*   **Syllabus Topic:** "Threads, Sharing processor among processes."
*   **Project Implementation:**
    *   Containers *are* processes. By running a container while your host system does other things, you are relying on the OS's **Scheduler** (CFS - Completely Fair Scheduler) to switch contexts between your container and your browser.
    *   While you won't write the scheduler, you will *configure* it using Cgroups (see below).

### Module 3: Memory Management
*   **Syllabus Topic:** "Address Space, Segmentation, Virtual Memory."
*   **Project Implementation:**
    *   **Cgroups (Control Groups):** This is the star of the show. You will write code to limit the **RAM** usage of your container.
    *   *Real-world Application:* If you set a 500MB limit and the process inside tries to `malloc` 600MB, the OS OOM (Out of Memory) Killer will terminate it. You will see memory management enforcement in action.

### Module 4: Files and Directories
*   **Syllabus Topic:** "The File System Interface, Mounting a file system, Directory Structure."
*   **Project Implementation:**
    *   **Root Filesystem:** You will implement `pivot_root` or `chroot`. This changes the logical "Root" (`/`) of the file system hierarchy for just one process.
    *   **Mounting:** You will programmatically `mount` the `/proc` filesystem. This is a special virtual filesystem that lets programs talk to the kernel.
    *   **Why it matters:** It proves you understand that the "File System" is just a logical tree that the OS presents to software, and it can be manipulated.

---

## 🎓 Exam/Viva Cheat Sheet

When a professor asks you about these topics, you can answer with **experience**:

| **Question** | **Theoretical Answer** | **Your "Mini-Docker" Answer** |
| :--- | :--- | :--- |
| *What is a Process?* | A program in execution. | "It's an instance that I created using `clone()`, giving it its own PID namespace so it thinks it's PID 1." |
| *What is a System Call?* | Interface between user and kernel. | "I used `nix::sched::clone` to trap into the kernel and request a new execution context." |
| *How does the OS manage resources?* | Scheduling and Memory Management. | "I interacted with the Cgroup Virtual Filesystem (`/sys/fs/cgroup`) to instruct the kernel to hard-limit the process's RAM usage." |
| *What is Mounting?* | Attaching a filesystem to the tree. | "I had to manually mount `/proc` inside my container so tools like `ps` could read process info from the kernel." |

---

### 🚀 Conclusion
Building this project is effectively **lab work for the entire semester** rolled into one application. You aren't just memorizing definitions; you are using the mechanisms the definitions describe.
