/**
 * Copyright (C) LibDriver 2015-2021 All rights reserved
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
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-04-06
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/04/06  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ds18b20_basic.h"
#include "driver_ds18b20_match.h"
#include "driver_ds18b20_search.h"
#include "driver_ds18b20_alarm.h"
#include "driver_ds18b20_register_test.h"
#include "driver_ds18b20_read_test.h"
#include "driver_ds18b20_search_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];        /**< uart buffer */
uint16_t g_len;            /**< uart buffer length */

/**
 * @brief     ds18b20 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t ds18b20(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            ds18b20_info_t info;
            
            /* print ds18b20 info */
            ds18b20_info(&info);
            ds18b20_interface_debug_print("ds18b20: chip is %s.\n", info.chip_name);
            ds18b20_interface_debug_print("ds18b20: manufacturer is %s.\n", info.manufacturer_name);
            ds18b20_interface_debug_print("ds18b20: interface is %s.\n", info.interface);
            ds18b20_interface_debug_print("ds18b20: driver version is %d.%d.\n", info.driver_version/1000, (info.driver_version%1000)/100);
            ds18b20_interface_debug_print("ds18b20: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            ds18b20_interface_debug_print("ds18b20: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            ds18b20_interface_debug_print("ds18b20: max current is %0.2fmA.\n", info.max_current_ma);
            ds18b20_interface_debug_print("ds18b20: max temperature is %0.1fC.\n", info.temperature_max);
            ds18b20_interface_debug_print("ds18b20: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            ds18b20_interface_debug_print("ds18b20: DQ pin connected to GPIOA PIN8.\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show ds18b20 help */
            
            help:
            
            ds18b20_interface_debug_print("ds18b20 -i\n\tshow ds18b20 chip and driver information.\n");
            ds18b20_interface_debug_print("ds18b20 -h\n\tshow ds18b20 help.\n");
            ds18b20_interface_debug_print("ds18b20 -p\n\tshow ds18b20 pin connections of the current board.\n");
            ds18b20_interface_debug_print("ds18b20 -t reg\n\trun ds18b20 register test.\n");
            ds18b20_interface_debug_print("ds18b20 -t read <times>\n\trun ds18b20 read test.times means the test times.\n");
            ds18b20_interface_debug_print("ds18b20 -t search\n\trun ds18b20 search test.\n");
            ds18b20_interface_debug_print("ds18b20 -c read <times>\n\trun ds18b20 read function.times means the read times.\n");
            ds18b20_interface_debug_print("ds18b20 -c match <times> -rom <r1> <r2> <r3> <r4> <r5> <r6> <r7> <r8>\n\t"
                                          "run ds18b20 match function.times means the read times.<r1>-<r8> mean the rom id and they are hexadecimal.\n");
            ds18b20_interface_debug_print("ds18b20 -c search\n\trun ds18b20 search rom function.\n");
            ds18b20_interface_debug_print("ds18b20 -c alarm set <lowthreshold> <highthreshold> -rom <r1> <r2> <r3> <r4> <r5> <r6> <r7> <r8>\n\t"
                                          "run alarm set threshold function.lowthreshold means the low threshold.highthreshold means the high threshold."); 
            ds18b20_interface_debug_print("<r1>-<r8> mean the rom id and they are hexadecimal.\n");
            ds18b20_interface_debug_print("ds18b20 -c alarm get -rom <r1> <r2> <r3> <r4> <r5> <r6> <r7> <r8>\n\trun alarm get threshold function."
                                          "<r1>-<r8> mean the rom id and they are hexadecimal.\n");
            ds18b20_interface_debug_print("ds18b20 -c alarm search\n\trun ds18b20 search alarm rom function.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 3)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
             /* reg test */
            if (strcmp("reg", argv[2]) == 0)
            {
                /* run reg test */
                if (ds18b20_register_test())
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            /* search test */
            else if (strcmp("search", argv[2]) == 0)
            {
                /* run search test */
                if (ds18b20_search_test())
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* run function */
        else if (strcmp("-c", argv[1]) == 0)
        {
             /* search function */
            if (strcmp("search", argv[2]) == 0)
            {
                volatile uint8_t res, i, j;
                volatile uint8_t rom[8][8];
                volatile uint8_t num;
                
                res = ds18b20_search_init();
                if (res)
                {
                    return 1;
                }
                num = 8;
                res = ds18b20_search((uint8_t (*)[8])rom, (uint8_t *)&num);
                if (res)
                {
                    ds18b20_search_deinit();
                    
                    return 1;
                }
                ds18b20_interface_debug_print("ds18b20: find %d rom(s).\n", num);
                for (i=0; i<num; i++)
                {
                    uart1_print("ds18b20: %d/%d is ", (uint32_t)(i+1), (uint32_t)num);
                    for (j=0; j<8; j++)
                    {
                        ds18b20_interface_debug_print("%02X ", rom[i][j]);
                    }
                    ds18b20_interface_debug_print(".\n");
                }
                ds18b20_search_deinit();
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
             /* read test */
            if (strcmp("read", argv[2]) == 0)
            {
                /* run read test */
                if (ds18b20_read_test(atoi(argv[3])))
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* run function */
        else if (strcmp("-c", argv[1]) == 0)
        {
             /* read function */
            if (strcmp("read", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t times;
                volatile uint32_t i;
                volatile float temperature;
                
                res = ds18b20_basic_init();
                if (res)
                {
                    return 1;
                }
                times = atoi(argv[3]);
                for (i=0; i<times; i++)
                {
                    delay_ms(2000);
                    res = ds18b20_basic_read((float *)&temperature);
                    if (res)
                    {
                        ds18b20_basic_deinit();
                        
                        return 1;
                    }
                    ds18b20_interface_debug_print("ds18b20: %d/%d.\n", (uint32_t)(i+1), (uint32_t)times);
                    ds18b20_interface_debug_print("ds18b20: temperature is %0.2fC.\n", temperature);
                }
                ds18b20_basic_deinit();
                
                return 0;
            }
            /* alarm function */
            else if (strcmp("alarm", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint8_t rom[8][8];
                volatile uint8_t num;
                volatile uint8_t i, j;
                
                if (strcmp("search", argv[3]) != 0)
                {
                    return 5;
                }
                
                res = ds18b20_alarm_init();
                if (res)
                {
                    return 1;
                }
                num = 8;
                res = ds18b20_alarm_search((uint8_t (*)[8])rom, (uint8_t *)&num);
                if (res)
                {
                    ds18b20_alarm_deinit();
                    
                    return 1;
                }
                ds18b20_interface_debug_print("ds18b20: find %d alarm rom(s).\n", num);
                for (i=0; i<num; i++)
                {
                    uart1_print("ds18b20: %d/%d is ", (uint32_t)(i+1), (uint32_t)num);
                    for (j=0; j<8; j++)
                    {
                        ds18b20_interface_debug_print("%02X ", rom[i][j]);
                    }
                    ds18b20_interface_debug_print(".\n");
                }
                ds18b20_alarm_deinit();
                
                return 0;
                
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 13)
    {
        /* run function */
        if (strcmp("-c", argv[1]) == 0)
        {
             /* match function */
            if (strcmp("match", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t times;
                volatile uint32_t i;
                volatile float temperature;
                volatile uint8_t rom[8];
                
                times = atoi(argv[3]);
                if (strcmp("-rom", argv[4]) != 0)
                {
                    return 5;
                }
                for (i=0; i<8; i++)
                {
                    volatile uint8_t temp;
                    
                    if (strlen(argv[5+i]) < 2)
                    {
                        return 5;
                    }
                    if ((argv[5+i][0] <= '9') && (argv[5+i][0] >= '0'))
                    {
                        temp = (argv[5+i][0] - '0') * 16;
                    }
                    else
                    {
                        temp = (argv[5+i][0] - 'A' + 10) * 16;
                    }
                    if ((argv[5+i][1] <= '9') && (argv[5+i][1] >= '0'))
                    {
                        temp += argv[5+i][1] - '0';
                    }
                    else
                    {
                        temp += argv[5+i][1] - 'A' + 10;
                    }
                    rom[i] = temp;
                }
                res = ds18b20_match_init();
                if (res)
                {
                    return 1;
                }
                for (i=0; i<times; i++)
                {
                    delay_ms(2000);
                    res = ds18b20_match_read((uint8_t *)rom, (float *)&temperature);
                    if (res)
                    {
                        ds18b20_match_deinit();
                        
                        return 1;
                    }
                    ds18b20_interface_debug_print("ds18b20: %d/%d.\n", (uint32_t)(i+1), (uint32_t)times);
                    ds18b20_interface_debug_print("ds18b20: temperature is %0.2fC.\n", temperature);
                }
                ds18b20_match_deinit();
                
                return 0;
            }
            /* alarm get function */
            else if (strcmp("alarm", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint8_t i;
                volatile uint8_t rom[8];
                volatile float low;
                volatile float high;
                
                if (strcmp("get", argv[3]) != 0)
                {
                    return 5;
                }
                if (strcmp("-rom", argv[4]) != 0)
                {
                    return 5;
                }
                for (i=0; i<8; i++)
                {
                    volatile uint8_t temp;
                    
                    if (strlen(argv[5+i]) < 2)
                    {
                        return 5;
                    }
                    if ((argv[5+i][0] <= '9') && (argv[5+i][0] >= '0'))
                    {
                        temp = (argv[5+i][0] - '0') * 16;
                    }
                    else
                    {
                        temp = (argv[5+i][0] - 'A' + 10) * 16;
                    }
                    if ((argv[5+i][1] <= '9') && (argv[5+i][1] >= '0'))
                    {
                        temp += argv[5+i][1] - '0';
                    }
                    else
                    {
                        temp += argv[5+i][1] - 'A' + 10;
                    }
                    rom[i] = temp;
                }
                res = ds18b20_alarm_init();
                if (res)
                {
                    ds18b20_alarm_deinit();
                    
                    return 1;
                }
                res = ds18b20_alarm_get_threshold((uint8_t *)rom, (float *)&low, (float *)&high);
                if (res)
                {
                    ds18b20_alarm_deinit();
                    
                    return 1;
                }
                ds18b20_interface_debug_print("ds18b20: alarm low threshold is %0.2f.\n", low); 
                ds18b20_interface_debug_print("ds18b20: alarm high threshold is %0.2f.\n", high); 
                ds18b20_alarm_deinit();
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 15)
    {
        /* run function */
        if (strcmp("-c", argv[1]) == 0)
        {
             /* alarm set function */
            if (strcmp("alarm", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint8_t i;
                volatile uint8_t rom[8];
                volatile float low;
                volatile float high;
                
                if (strcmp("set", argv[3]) != 0)
                {
                    return 5;
                }
                low = (float)(atof(argv[4]));
                high = (float)(atof(argv[5]));
                if (strcmp("-rom", argv[6]) != 0)
                {
                    return 5;
                }
                for (i=0; i<8; i++)
                {
                    volatile uint8_t temp;
                    
                    if (strlen(argv[7+i]) < 2)
                    {
                        return 5;
                    }
                    if ((argv[7+i][0] <= '9') && (argv[7+i][0] >= '0'))
                    {
                        temp = (argv[7+i][0] - '0') * 16;
                    }
                    else
                    {
                        temp = (argv[7+i][0] - 'A' + 10) * 16;
                    }
                    if ((argv[7+i][1] <= '9') && (argv[7+i][1] >= '0'))
                    {
                        temp += argv[7+i][1] - '0';
                    }
                    else
                    {
                        temp += argv[7+i][1] - 'A' + 10;
                    }
                    rom[i] = temp;
                }
                res = ds18b20_alarm_init();
                if (res)
                {
                    ds18b20_alarm_deinit();
                    
                    return 1;
                }
                res = ds18b20_alarm_set_threshold((uint8_t *)rom, low, high);
                if (res)
                {
                    ds18b20_alarm_deinit();
                    
                    return 1;
                }
                ds18b20_interface_debug_print("ds18b20: set alarm low threshold %0.2f.\n", low); 
                ds18b20_interface_debug_print("ds18b20: set alarm high threshold %0.2f.\n", high); 
                ds18b20_alarm_deinit();
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    volatile uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart1 init */
    uart1_init(115200);
    
    /* shell init && register ds18b20 fuction */
    shell_init();
    shell_register("ds18b20", ds18b20);
    uart1_print("ds18b20: welcome to libdriver ds18b20.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart1_read(g_buf, 256);
        if (g_len)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart1_print("ds18b20: run failed.\n");
            }
            else if (res == 2)
            {
                uart1_print("ds18b20: unknow command.\n");
            }
            else if (res == 3)
            {
                uart1_print("ds18b20: length is too long.\n");
            }
            else if (res == 4)
            {
                uart1_print("ds18b20: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart1_print("ds18b20: param is invalid.\n");
            }
            else
            {
                uart1_print("ds18b20: unknow status code.\n");
            }
            uart1_flush();
        }
        delay_ms(100);
    }
}
