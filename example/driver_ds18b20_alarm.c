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
 * @file      driver_ds18b20_alarm.c
 * @brief     driver ds18b20 alarm source file
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

#include "driver_ds18b20_alarm.h"

static ds18b20_handle_t gs_handle;        /**< ds18b20 handle */

/**
 * @brief  alarm example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t ds18b20_alarm_init(void)
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_DS18B20_LINK_INIT(&gs_handle, ds18b20_handle_t);
    DRIVER_DS18B20_LINK_BUS_INIT(&gs_handle, ds18b20_interface_init);
    DRIVER_DS18B20_LINK_BUS_DEINIT(&gs_handle, ds18b20_interface_deinit);
    DRIVER_DS18B20_LINK_BUS_READ(&gs_handle, ds18b20_interface_read);
    DRIVER_DS18B20_LINK_BUS_WRITE(&gs_handle, ds18b20_interface_write);
    DRIVER_DS18B20_LINK_DELAY_MS(&gs_handle, ds18b20_interface_delay_ms);
    DRIVER_DS18B20_LINK_DELAY_US(&gs_handle, ds18b20_interface_delay_us);
    DRIVER_DS18B20_LINK_ENABLE_IRQ(&gs_handle, ds18b20_interface_enable_irq);
    DRIVER_DS18B20_LINK_DISABLE_IRQ(&gs_handle, ds18b20_interface_disable_irq);
    DRIVER_DS18B20_LINK_DEBUG_PRINT(&gs_handle, ds18b20_interface_debug_print);
    
    /* ds18b20 init */
    res = ds18b20_init(&gs_handle);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: init failed.\n");
        
        return 1;
    }   
    
    /* set match rom mode */
    res = ds18b20_set_mode(&gs_handle, DS18B20_MODE_MATCH_ROM);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: set mode failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief     alarm example set the threshold
 * @param[in] *rom points to a rom buffer
 * @param[in] low is the low threshold
 * @param[in] high is the high threshold
 * @return     status code
 *             - 0 success
 *             - 1 set threshold failed
 * @note       none
 */
uint8_t ds18b20_alarm_set_threshold(uint8_t rom[8], float low, float high)
{
    uint8_t res;
    int8_t threshold_high;
    int8_t threshold_low;
    
    /* set rom */
    res = ds18b20_set_rom(&gs_handle, (uint8_t *)rom);
    if (res != 0)
    {
        return 1;
    }
    
    /* convert high threshold */
    res = ds18b20_alarm_convert_to_register(&gs_handle, high, (int8_t *)&threshold_high);
    if (res != 0)
    {
        return 1;
    }
    
    /* convert low threshold */
    res = ds18b20_alarm_convert_to_register(&gs_handle, low, (int8_t *)&threshold_low);
    if (res != 0)
    {
        return 1;
    }
    
    /* set alarm threshold */
    res = ds18b20_scratchpad_set_alarm_threshold(&gs_handle, threshold_high, threshold_low);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      alarm example get the threshold
 * @param[in]  *rom points to a rom buffer
 * @param[out] *low points to a low threshold
 * @param[out] *high points to a high threshold
 * @return      status code
 *              - 0 success
 *              - 1 get threshold failed
 * @note        none
 */
uint8_t ds18b20_alarm_get_threshold(uint8_t rom[8], float *low, float *high)
{
    uint8_t res;
    int8_t threshold_high;
    int8_t threshold_low;
    
    /* set rom */
    res = ds18b20_set_rom(&gs_handle, (uint8_t *)rom);
    if (res != 0)
    {
        return 1;
    }
    
    /* get alarm threshold */
    res = ds18b20_scrachpad_get_alarm_threshold(&gs_handle, (int8_t *)&threshold_high, (int8_t *)&threshold_low);
    if (res != 0)
    {
        return 1;
    }
    
    /* convert low threshold */
    res = ds18b20_alarm_convert_to_data(&gs_handle, threshold_low, low);
    if (res != 0)
    {
        return 1;
    }
    
    /* convert high threshold */
    res = ds18b20_alarm_convert_to_data(&gs_handle, threshold_high, high);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief         alarm example find
 * @param[in]     *rom points to a rom buffer
 * @param[in,out] *num points to a number buffer
 * @return        status code
 *                - 0 success
 *                - 1 search failed
 * @note          none
 */
uint8_t ds18b20_alarm_search(uint8_t (*rom)[8], uint8_t *num)
{
    /* search alarm */
    if (ds18b20_search_alarm(&gs_handle, rom, num) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  alarm example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ds18b20_alarm_deinit(void)
{
    /* ds18b20 close */
    if (ds18b20_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
