#define CSR_MSTATUS_NUMBER  0x300
#define CSR_MISA_NUMBER     0x301

inline unsigned long __attribute__((always_inline))
_csr_read(int csr_num)
{
  unsigned long result;
  __asm__ __volatile__("csrr %0, %1" : "=r"((result)) : "i"((csr_num)));
  return result;
}

inline void __attribute__((always_inline))
_csr_write(int csr_num, unsigned long data)
{
  __asm__ __volatile__("csrw %0, %1" :: "i"(csr_num), "r"(data));
}
