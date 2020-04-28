#include "t124.h"
#include "types.h"

static inline void write32(uintptr_t addr, u32 val) {
    *(vu32 *)addr = val;
}

__attribute__((section(".init")))
void main() {
    // TODO: THIS DOESN'T WORK
    write32(APB_BASE + APB_MISC_PP_CONFIG_CTL_0, JTAG_ON);
    write32(SECURE_BOOT_BASE + SB_PFCFG_0, DEBUG_ON);
    while(1);
}

