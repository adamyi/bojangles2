#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "threads.h"
#include "clash.h"
#include "parse.h"

int main(int argc, const char *argv[]) {
    printf("Threads: %d\n", num_threads());
    uchar a[10];
    memset(a, 1, sizeof(a));
    int res = count_bits_asm(a, 10);
    printf("%d\n", res);

    getClassesFromAdminData("classes");
}
