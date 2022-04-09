/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_ds18b20.h
 * @brief     driver ds18b20 header file
 * @version   2.0.0
 * @author    Shifeng Li
 * @date      2021-04-06
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/04/06  <td>2.0      <td>Shifeng Li  <td>format the code
 * <tr><td>2020/12/20  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_DS18B20_H
#define DRIVER_DS18B20_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ds18b20_driver ds18b20 driver function
 * @brief    ds18b20 driver modules
 * @{
 */

/**
 * @addtogroup ds18b20_advance_driver
 * @{
 */

/**
 * @brief ds18b20 max search size definition
 */
#ifndef DS18B20_MAX_SEARCH_SIZE
    #define DS18B20_MAX_SEARCH_SIZE 64        /**< max 64 devices */
#endif

/**
 * @}
 */

/**
 * @addtogroup ds18b20_base_driver
 * @{
 */

/**
 * @brief ds18b20 mode enumeration definition
 */
typedef enum
{
    DS18B20_MODE_SKIP_ROM  = 0x00,        /**< skip rom mode */
    DS18B20_MODE_MATCH_ROM = 0x01,        /**< match rom mode */
} ds18b20_mode_t;

/**
 * @brief ds18b20 power mode enumeration definition
 */
typedef enum
{
    DS18B20_POWER_MODE_PARASITE   = 0x00,        /**< parasite power mode */
    DS18B20_POWER_MODE_EXTERNALLY = 0x01,        /**< externally power mode */
} ds18b20_power_mode_t;

/**
 * @brief ds18b20 resolution enumeration definition
 */
typedef enum
{
    DS18B20_RESOLUTION_9BIT  = 0x00,        /**< 9 bit resolution */
    DS18B20_RESOLUTION_10BIT = 0x01,        /**< 10 bit resolution */
    DS18B20_RESOLUTION_11BIT = 0x02,        /**< 11 bit resolution */
    DS18B20_RESOLUTION_12BIT = 0x03,        /**< 12 bit resolution */
} ds18b20_resolution_t;

/**
 * @brief ds18b20 handle structure definition
 */
typedef struct ds18b20_handle_s
{
    uint8_t (*bus_init)(void);                              /**< point to a bus_init function address */
    uint8_t (*bus_deinit)(void);                            /**< point to a bus_deinit function address */
    uint8_t (*bus_read)(uint8_t *value);                    /**< point to a bus_read function address */
    uint8_t (*bus_write)(uint8_t value);                    /**< point to a bus_write function address */
    void (*delay_ms)(uint32_t ms);                          /**< point to a delay_ms function address */
    void (*delay_us)(uint32_t us);                          /**< point to a delay_us function address */
    void (*enable_irq)(void);                               /**< point to a enable_irq function address */
    void (*disable_irq)(void);                              /**< point to a disable_irq function address */
    void (*debug_print)(const char *const fmt, ...);        /**< point to a debug_print function address */
    uint8_t inited;                                         /**< inited flag */
    uint8_t mode;                                           /**< chip mode */
    uint8_t rom[8];                                         /**< chip mode */
} ds18b20_handle_t;

/**
 * @brief ds18b20 info structure definition
 */
typedef struct ds18b20_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} ds18b20_info_t;

/**
 * @}
 */

/**
 * @defgroup ds18b20_link_driver ds18b20 link driver function
 * @brief    ds18b20 link driver modules
 * @ingroup  ds18b20_driver
 * @{
 */

/**
 * @brief     initialize ds18b20_handle_t structure
 * @param[in] HANDLE points to a ds18b20 handle structure
 * @param[in] STRUCTURE is ds18b20_handle_t
 * @note      none
 */
#define DRIVER_DS18B20_LINK_INIT(HANDLE, STRUCTURE)   memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link bus_init function
 * @param[in] HANDLE points to a ds18b20 handle structure
 * @param[in] FUC points to a bus_init function address
 * @note      none
 */
#define DRIVER_DS18B20_LINK_BUS_INIT(HANDLE, FUC)    (HANDLE)->bus_init = FUC

/**
 * @brief     link bus_deinit function
 * @param[in] HANDLE points to a ds18b20 handle structure
 * @param[in] FUC points to a bus_deinit function address
 * @note      none
 */
#define DRIVER_DS18B20_LINK_BUS_DEINIT(HANDLE, FUC)  (HANDLE)->bus_deinit = FUC

/**
 * @brief     link bus_read function
 * @param[in] HANDLE points to a ds18b20 handle structure
 * @param[in] FUC points to a bus_read function address
 * @note      none
 */
#define DRIVER_DS18B20_LINK_BUS_READ(HANDLE, FUC)    (HANDLE)->bus_read = FUC

/**
 * @brief     link bus_write function
 * @param[in] HANDLE points to a ds18b20 handle structure
 * @param[in] FUC points to a bus_write function address
 * @note      none
 */
#define DRIVER_DS18B20_LINK_BUS_WRITE(HANDLE, FUC)   (HANDLE)->bus_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to a ds18b20 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_DS18B20_LINK_DELAY_MS(HANDLE, FUC)    (HANDLE)->delay_ms = FUC

/**
 * @brief     link delay_us function
 * @param[in] HANDLE points to a ds18b20 handle structure
 * @param[in] FUC points to a delay_us function address
 * @note      none
 */
#define DRIVER_DS18B20_LINK_DELAY_US(HANDLE, FUC)    (HANDLE)->delay_us = FUC

/**
 * @brief     link enable_irq function
 * @param[in] HANDLE points to a ds18b20 handle structure
 * @param[in] FUC points to a enable_irq function address
 * @note      none
 */
#define DRIVER_DS18B20_LINK_ENABLE_IRQ(HANDLE, FUC)  (HANDLE)->enable_irq = FUC

/**
 * @brief     link disable_irq function
 * @param[in] HANDLE points to a ds18b20 handle structure
 * @param[in] FUC points to a disable_irq function address
 * @note      none
 */
#define DRIVER_DS18B20_LINK_DISABLE_IRQ(HANDLE, FUC) (HANDLE)->disable_irq = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to a ds18b20 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_DS18B20_LINK_DEBUG_PRINT(HANDLE, FUC) (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup ds18b20_base_driver ds18b20 base driver function
 * @brief    ds18b20 base driver modules
 * @ingroup  ds18b20_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to a ds18b20 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ds18b20_info(ds18b20_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a ds18b20 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 bus initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 * @note      none
 */
uint8_t ds18b20_init(ds18b20_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to a ds18b20 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 bus deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds18b20_deinit(ds18b20_handle_t *handle);

/**
 * @brief      read data from the chip
 * @param[in]  *handle points to a ds18b20 handle structure
 * @param[out] *raw points to a raw adc buffer
 * @param[out] *temp points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds18b20_read(ds18b20_handle_t *handle, int16_t *raw, float *temp);

/**
 * @brief     set the chip mode
 * @param[in] *handle points to a ds18b20 handle structure
 * @param[in] mode is the chip mode
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds18b20_set_mode(ds18b20_handle_t *handle, ds18b20_mode_t mode);

/**
 * @brief      get the chip mode
 * @param[in]  *handle points to a ds18b20 handle structure
 * @param[out] *mode points to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds18b20_get_mode(ds18b20_handle_t *handle, ds18b20_mode_t *mode);

/**
 * @brief     set the handle rom
 * @param[in] *handle points to a ds18b20 handle structure
 * @param[in] *rom points to a rom buffer
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds18b20_set_rom(ds18b20_handle_t *handle, uint8_t rom[8]);

/**
 * @brief      get the chip rom
 * @param[in]  *handle points to a ds18b20 handle structure
 * @param[out] *rom points to a rom buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds18b20_get_rom(ds18b20_handle_t *handle, uint8_t rom[8]);

/**
 * @brief     set the resolution in the scratchpad
 * @param[in] *handle points to a ds18b20 handle structure
 * @param[in] resolution is the chip resolution
 * @return    status code
 *            - 0 success
 *            - 1 scratchpad set resolution failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds18b20_scratchpad_set_resolution(ds18b20_handle_t *handle, ds18b20_resolution_t resolution);

/**
 * @brief      get the resolution in the scratchpad
 * @param[in]  *handle points to a ds18b20 handle structure
 * @param[out] *resolution points to a chip resolution buffer
 * @return     status code
 *             - 0 success
 *             - 1 scratchpad get resolution failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds18b20_scratchpad_get_resolution(ds18b20_handle_t *handle, ds18b20_resolution_t *resolution);

/**
 * @brief     copy the scratchpad content to the eeprom
 * @param[in] *handle points to a ds18b20 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 copy scratchpad to eeprom failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds18b20_copy_scratchpad_to_eeprom(ds18b20_handle_t *handle);

/**
 * @brief     copy the eeprom content to the scratchpad
 * @param[in] *handle points to a ds18b20 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 copy eeprom to scratchpad failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds18b20_copy_eeprom_to_scratchpad(ds18b20_handle_t *handle);

/**
 * @}
 */

/**
 * @defgroup ds18b20_advance_driver ds18b20 advance driver function
 * @brief    ds18b20 advance driver modules
 * @ingroup  ds18b20_driver
 * @{
 */

/**
 * @brief     set the alarm threshold in the scratchpad
 * @param[in] *handle points to a ds18b20 handle structure
 * @param[in] threshold_high is the high threshold
 * @param[in] threshold_low is the low threshold
 * @return    status code
 *            - 0 success
 *            - 1 scratchpad set alarm threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds18b20_scratchpad_set_alarm_threshold(ds18b20_handle_t *handle, int8_t threshold_high, int8_t threshold_low);

/**
 * @brief      get the alarm threshold in the scratchpad
 * @param[in]  *handle points to a ds18b20 handle structure
 * @param[out] *threshold_high points to a high threshold buffer
 * @param[out] *threshold_low points to a low threshold buuffer
 * @return     status code
 *             - 0 success
 *             - 1 scratchpad get alarm threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds18b20_scrachpad_get_alarm_threshold(ds18b20_handle_t *handle, int8_t *threshold_high, int8_t *threshold_low);

/**
 * @brief      convert the alarm temperature to the register data
 * @param[in]  *handle points to a ds18b20 handle structure
 * @param[in]  temp is the alarm temperature
 * @param[out] *reg points to a register data
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds18b20_alarm_convert_to_register(ds18b20_handle_t *handle, float temp, int8_t *reg);

/**
 * @brief      convert the register data to the alarm temperature
 * @param[in]  *handle points to a ds18b20 handle structure
 * @param[in]  reg is the register data
 * @param[out] *temp points to a alarm temperature buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds18b20_alarm_convert_to_data(ds18b20_handle_t *handle, int8_t reg, float *temp);

/**
 * @brief         search the ds18b20 rom
 * @param[in]     *handle points to a ds18b20 handle structure
 * @param[out]    **rom points to a rom array
 * @param[in,out] *num points to a array size buffer
 * @return        status code
 *                - 0 success
 *                - 1 search rom failed
 * @note          none
 */
uint8_t ds18b20_search_rom(ds18b20_handle_t *handle, uint8_t (*rom)[8], uint8_t *num);

/**
 * @brief         search the ds18b20 alarm rom
 * @param[in]     *handle points to a ds18b20 handle structure
 * @param[out]    **rom points to a rom array
 * @param[in,out] *num points to a array size buffer
 * @return        status code
 *                - 0 success
 *                - 1 search alarm failed
 * @note          none
 */
uint8_t ds18b20_search_alarm(ds18b20_handle_t *handle, uint8_t (*rom)[8], uint8_t *num);

/**
 * @brief      get the power mode
 * @param[in]  *handle points to a ds18b20 handle structure
 * @param[out] *power_mode points to a power mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get power mode failed
 * @note       none
 */
uint8_t ds18b20_get_power_mode(ds18b20_handle_t *handle, ds18b20_power_mode_t *power_mode);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
