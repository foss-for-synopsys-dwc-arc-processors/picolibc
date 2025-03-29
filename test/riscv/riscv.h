#define CSR_MSTATUS_NUMBER  0x300
#define CSR_MISA_NUMBER     0x301

#define _csr_read(csr_num, result) \
  __asm__ __volatile__("csrr %0, %1" : "=r"((result)) : "i"((csr_num)));

#define _csr_write(csr_num, data) \
  __asm__ __volatile__("csrw %0, %1" :: "i"(csr_num), "r"(data));
