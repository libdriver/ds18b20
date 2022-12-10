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
 * @file      driver_ds18b20_search_test.c
 * @brief     driver ds18b20 search test source file
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

#include "driver_ds18b20_search_test.h"

static ds18b20_handle_t gs_handle;        /**< ds18b20 handle */

/**
 * @brief  search test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t ds18b20_search_test(void)
{
    uint8_t res, i, num;
    uint8_t rom[3][8];
    ds18b20_info_t info;
   
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
   
    /* get ds18b20 info */
    res = ds18b20_info(&info);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print ds18b20 info */
        ds18b20_interface_debug_print("ds18b20: chip is %s.\n", info.chip_name);
        ds18b20_interface_debug_print("ds18b20: manufacturer is %s.\n", info.manufacturer_name);
        ds18b20_interface_debug_print("ds18b20: interface is %s.\n", info.interface);
        ds18b20_interface_debug_print("ds18b20: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ds18b20_interface_debug_print("ds18b20: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ds18b20_interface_debug_print("ds18b20: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ds18b20_interface_debug_print("ds18b20: max current is %0.2fmA.\n", info.max_current_ma);
        ds18b20_interface_debug_print("ds18b20: max temperature is %0.1fC.\n", info.temperature_max);
        ds18b20_interface_debug_print("ds18b20: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start search test */
    ds18b20_interface_debug_print("ds18b20: start search test.\n");
    
    /* ds18b20 init */
    res = ds18b20_init(&gs_handle);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: init failed.\n");
       
        return 1;
    }
    
    /* search rom */
    ds18b20_interface_debug_print("ds18b20: search rom...\n");
    
    /* max find 3 */
    num = 3;
    
    /* search rom */
    res = ds18b20_search_rom(&gs_handle, (uint8_t (*)[8])rom, (uint8_t *)&num);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: search rom failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: find %d rom.\n", num);
    
    /* print found rom */
    for (i = 0; i < num; i++)
    {
        char cmd_buf[128];
        
        memset(cmd_buf, 0, 128);
        strcat(cmd_buf, "ds18b20: rom ");
        for (uint8_t j=0; j<8; j++)
        {
            char hex_buf[6];
            
            memset((char *)hex_buf, 0 ,sizeof(char)*6);
            (void)snprintf((char *)hex_buf, 6, "%02X", rom[i][j]);
            strcat((char *)cmd_buf, (char *)hex_buf);
        }
        ds18b20_interface_debug_print("%s.\n",cmd_buf);
    }
    
    /* search alarm rom */
    ds18b20_interface_debug_print("ds18b20: search alarm rom...\n");
    
    /* max find 3 */
    num = 3;
    
    /* search alarm rom */
    res = ds18b20_search_alarm(&gs_handle, (uint8_t (*)[8])rom, (uint8_t *)&num);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: search alarm rom failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    
    /* print found rom */
    ds18b20_interface_debug_print("ds18b20: find %d alarm rom.\n", num);
    for (i = 0; i < num; i++)
    {
        char cmd_buf[128];
        
        memset(cmd_buf, 0, 128);
        strcat(cmd_buf, "ds18b20: alarm rom ");
        for (uint8_t j=0; j<8; j++)
        {
            char hex_buf[6];
            
            memset((char *)hex_buf, 0 ,sizeof(char)*6);
            (void)snprintf((char *)hex_buf, 6, "%02X", rom[i][j]);
            strcat((char *)cmd_buf, (char *)hex_buf);
        }
        ds18b20_interface_debug_print("%s.\n",cmd_buf);
    }
    
    /* finish search test */
    ds18b20_interface_debug_print("ds18b20: finish search test.\n");
    (void)ds18b20_deinit(&gs_handle);
    
    return 0;
}
