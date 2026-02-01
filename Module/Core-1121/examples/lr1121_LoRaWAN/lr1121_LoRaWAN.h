#ifdef LR1121_LORAWAN
#ifndef LR1121_LORAWAN_H
#define LR1121_LORAWAN_H
#include <stdio.h>
#include <stdint.h>
#include "lorawan_commissioning.h"
#include "lr1121_config.h"

extern bool user_button_is_press;

/*!
 * @brief Stringify constants
 */
#define xstr( a ) str( a )
#define str( a ) #a


/*!
 * @brief Helper macro that returned a human-friendly message if a command does not return LR1121_MODEM_RESPONSE_CODE_OK
 *
 * @remark The macro is implemented to be used with functions returning a @ref lr1121_modem_return_code_t
 *
 * @param[in] rc  Return code
 */

#define LOG_LEVEL 0 // Setting it to 0 means not printing, and setting it to 1 means printing.

#define ASSERT_SMTC_MODEM_RC( rc_func ) \
    do \
    { \
        lr1121_modem_response_code_t rc = rc_func; \
        if( LOG_LEVEL ) \
        { \
            if( rc == LR1121_MODEM_RESPONSE_CODE_NOT_INITIALIZED ) \
            { \
                printf( "In %s - %s (line %d): %s\n", __FILE__, __func__, __LINE__, xstr( LR1121_MODEM_RESPONSE_CODE_NOT_INITIALIZED ) ); \
            } \
            else if( rc == LR1121_MODEM_RESPONSE_CODE_INVALID ) \
            { \
                printf( "In %s - %s (line %d): %s\n", __FILE__, __func__, __LINE__, xstr( LR1121_MODEM_RESPONSE_CODE_INVALID ) ); \
            } \
            else if( rc == LR1121_MODEM_RESPONSE_CODE_BUSY ) \
            { \
                printf( "In %s - %s (line %d): %s\n", __FILE__, __func__, __LINE__, xstr( LR1121_MODEM_RESPONSE_CODE_BUSY ) ); \
            } \
            else if( rc == LR1121_MODEM_RESPONSE_CODE_FAIL ) \
            { \
                printf( "In %s - %s (line %d): %s\n", __FILE__, __func__, __LINE__, xstr( LR1121_MODEM_RESPONSE_CODE_FAIL ) ); \
            } \
            else if( rc == LR1121_MODEM_RESPONSE_CODE_NO_TIME ) \
            { \
                printf( "In %s - %s (line %d): %s\n", __FILE__, __func__, __LINE__, xstr( LR1121_MODEM_RESPONSE_CODE_NO_TIME ) ); \
            } \
            else if( rc == LR1121_MODEM_RESPONSE_CODE_NO_EVENT ) \
            { \
                printf( "In %s - %s (line %d): %s\n", __FILE__, __func__, __LINE__, xstr( LR1121_MODEM_RESPONSE_CODE_NO_EVENT ) ); \
            } \
        } \
    } while( 0 )

/**
 * @brief Watchdog counter reload value during sleep (The period must be lower than MCU watchdog period (here 20s))
 */
#define WATCHDOG_RELOAD_PERIOD_MS 20000

/**
 * @brief Periodical uplink alarm delay in seconds
 */
#define PERIODICAL_UPLINK_DELAY_S 10

#define EXTI_BUTTON 0

/*!
 * @brief User application data buffer size
 */
#define LORAWAN_APP_DATA_MAX_SIZE 242

/*!
 * @brief LoRaWAN regulatory region.
 * One of:
 * LR1121_LORAWAN_REGION_AS923_GRP1
 * LR1121_LORAWAN_REGION_AS923_GRP2
 * LR1121_LORAWAN_REGION_AS923_GRP3
 * LR1121_LORAWAN_REGION_AS923_GRP4
 * LR1121_LORAWAN_REGION_AU915
 * LR1121_LORAWAN_REGION_CN470
 * LR1121_LORAWAN_REGION_EU868
 * LR1121_LORAWAN_REGION_IN865
 * LR1121_LORAWAN_REGION_KR920
 * LR1121_LORAWAN_REGION_RU864
 * LR1121_LORAWAN_REGION_US915
 */
#define LORAWAN_REGION_USED LR1121_LORAWAN_REGION_EU868

void lr1121_LoRaWAN_test(void);

#endif  // LR1121_LORAWAN_H
#endif  // LR1121_LORAWAN
