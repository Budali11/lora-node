/*****************************************************************************
 * | File      	 :   wavesahre_lora_1121.h
 * | Author      :   Waveshare team
 * | Function    :   Hardware underlying interface
 * | Info        :
 *                   
 *----------------
 * |This version :   V1.0
 * | Date        :   2025-06-27
 * | Info        :   Basic version
 *
 ******************************************************************************/
#ifndef WAVESHARE_LORA_SPI_H
#define WAVESHARE_LORA_SPI_H

#include <string.h>
#include "main.h"
#include "module_redirect.h"
// LR11XX driver headers
#include "lr11xx_driver/lr11xx_bootloader.h"
#include "lr11xx_driver/lr11xx_hal.h"
#include "lr11xx_driver/lr11xx_system.h"
#include "lr11xx_driver/lr11xx_radio.h"
#include "lr11xx_driver/lr11xx_regmem.h"
#include "lr11xx_driver/lr11xx_lr_fhss.h"
#include "lr11xx_driver/lr11xx_driver_version.h"

// LR1121 modem headers
#include "lr1121_modem/lr1121_modem_helper.h"
#include "lr1121_modem/lr1121_modem_system_types.h"
#include "lr1121_modem/lr1121_modem_common.h"
#include "lr1121_modem/lr1121_modem_modem.h"
#include "lr1121_modem/lr1121_modem_hal.h"
#include "lr1121_modem/lr1121_modem_system.h"
#include "lr1121_modem/lr1121_modem_bsp.h"
#include "lr1121_modem/lr1121_modem_radio.h"

// Debug string printers for LR1121
#include "lr1121_printers/lr11xx_bootloader_types_str.h"
#include "lr1121_printers/lr11xx_crypto_engine_types_str.h"
#include "lr1121_printers/lr11xx_lr_fhss_types_str.h"
#include "lr1121_printers/lr11xx_radio_types_str.h"
#include "lr1121_printers/lr11xx_rttof_types_str.h"
#include "lr1121_printers/lr11xx_system_types_str.h"
#include "lr1121_printers/lr11xx_types_str.h"
#include "lr1121_printers/lr11xx_printf_info.h"
#include "lr1121_printers/lr1121_modem_printf_info.h"

// Common definitions
#include "lr1121_common/lr1121_common.h"

//#define USE_LR11XX_CRC_OVER_SPI // Optional: Enable CRC check for SPI transfers

/**
 * @brief Structure representing the LR1121 radio pin mapping and SPI interface
 */
typedef struct lr1121_s
{
	uint16_t reset;  ///< Reset pin
	uint16_t busy;   ///< Busy status pin
	uint16_t irq;    ///< Interrupt request pin
	uint16_t mosi;   ///< SPI MOSI pin
	uint16_t miso;   ///< SPI MISO pin
	uint16_t clk;    ///< SPI CLK pin
	uint16_t cs;     ///< SPI Chip Select pin
	uint16_t led;    ///< Optional status LED pin
    // spi_device_handle_t spi; ///< SPI handle (optional, unused in this implementation)
} lr1121_t;

/**
 * @brief Initializes the radio IO context (assign pin numbers to context)
 *
 * @param [in] context Pointer to lr1121_t structure
 */
void lora_init_io_context(const void* context );

/**
 * @brief Configures GPIO modes for the radio IO pins (input/output)
 *
 * @param [in] context Pointer to lr1121_t structure
 */
void lora_init_io( const void* context );

/**
 * @brief Writes data over SPI to the LR1121
 *
 * @param [in] context Pointer to lr1121_t structure
 * @param [in] wirte Pointer to byte buffer to write
 * @param [in] wirte_length Number of bytes to write
 */
void lora_spi_write_bytes(const void* context, const uint8_t *wirte, const uint16_t wirte_length);

/**
 * @brief Reads data over SPI from the LR1121
 *
 * @param [in] context Pointer to lr1121_t structure
 * @param [out] read Pointer to buffer to store received data
 * @param [in] read_length Number of bytes to read
 */
void lora_spi_read_bytes(const void* context, uint8_t *read,const uint16_t read_length);

/**
 * @brief Clears all modem events by polling until the queue is empty
 *
 * @param [in] context Pointer to lr1121_t structure
 * @returns LR1121 modem response code indicating status
 */
lr1121_modem_response_code_t lr1121_modem_board_event_flush( const void* context );

#endif // WAVESHARE_LORA_SPI_H
