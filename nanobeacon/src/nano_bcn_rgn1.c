#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "nano_bcn_rgn1.h"
#include "nano_bcn_serial_protocol.h"

static uint16_t region_one_data[58];

void bcn_rgn1_data_reset(void)
{
	memset(region_one_data, 0, sizeof(region_one_data));
	bcn_rgn1_set_value(1, RGN1_SLEEP_AFT_UART_BI_MASK, RGN1_SLEEP_AFT_UART_BI_SHIFT, RGN1_SLEEP_AFT_UART_BI_OFFSET);
	bcn_rgn1_set_value(30, RGN1_TX_PKT_GAP_MASK, RGN1_TX_PKT_GAP_SHIFT, RGN1_TX_PKT_GAP_OFFSET);
	bcn_rgn1_set_value(4, RGN1_CAL_FREQ_MASK, RGN1_CAL_FREQ_SHIFT, RGN1_CAL_FREQ_OFFSET);
	//bcn_rgn1_set_value(1, RGN1_RGN2_PRESENT_MASK, RGN1_RGN2_PRESENT_SHIFT, RGN1_RGN2_PRESENT_OFFSET);
	//bcn_rgn1_set_value(1, RGN1_RGN3_PRESENT_MASK, RGN1_RGN3_PRESENT_SHIFT, RGN1_RGN3_PRESENT_OFFSET);
}

void bcn_rgn1_set_value(uint16_t value, uint16_t mask, uint8_t shift, uint8_t offset)
{
	region_one_data[offset] &= (~(mask << shift));
	region_one_data[offset] |= ((value & mask) << shift);
}

int bcn_rgn1_load_to_ram(void)
{
	int i, ret;
	uint16_t* pdata;
	uint32_t value;
	uint32_t tmp;
	uint16_t mem_address = 0x20;
	uint16_t reg_address = 0x10E0;
	/*setup region 1*/
	pdata = &region_one_data[0];
	ret = nano_bcn_read_reg(0x10E0, &tmp);
	if (0 != ret) {
		return ret;
	}
	tmp = tmp & ((1 << 17) | (1 << 24));
	value = pdata[0] | (pdata[1] << 16);
	value = value & (~((1 << 17) | (1 << 24)));
    value = value & (~(1<<25));
	value |= tmp;
	ret += nano_bcn_write_mem(mem_address, value, NULL);
	ret += nano_bcn_write_reg(reg_address, value, NULL);
	mem_address = mem_address + 1;
	reg_address = reg_address + 4;
	for (i = 2; i < 58; ) {
		value = pdata[i] | (pdata[i + 1] << 16);
		ret += nano_bcn_write_mem(mem_address, value, NULL);
		ret += nano_bcn_write_reg(reg_address, value, NULL);
		mem_address = mem_address + 1;
		reg_address = reg_address + 4;
		i += 2;
	}
	return ret;
}

int bcn_rgn1_brun(void)
{
	int ret = 0;
	uint8_t addr = 16;
	for (int i = 0; i < (sizeof(region_one_data) / sizeof(uint16_t)); i++) {
		ret += nano_bcn_write_efuse(addr++, region_one_data[i], NULL);
	}
	return ret;
}


int bcn_rgn1_is_sleep_aft_tx(void)
{
    uint32_t val;
    val = region_one_data[0]|(region_one_data[1] << 16);
    return (val & (1 << 25));
}

