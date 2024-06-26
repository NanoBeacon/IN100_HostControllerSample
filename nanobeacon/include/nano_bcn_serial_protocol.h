/**
 ****************************************************************************************
 *
 * @file nano_bcn_serial_protocol.h
 *
 * @brief Nano beacon uart communication header file
 *
 * Copyright (C) Inplay Technologies Inc. 2018-2020
 *
 ****************************************************************************************
 */
#ifndef NANO_BCN_SERIAL_PROTOCOL_H
#define NANO_BCN_SERIAL_PROTOCOL_H

#include <stdint.h>

/*
 * Enum
 ****************************************************************************************
 */

enum {
	UART_ERR_NO_ERROR,
	UART_ERR_TMO,
};

/// Error code 
enum {
	NANO_BCN_ERR_NO_ERROR,
	NANO_BCN_ERR_NOT_READY,
	NANO_BCN_ERR_INVALID_PARAM,
	NANO_BCN_ERR_BAD_RESP,
	NANO_BCN_ERR_UART,
	NANO_BCN_ERR_RX_TMO,

	NANO_BCN_ERR_ILLEGAL_TAIL_BYTE,
	NANO_BCN_ERR_ILLEGAL_CMD,
	NANO_BCN_ERR_UART_BI,
	NANO_BCN_ERR_UART_FE,
	NANO_BCN_ERR_ILLEGAL_ADDR,
	NANO_BCN_ERR_HCI_PARSE,
	NANO_BCN_ERR_UNKNOWN,
};


/*
 * Structure
 ****************************************************************************************
 */
typedef struct {
	void (*delay)(uint32_t msec);
	int (*serial_tx)(uint8_t *buf, uint16_t buf_len,  uint32_t tmo);
	int (*serial_rx)(uint8_t *buf, uint16_t buf_len,  uint32_t tmo);
	void (*serial_break)(int on);
} host_itf_t;


/*
 * APIs
 ****************************************************************************************
 */
#if defined(__cplusplus)
 extern "C" {                // Make sure we have C-declarations in C++ programs
#endif

int nano_bcn_read_reg(uint16_t adr, uint32_t* p_rd);
int nano_bcn_write_reg(uint16_t adr, uint32_t wd, uint32_t* p_rd);
int nano_bcn_read_mem(uint16_t adr, uint32_t* p_rd);
int nano_bcn_write_mem(uint16_t adr, uint32_t wd, uint32_t* p_rd);
int nano_bcn_read_efuse(uint8_t aid, uint16_t* p_rd);
int nano_bcn_write_efuse(uint8_t aid, uint16_t wd, uint16_t* p_rd);

/**
 * Usage:   Used to register host interface function and init
 * Params:  p_hif     host interface function
 * Return:  0 indicates success, others indicate failure
 */
int nano_bcn_uart_init(host_itf_t *p_hif);

/**
 * Usage:   Used to deinit
 * Params: 
 * Return: 
 */
void nano_bcn_uart_deinit(void);

/**
 * Usage:   Used to put the chip into sleep mode by UART break on signal
 * Params:
 * Return:
 */
void nano_bcn_sleep_by_uart_break_on();

/**
 * Usage:   Used to wakeup chip by UART break off signal
 * Params:
 * Return:
 */
void nano_bcn_wakeup_by_uart_break_off();


int nano_bcn_chip_reset(void);

int nano_bcn_uart_baud_rate_auto_detect(int* baud_rate);

#if defined(__cplusplus)
}     /* Make sure we have C-declarations in C++ programs */
#endif
#endif	// NANO_BCN_SERIAL_PROTOCOL_H
