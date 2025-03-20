#include <stdlib.h>
#include <stdio.h>
#include "riscv.h"

int main(void) {
    unsigned long mstatus = _csr_read(CSR_MSTATUS_NUMBER);
    unsigned long misa = _csr_read(CSR_MISA_NUMBER);
    unsigned long mstatus_vs = (mstatus >> 9) & 3;
    unsigned long misa_v = (misa >> 21) & 1;

    if (misa_v != 1) {
        printf("SKIP: this target does not have V\n");
        exit(77);
    }

    if (mstatus_vs == 0) {
        printf("FAIL: mstatus.VS[1:0] == %lu, mstatus == %lx\n", mstatus_vs, mstatus);
        exit(1);
    }

    printf("PASS: mstatus.VS[1:0] != 0 as expected\n");

    exit(0);
}
