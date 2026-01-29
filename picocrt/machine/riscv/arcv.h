#if defined(CRT0_ARCV)
#define CSR_NUM_MSTATUS                  0x300
#define MSTATUS_VS_OFFSET                0x9
#define MSTATUS_VS_MASK                  0x3

#define CSR_NUM_ARCV_MCACHE_CTRL         0x7C8
#define ARCV_MCACHE_CTRL_IC_EN_OFFSET    0x0
#define ARCV_MCACHE_CTRL_DC_EN_OFFSET    0x8
#define ARCV_MCACHE_CTRL_DC_L0_EN_OFFSET 0xC
#define ARCV_MCACHE_CTRL_L2_EN_OFFSET    0x10

#define CSR_NUM_ARCV_RVV_BUILD           0xFC4
#define ARCV_RVV_BUILD_V_OPTION_OFFSET   0x0
#define ARCV_RVV_BUILD_V_OPTION_MASK     0x7

inline unsigned long __attribute__((always_inline, target("arch=+zicsr")))
_arcv_csr_read(int csr_num)
{
    unsigned long result;
    __asm__ __volatile__("csrr %0, %1" : "=r"((result)) : "i"((csr_num)));
    return result;
}

inline void __attribute__((always_inline, target("arch=+zicsr")))
_arcv_csr_write(int csr_num, unsigned long data)
{
    __asm__ __volatile__("csrw %0, %1" ::"i"(csr_num), "r"(data));
}

static void __section(".init") __used __attribute__((target("arch=+zicsr")))
_arcv_cache_enable()
{
    unsigned long mcache = _arcv_csr_read(CSR_NUM_ARCV_MCACHE_CTRL);
    mcache |= (1 << ARCV_MCACHE_CTRL_IC_EN_OFFSET) | (1 << ARCV_MCACHE_CTRL_DC_EN_OFFSET)
        | (1 << ARCV_MCACHE_CTRL_DC_L0_EN_OFFSET) | (1 << ARCV_MCACHE_CTRL_L2_EN_OFFSET);
    _arcv_csr_write(CSR_NUM_ARCV_MCACHE_CTRL, mcache);
}

static void __section(".init") __used __attribute__((target("arch=+zicsr")))
_arcv_vector_enable()
{
    unsigned long rvv_build = _arcv_csr_read(CSR_NUM_ARCV_RVV_BUILD);
    unsigned long v_option
        = (rvv_build >> ARCV_RVV_BUILD_V_OPTION_OFFSET) & ARCV_RVV_BUILD_V_OPTION_MASK;

    if (v_option != 0) {
        unsigned long mstatus = _arcv_csr_read(CSR_NUM_MSTATUS);
        mstatus |= (1 << MSTATUS_VS_OFFSET);
        _arcv_csr_write(CSR_NUM_MSTATUS, mstatus);
    }
}
#endif
