#include "t124.h"
#include "types.h"

static inline void write32( uintptr_t addr, u32 val ) {
    *(vu32 *)addr = val;
}

__attribute__(( section(".init") ))
void main() {
    // TODO: THIS DOESN'T WORK
    write32( APB_BASE + APB_MISC_PP_CONFIG_CTL_0, JTAG_ON );
    while(1) {
        // Halt COP and wait for JTAG
        write32( FLOW_CTLR_BASE + FLOW_CTLR_HALT_COP_EVENTS_0,
                FLOW_CTLR_HALT_COP_FLOW_MODE_WAITEVENT |
                FLOW_CTLR_HALT_COP_JTAG );
    }
}

