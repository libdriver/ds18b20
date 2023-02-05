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
 * @file      driver_ds18b20_register_test.c
 * @brief     driver ds18b20 register test source file
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

#include "driver_ds18b20_register_test.h"
#include <stdlib.h>

static ds18b20_handle_t gs_handle;        /**< ds18b20 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t ds18b20_register_test(void)
{
    uint8_t res, i;
    float temp, temp_check;
    int8_t reg;
    int8_t threshold_high, threshold_low;
    int8_t threshold_high_check, threshold_low_check;
    uint8_t rom[8], rom_check[8];
    ds18b20_info_t info;
    ds18b20_mode_t mode;
    ds18b20_power_mode_t power_mode;
    ds18b20_resolution_t resolution;
    
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
    
    /* start register test */
    ds18b20_interface_debug_print("ds18b20: start register test.\n");
    
    /* ds18b20 init */
    res = ds18b20_init(&gs_handle);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: init failed.\n");
       
        return 1;
    }
    
    /* ds18b20_set_mode/ds18b20_get_mode test */
    ds18b20_interface_debug_print("ds18b20: ds18b20_set_mode/ds18b20_get_mode test.\n");
    
    /* skip rom */
    res = ds18b20_set_mode(&gs_handle, DS18B20_MODE_SKIP_ROM);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: set mode failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: set skip rom mode.\n");
    res = ds18b20_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: get mode failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: check mode %s.\n", mode==DS18B20_MODE_SKIP_ROM?"ok":"error");
    
    /* match rom */
    res = ds18b20_set_mode(&gs_handle, DS18B20_MODE_MATCH_ROM);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: set mode failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: set match rom mode.\n");
    res = ds18b20_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: get mode failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: check mode %s.\n", mode==DS18B20_MODE_MATCH_ROM?"ok":"error");
    
    /* ds18b20_set_rom/ds18b20_get_rom test */
    ds18b20_interface_debug_print("ds18b20: ds18b20_set_rom/ds18b20_get_rom test.\n");
    res = ds18b20_get_rom(&gs_handle, (uint8_t *)rom);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: get rom failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    res = ds18b20_set_rom(&gs_handle, (uint8_t *)rom);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: set mode failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: set rom.\n");
    res = ds18b20_get_rom(&gs_handle, (uint8_t *)rom_check);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: get rom failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    for (i=0; i<8; i++)
    {
        if (rom[i] != rom_check[i])
        {
            res = 1;
        }
    }
    ds18b20_interface_debug_print("ds18b20: check rom %s.\n", (res==0)?"ok":"error");
    
    /* ds18b20_get_power_mode test */
    ds18b20_interface_debug_print("ds18b20: ds18b20_get_power_mode test.\n");
    res = ds18b20_get_power_mode(&gs_handle, &power_mode);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: get power mode failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: check power mode %s.\n",
                                  power_mode == DS18B20_POWER_MODE_EXTERNALLY ? "externally power" : "parasite power");
    
    /* ds18b20_scrachpad_set_resolution/ds18b20_scrachpad_get_resolution test */
    ds18b20_interface_debug_print("ds18b20: ds18b20_scrachpad_set_resolution/ds18b20_scrachpad_get_resolution test.\n");
    
    /* RESOLUTION_9BIT */
    res = ds18b20_scratchpad_set_resolution(&gs_handle, DS18B20_RESOLUTION_9BIT);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: scratchpad set resolution failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: scratchpad set resolution 9bit.\n");
    res = ds18b20_scratchpad_get_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: scratchpad get resolution failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: check resolution %s.\n", resolution==DS18B20_RESOLUTION_9BIT?"ok":"error");
   
    /* RESOLUTION_10BIT */
    res = ds18b20_scratchpad_set_resolution(&gs_handle, DS18B20_RESOLUTION_10BIT);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: scratchpad set resolution failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: scratchpad set resolution 10bit.\n");
    res = ds18b20_scratchpad_get_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: scratchpad get resolution failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: check resolution %s.\n", resolution==DS18B20_RESOLUTION_10BIT?"ok":"error");
    
    /* RESOLUTION_11BIT */
    res = ds18b20_scratchpad_set_resolution(&gs_handle, DS18B20_RESOLUTION_11BIT);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: scratchpad set resolution failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: scratchpad set resolution 11bit.\n");
    res = ds18b20_scratchpad_get_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: scratchpad get resolution failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: check resolution %s.\n", resolution==DS18B20_RESOLUTION_11BIT?"ok":"error");
    
    /* RESOLUTION_12BIT */
    res = ds18b20_scratchpad_set_resolution(&gs_handle, DS18B20_RESOLUTION_12BIT);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: scratchpad set resolution failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: scratchpad set resolution 12bit.\n");
    res = ds18b20_scratchpad_get_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: scratchpad get resolution failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: check resolution %s.\n", resolution==DS18B20_RESOLUTION_12BIT?"ok":"error");
    
    /* ds18b20_scrachpad_set_alarm_threshold/ds18b20_scrachpad_get_alarm_threshold test */
    ds18b20_interface_debug_print("ds18b20: ds18b20_scrachpad_set_alarm_threshold/ds18b20_scrachpad_get_alarm_threshold test.\n");
    threshold_high = (int8_t)(rand()%128);
    threshold_low = (int8_t)(-(rand()%128));
    res = ds18b20_scratchpad_set_alarm_threshold(&gs_handle, threshold_high, threshold_low);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: scratchpad set alarm threshold failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: scratchpad set alarm high threshold %d.\n", threshold_high);
    ds18b20_interface_debug_print("ds18b20: scratchpad set alarm low threshold %d.\n", threshold_low);
    res = ds18b20_scrachpad_get_alarm_threshold(&gs_handle, (int8_t *)&threshold_high_check, (int8_t *)&threshold_low_check);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: scratchpad get alarm threshold failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: check alarm high threshold %s.\n", threshold_high==threshold_high_check?"ok":"error");
    ds18b20_interface_debug_print("ds18b20: check alarm low threshold %s.\n", threshold_low==threshold_low_check?"ok":"error");
    
    /* ds18b20_copy_scratchpad_to_eeprom/ds18b20_copy_eeprom_to_scratchpad test */
    ds18b20_interface_debug_print("ds18b20: ds18b20_copy_scratchpad_to_eeprom/ds18b20_copy_eeprom_to_scratchpad test.\n");
    res = ds18b20_scratchpad_set_resolution(&gs_handle, DS18B20_RESOLUTION_9BIT);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: scratchpad set resolution failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    res = ds18b20_copy_scratchpad_to_eeprom(&gs_handle);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: copy scratchpad to eeprom failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: copy scratchpad to eeprom.\n");
    res = ds18b20_copy_eeprom_to_scratchpad(&gs_handle);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: ds18b20 copy eeprom to scratchpad failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    res = ds18b20_scratchpad_get_resolution(&gs_handle, &resolution);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: scratchpad get resolution failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: check scratchpad eeprom %s.\n", resolution==DS18B20_RESOLUTION_9BIT?"ok":"error");
    
    /* ds18b20_alarm_convert_to_register/ds18b20_alarm_convert_to_data test */
    ds18b20_interface_debug_print("ds18b20: ds18b20_alarm_convert_to_register/ds18b20_alarm_convert_to_data test.\n");
    temp = (float)(rand()%36000)/1000.0f;
    res = ds18b20_alarm_convert_to_register(&gs_handle, temp, (int8_t *)&reg);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: alarm convert to register failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: set alarm convert to register %0.2f.\n", temp);
    res = ds18b20_alarm_convert_to_data(&gs_handle, reg, (float *)&temp_check);
    if (res != 0)
    {
        ds18b20_interface_debug_print("ds18b20: alarm convert to data failed.\n");
        (void)ds18b20_deinit(&gs_handle);
        
        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: get convert to data %0.2f.\n", temp_check);
    
    /* finish register test */
    ds18b20_interface_debug_print("ds18b20: finish register test.\n");
    (void)ds18b20_deinit(&gs_handle);
    
    return 0;
}
