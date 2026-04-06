# Carapace 🐢

> **Note:** This project is a proof of concept (PoC) for a containerized environment built purely for educational purposes. **It is NOT meant for real-world or production use cases.**

Carapace is a lightweight, minimalistic container runtime written in C. It demonstrates how modern Linux container tools (like Docker) work under the hood by manually setting up isolation and resource limits for processes. 

## ✨ Features
- **Process Isolation**: Uses Linux Namespaces (`CLONE_NEWPID`, `CLONE_NEWUTS`, `CLONE_NEWNS`) to isolate the process ID, hostname, and mount points.
- **Resource Limiting**: Utilizes `cgroups` (Control Groups) to set strict memory limits on the containerized process, automatically killing it if it goes out of memory (OOM).
- **Filesystem Isolation**: Implements a jailed filesystem using `chroot`, confining the process to a specified `rootfs` directory.

## 🛠️ Project Structure
- `src/main.c`: The entry point and CLI argument parser.
- `src/container.c`: Handles the core container initialization and process forking (`clone`).
- `src/namespaces.c`: Configures the child process, performs `chroot`, and executes the target command.
- `src/cgroups.c`: Manages resource allocation via `/sys/fs/cgroup`.
- `Makefile`: Simple build automation.

## 🚀 Usage

### Building
To compile the project, just run:
```bash
make
```

### Running
You need `sudo` privileges because creating namespaces and modifying cgroups requires root access.

```bash
sudo ./build/carapace --mem <limit>M [--root <path>] <command> [args...]
```

**Parameters:**
- `--mem <limit>M`: Set the maximum memory limit (e.g., `50M` for 50 Megabytes).
- `--root <path>`: (Optional) Specify the root filesystem path. Defaults to `rootfs`.
- `<command>`: The command to execute inside the container (e.g., `/bin/sh`).

### Example
Assuming you have a valid Linux filesystem setup in a `rootfs` folder:
```bash
sudo ./build/carapace --mem 100M --root ./rootfs /bin/sh
```

## 🧹 Cleanup
To clean up the compiled binaries:
```bash
make clean
```
