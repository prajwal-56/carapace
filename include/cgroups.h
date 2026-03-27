#ifndef CGROUPS_H
#define CGROUPS_H
#include <sys/types.h>

void cgroups_init(pid_t pid, long memory_limit_in_bytes);

#endif