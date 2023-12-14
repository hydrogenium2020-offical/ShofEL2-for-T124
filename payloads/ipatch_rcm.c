/* Based on similar payload by ktemkin
 * https://gist.github.com/ktemkin/825d5f4316f63a7c11ea851a2022415a
 */

#include "types.h"
#include "t124.h"

#define _REG(base, off) *(volatile unsigned int *)((base) + (off))
#define reg_write(base, off, value) _REG(base, off) = value
#define reg_clear(base, off, value) _REG(base, off) &= ~value
#define reg_set(base, off, value) _REG(base, off) |= value

/**
 * Patches over a given address in the IROM using the IPATCH hardware.
 */
void ipatch_word(u8 slot, u32 addr, u16 new_value)
{
	u32 slot_value;
	u32 offset;

	// Mark the relevant ipatch slot as not-in-use.
	reg_clear(IPATCH_BASE, IPATCH_SELECT, (1 << slot));

	// Compute the new patch value.
	offset = (addr & 0xFFFF) >> 1;
	slot_value = (offset << 16) | new_value;

	// Figure out the location of the slot to touch.
	reg_write(IPATCH_BASE, IPATCH_REGS + (slot * 4), slot_value);

	// Apply the new one.
	reg_set(IPATCH_BASE, IPATCH_SELECT, (1 << slot));
}

__attribute__((section(".init")))
void entry() {
	ipatch_word(1, BOOTROM_GET_SECURITY_MODE, 0x2000);

	register entry_point entry = (entry_point) ( 0x00101128 | 1 );
	entry();
}