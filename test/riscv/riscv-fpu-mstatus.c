#include <stdlib.h>
#include <stdio.h>
#include "riscv.h"

int __attribute__((target("arch=+zicsr")))
main(void) {
    unsigned long mstatus = 0;
    unsigned long misa = 0;
    unsigned long mstatus_fs = (mstatus >> 13) & 3;
    unsigned long misa_f = (misa >> 5) & 1;
    unsigned long misa_d = (misa >> 3) & 1;

    _csr_read(CSR_MSTATUS_NUMBER, mstatus);
    _csr_read(CSR_MISA_NUMBER, misa);

    if ((misa_f | misa_d) != 1) {
        printf("SKIP: this target does not have FPU\n");
        exit(77);
    }

    if (mstatus_fs == 0) {
        printf("FAIL: mstatus.FS[1:0] == %lu, mstatus == %lx\n", mstatus_fs, mstatus);
        exit(1);
    }

    printf("PASS: mstatus.FS[1:0] != 0 as expected\n");

    exit(0);
}
