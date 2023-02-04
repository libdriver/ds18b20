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
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];        /**< uart buffer */
volatile uint16_t g_len;   /**< uart buffer length */

/**
 * @brief     ds18b20 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t ds18b20(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"high-threshold", required_argument, NULL, 1},
        {"low-threshold", required_argument, NULL, 2},
        {"rom", required_argument, NULL, 3},
        {"times", required_argument, NULL, 4},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint8_t flag_rom = 0;
    uint8_t flag_low = 0;
    uint8_t flag_high = 0;
    uint32_t times = 3;
    uint8_t rom[8] = {0};
    float low = 0.0f;
    float high = 0.0f;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* high threshold */
            case 1 :
            {
                /* set the high threshold */
                high = atof(optarg);
                
                /* set the high flag */
                flag_high = 1;
                
                break;
            }
            
            /* low threshold */
            case 2 :
            {
                /* set the low threshold */
                low = atof(optarg);
                
                /* set the low flag */
                flag_low = 1;
                
                break;
            }
            
            /* rom */
            case 3 :
            {
                uint8_t i;
                
                /* check the flag */
                if (strlen(optarg) != 16)
                {
                    return 5;
                }
                
                /* set the rom */
                for (i = 0; i < 8; i++)
                {
                    uint8_t temp;
                    
                    if ((optarg[i * 2 + 0] <= '9') && (optarg[i * 2 + 0] >= '0'))
                    {
                        temp = (optarg[i * 2 + 0] - '0') * 16;
                    }
                    else
                    {
                        temp = (optarg[i * 2 + 0] - 'A' + 10) * 16;
                    }
                    if ((optarg[i * 2 + 1] <= '9') && (optarg[i * 2 + 1] >= '0'))
                    {
                        temp += optarg[i * 2 + 1] - '0';
                    }
                    else
                    {
                        temp += optarg[i * 2 + 1] - 'A' + 10;
                    }
                    rom[i] = temp;
                }
                
                /* set the rom flag */
                flag_rom = 1;
                
                break;
            }
            
            /* running times */
            case 4 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (ds18b20_register_test() != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (ds18b20_read_test(times) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_search", type) == 0)
    {
        /* run search test */
        if (ds18b20_search_test() != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float temperature;
        
        /* init */
        res = ds18b20_basic_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 2000ms */
            ds18b20_interface_delay_ms(2000);
            
            /* read data */
            res = ds18b20_basic_read((float *)&temperature);
            if (res != 0)
            {
                (void)ds18b20_basic_deinit();
                
                return 1;
            }
            
            /* output */
            ds18b20_interface_debug_print("ds18b20: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            ds18b20_interface_debug_print("ds18b20: temperature is %0.2fC.\n", temperature);
        }
        
        /* deinit */
        (void)ds18b20_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_match", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float temperature;
        
        /* check the rom */
        if (flag_rom != 1)
        {
            ds18b20_interface_debug_print("ds18b20: no rom.\n");
            
            return 1;
        }
        
        /* init */
        res = ds18b20_match_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 2000ms */
            ds18b20_interface_delay_ms(2000);
            
            /* read data */
            res = ds18b20_match_read((uint8_t *)rom, (float *)&temperature);
            if (res != 0)
            {
                (void)ds18b20_match_deinit();
                
                return 1;
            }
            
            /* output */
            ds18b20_interface_debug_print("ds18b20: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            ds18b20_interface_debug_print("ds18b20: temperature is %0.2fC.\n", temperature);
        }
        
        /* deinit */
        (void)ds18b20_match_deinit();
        
        return 0;
    }
    else if (strcmp("e_search", type) == 0)
    {
        uint8_t res, i, j;
        uint8_t rom[8][8];
        uint8_t num;
        
        /* init */
        res = ds18b20_search_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* search */
        num = 8;
        res = ds18b20_search((uint8_t (*)[8])rom, (uint8_t *)&num);
        if (res != 0)
        {
            (void)ds18b20_search_deinit();
            
            return 1;
        }
        
        /* output */
        ds18b20_interface_debug_print("ds18b20: find %d rom(s).\n", num);
        for (i = 0; i < num; i++)
        {
            ds18b20_interface_debug_print("ds18b20: %d/%d is ", (uint32_t)(i + 1), (uint32_t)num);
            for (j = 0; j < 8; j++)
            {
                ds18b20_interface_debug_print("%02X", rom[i][j]);
            }
            ds18b20_interface_debug_print(".\n");
        }
        
        /* deinit */
        (void)ds18b20_search_deinit();
        
        return 0;
    }
    else if (strcmp("e_alarm-set", type) == 0)
    {
        uint8_t res;
        
        /* check the rom */
        if (flag_rom != 1)
        {
            ds18b20_interface_debug_print("ds18b20: no rom.\n");
            
            return 1;
        }
        
        /* check the low threshold */
        if (flag_low != 1)
        {
            ds18b20_interface_debug_print("ds18b20: no low threshold.\n");
            
            return 1;
        }
        
        /* check the high threshold */
        if (flag_high != 1)
        {
            ds18b20_interface_debug_print("ds18b20: no high threshold.\n");
            
            return 1;
        }
        
        /* init */
        res = ds18b20_alarm_init();
        if (res != 0)
        {
            (void)ds18b20_alarm_deinit();
            
            return 1;
        }
        
        /* set the threshold */
        res = ds18b20_alarm_set_threshold((uint8_t *)rom, low, high);
        if (res != 0)
        {
            (void)ds18b20_alarm_deinit();
            
            return 1;
        }
        
        /* output */
        ds18b20_interface_debug_print("ds18b20: set alarm low threshold %0.2f.\n", low); 
        ds18b20_interface_debug_print("ds18b20: set alarm high threshold %0.2f.\n", high); 
        
        /* deinit */
        (void)ds18b20_alarm_deinit();
        
        return 0;
    }
    else if (strcmp("e_alarm-get", type) == 0)
    {
        uint8_t res;
        
        /* init */
        res = ds18b20_alarm_init();
        if (res != 0)
        {
            (void)ds18b20_alarm_deinit();
            
            return 1;
        }
        
        /* get the threshold */
        res = ds18b20_alarm_get_threshold((uint8_t *)rom, (float *)&low, (float *)&high);
        if (res != 0)
        {
            (void)ds18b20_alarm_deinit();
            
            return 1;
        }
        
        /* output */
        ds18b20_interface_debug_print("ds18b20: alarm low threshold is %0.2f.\n", low); 
        ds18b20_interface_debug_print("ds18b20: alarm high threshold is %0.2f.\n", high); 
        
        /* deinit */
        (void)ds18b20_alarm_deinit();
        
        return 0;
    }
    else if (strcmp("e_alarm-search", type) == 0)
    {
        uint8_t res;
        uint8_t rom[8][8];
        uint8_t num;
        uint8_t i, j;
        
        /* init */
        res = ds18b20_alarm_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* search */
        num = 8;
        res = ds18b20_alarm_search((uint8_t (*)[8])rom, (uint8_t *)&num);
        if (res != 0)
        {
            (void)ds18b20_alarm_deinit();
            
            return 1;
        }
        
        /* output */
        ds18b20_interface_debug_print("ds18b20: find %d alarm rom(s).\n", num);
        for (i = 0; i < num; i++)
        {
            ds18b20_interface_debug_print("ds18b20: %d/%d is ", (uint32_t)(i + 1), (uint32_t)num);
            for (j = 0; j < 8; j++)
            {
                ds18b20_interface_debug_print("%02X", rom[i][j]);
            }
            ds18b20_interface_debug_print(".\n");
        }
        
        /* deinit */
        (void)ds18b20_alarm_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        ds18b20_interface_debug_print("Usage:\n");
        ds18b20_interface_debug_print("  ds18b20 (-i | --information)\n");
        ds18b20_interface_debug_print("  ds18b20 (-h | --help)\n");
        ds18b20_interface_debug_print("  ds18b20 (-p | --port)\n");
        ds18b20_interface_debug_print("  ds18b20 (-t reg | --test=reg)\n");
        ds18b20_interface_debug_print("  ds18b20 (-t read | --test=read) [--times=<num>]\n");
        ds18b20_interface_debug_print("  ds18b20 (-t search | --test=search)\n");
        ds18b20_interface_debug_print("  ds18b20 (-e read | --example=read) [--times=<num>]\n");
        ds18b20_interface_debug_print("  ds18b20 (-e match | --example=match) --rom=<code> [--times=<num>]\n");
        ds18b20_interface_debug_print("  ds18b20 (-e search | --example=search)\n");
        ds18b20_interface_debug_print("  ds18b20 (-e alarm-set | --example=alarm-set) --low-threshold=<low> --high-threshold=<high> --rom=<code>\n");
        ds18b20_interface_debug_print("  ds18b20 (-e alarm-get | --example=alarm-get) --rom=<code>\n");
        ds18b20_interface_debug_print("  ds18b20 (-e alarm-search | --example=alarm-search)\n");
        ds18b20_interface_debug_print("\n");
        ds18b20_interface_debug_print("Options:\n");
        ds18b20_interface_debug_print("  -e <read | match | search | alarm-set | alarm-get | alarm-search>, --example=<read | match | search | alarm-set | alarm-get | alarm-search>\n");
        ds18b20_interface_debug_print("                                 Run the driver example.\n");
        ds18b20_interface_debug_print("  -h, --help                     Show the help.\n");
        ds18b20_interface_debug_print("      --high-threshold=<high>    Set the temperature interrupt high threshold.\n");
        ds18b20_interface_debug_print("  -i, --information              Show the chip information.\n");
        ds18b20_interface_debug_print("      --low-threshold=<low>      Set the temperature interrupt low threshold.\n");
        ds18b20_interface_debug_print("  -p, --port                     Display the pin connections of the current board.\n");
        ds18b20_interface_debug_print("      --rom=<code>               Set the rom with the length of 8 and it is hexadecimal.\n");
        ds18b20_interface_debug_print("  -t <reg | read | search>, --test=<reg | read | search>\n");
        ds18b20_interface_debug_print("                                 Run the driver test.\n");
        ds18b20_interface_debug_print("      --times=<num>              Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        ds18b20_info_t info;
        
        /* print ds18b20 info */
        ds18b20_info(&info);
        ds18b20_interface_debug_print("ds18b20: chip is %s.\n", info.chip_name);
        ds18b20_interface_debug_print("ds18b20: manufacturer is %s.\n", info.manufacturer_name);
        ds18b20_interface_debug_print("ds18b20: interface is %s.\n", info.interface);
        ds18b20_interface_debug_print("ds18b20: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ds18b20_interface_debug_print("ds18b20: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ds18b20_interface_debug_print("ds18b20: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ds18b20_interface_debug_print("ds18b20: max current is %0.2fmA.\n", info.max_current_ma);
        ds18b20_interface_debug_print("ds18b20: max temperature is %0.1fC.\n", info.temperature_max);
        ds18b20_interface_debug_print("ds18b20: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        ds18b20_interface_debug_print("ds18b20: DQ pin connected to GPIOA PIN8.\n");
        
        return 0;
    }
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
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register ds18b20 function */
    shell_init();
    shell_register("ds18b20", ds18b20);
    uart_print("ds18b20: welcome to libdriver ds18b20.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("ds18b20: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("ds18b20: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("ds18b20: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("ds18b20: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("ds18b20: param is invalid.\n");
            }
            else
            {
                uart_print("ds18b20: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
