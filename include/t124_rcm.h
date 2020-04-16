#ifndef _T124_RCM_H_
#define _T124_RCM_H_

#define JETSON_TK1_VID 0x0955
#define JETSON_TK1_PID 0x7140


#define IRAM_BEGIN              0x40000000
#define IRAM_END                0x40040000

#define BOOTROM_USB_BUF_1       0x40004000
#define BOOTROM_USB_BUF_2       0x40008000
#define BOOTROM_PAYLOAD_ENTRY   0x4000E000
#define BOOTROM_SMASH_TARGET    0x4000DCD8
#define BOOTROM_SMASH_LEN       (BOOTROM_SMASH_TARGET - BOOTROM_USB_BUF_2)

#define RCM_EP1_IN      0x81
#define RCM_EP1_OUT     0x01
#define RCM_CHIP_ID_LEN 0x10


#define RCM_CMD_BUF_LEN 0x7000
#define RCM_CMD_BUF_LEN 0x7000
#define RCM_CMD_BUF_PAYLOAD_INIT_POS 0x284
#define RCM_CMD_BUF_MEMCPY_RET_POS (RCM_CMD_BUF_PAYLOAD_INIT_POS + BOOTROM_SMASH_LEN - 0x30C - 0x4)

#define MAX_PAYLOAD_FILE_SIZE (RCM_CMD_BUF_MEMCPY_RET_POS - RCM_CMD_BUF_PAYLOAD_INIT_POS)

#endif
