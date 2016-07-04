#include <hal.h>
#include "pic.h"

#define I86_PIC1_REG_COMMAND 0x20
#define I86_PIC1_REG_STATUS 0x20
#define I86_PIC1_REG_DATA 0x21
#define I86_PIC1_REG_IMR 0x21

#define I86_PIC2_REG_COMMAND 0xA0
#define I86_PIC2_REG_STATUS 0xA0
#define I86_PIC2_REG_DATA 0xA1
#define I86_PIC2_REG_IMR 0xA1


#define I86_PIC_ICW1_MASK_IC4 0x1
#define I86_PIC_ICW1_MASK_SNGL 0x2
#define I86_PIC_ICW1_MASK_ADI 0x4
#define I86_PIC_ICW1_MASK_LTIM 0x8
#define I86_PIC_ICW1_MASK_INIT 0x10

#define I86_PIC_ICW4_MASK_UPM 0x1
#define I86_PIC_ICW4_MASK_AEOI 0x2
#define I86_PIC_ICW4_MASK_MS 0x4
#define I86_PIC_ICW4_MASK_BUF 0x8
#define I86_PIC_ICW4_MASK_SFNM 0x10


#define I86_PIC_ICW1_IC4_EXPECT 1
#define I86_PIC_ICW1_IC4_NO 0
#define I86_PIC_ICW1_SNGL_YES 2
#define I86_PIC_ICW1_SNGL_NO 0
#define I86_PIC_ICW1_ADI_CALLINTERVAL4 4
#define I86_PIC_ICW1_ADI_CALLITNERVAL8 0
#define I86_PIC_ICW1_LTIM_LEVELTRIGGERED 8
#define I86_PIC_ICW1_LTIM_EDGETRIGGERED 0
#define I86_PIC_ICW1_INIT_YES 0x10
#define I86_PIC_ICW1_INIT_NO 0

#define I86_PIC_ICW4_UPM_86MODE 1
#define I86_PIC_ICW4_UPM_MCSMODE 0
#define I86_PIC_ICW4_AEOI_AUTOEOI 2
#define I86_PIC_ICW4_AEOI_NOAUTOEOI 0
#define I86_PIC_ICW4_MS_BUFFERMASTER 4
#define I86_PIC_ICW4_MS_BUFFERSLAVE 0
#define I86_PIC_ICW4_BUF_MODEYES 8
#define I86_PIC_ICW4_BUF_MODENO 0
#define I86_PIC_ICW4_SFNM_NESTEDMODE 0x10
#define I86_PIC_ICW4_SFNM_NOTNESTED 0

inline void i86_pic_send_command(uint8_t cmd, uint8_t picNum) {
    if (picNum > 1) return;

    uint8_t reg = (picNum == 1) ? I86_PIC2_REG_COMMAND : I86_PIC1_REG_COMMAND;
    outportb(reg, cmd);
}

inline void i86_pic_send_data(uint8_t data, uint8_t picNum) {
    if (picNum > 1) return;

    uint8_t reg = (picNum == 1) ? I86_PIC2_REG_DATA : I86_PIC1_REG_DATA;
    outportb(reg, data);
}

inline uint8_t i86_pic_read_data(uint8_t picNum) {
    if (picNum > 1) return 0;

    uint8_t reg = (picNum == 1) ? I86_PIC2_REG_DATA : I86_PIC1_REG_DATA;
    return inportb(reg);
}

void i86_pic_initialize(uint8_t base0, uint8_t base1) {
    uint8_t icw = 0;

    disable();

    icw = (icw & ~I86_PIC_ICW1_MASK_INIT) | I86_PIC_ICW1_INIT_YES;
    icw = (icw & I86_PIC_ICW1_MASK_IC4) | I86_PIC_ICW1_IC4_EXPECT;

    i86_pic_send_command(icw, 0);
    i86_pic_send_command(icw, 1);

    i86_pic_send_data(0x04, 0);
    i86_pic_send_data(0x02, 1);

    icw = (icw & I86_PIC_ICW4_MASK_UPM) | I86_PIC_ICW4_UPM_86MODE;

    i86_pic_send_data(icw, 0);
    i86_pic_send_data(icw, 1);
}