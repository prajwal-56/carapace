#include "cgroups.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Creates a folder inside cgroups 
void cgroups_init(pid_t pid, long memory_limit_in_bytes){

    // create a new cgroup directory for the container
    mkdir("/sys/fs/cgroup/carapace", 0755);

    const char *memory_max_path = "/sys/fs/cgroup/carapace/memory.max";
    const char *cgroup_procs_path = "/sys/fs/cgroup/carapace/cgroup.procs";

    // writing into memory.max
    FILE *memMaxFp = fopen(memory_max_path , "w");
    if (memMaxFp == NULL){
        perror("Couldn't allocate memory or open **memory.max** file. Try running with sudo :(");
        return;
    } 
    fprintf(memMaxFp, "%ld", memory_limit_in_bytes);    
    fclose(memMaxFp);
    printf("---- memory limit set to %ld bytes -----\n", memory_limit_in_bytes);
    printf("---- modified %s -----\n", memory_max_path);

    // writing into cgroup.procs
    FILE *cgroupProcsFp = fopen(cgroup_procs_path, "w");
    if( cgroupProcsFp == NULL){
        perror("Couldn't open **cgroup.procs** file. Try running with sudo :(");
        return;
    }
    fprintf(cgroupProcsFp , "%d", pid);
    fclose(cgroupProcsFp);
    printf("---- added PID %d to %s -----\n", pid , cgroup_procs_path);
}
