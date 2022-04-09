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
 * @file      driver_ds18b20.c
 * @brief     driver ds18b20 source file
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

#include "driver_ds18b20.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Maxim Integrated DS18B20"        /**< chip name */
#define MANUFACTURER_NAME         "Maxim Integrated"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        3.0f                              /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                              /**< chip max supply voltage */
#define MAX_CURRENT               4.0f                              /**< chip max current */
#define TEMPERATURE_MIN           -55.0f                            /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f                            /**< chip max operating temperature */
#define DRIVER_VERSION            2000                              /**< driver version */

/**
 * @brief chip command definition
 */
#define DS18B20_CMD_SEARCH_ROM               0xF0        /**< search rom command */
#define DS18B20_CMD_READ_ROM                 0x33        /**< read rom command */
#define DS18B20_CMD_MATCH_ROM                0x55        /**< match rom command */
#define DS18B20_CMD_SKIP_ROM                 0xCC        /**< skip rom command */
#define DS18B20_CMD_ALARM_SEARCH             0xEC        /**< alarm search command */
#define DS18B20_CMD_CONVERT_T                0x44        /**< convert command */
#define DS18B20_CMD_WRITE_SCRATCHPAD         0x4E        /**< write scrachpad command */
#define DS18B20_CMD_READ_SCRATCHPAD          0xBE        /**< read scrachpad command */
#define DS18B20_CMD_COPY_SCRATCHPAD          0x48        /**< copy scrachpad command */
#define DS18B20_CMD_RECALL_EE                0xB8        /**< recall ee command */
#define DS18B20_CMD_READ_POWER_SUPPLY        0xB4        /**< read power supply command */

/**
 * @brief crc table
 */
const uint8_t gc_ds18b20_crc_table[256] =
{
    0X00, 0X5E, 0XBC, 0XE2, 0X61, 0X3F, 0XDD, 0X83, 0XC2, 0X9C, 0X7E, 0X20, 0XA3,
    0XFD, 0X1F, 0X41, 0X9D, 0XC3, 0X21, 0X7F, 0XFC, 0XA2, 0X40, 0X1E, 0X5F, 0X01,
    0XE3, 0XBD, 0X3E, 0X60, 0X82, 0XDC, 0X23, 0X7D, 0X9F, 0XC1, 0X42, 0X1C, 0XFE,
    0XA0, 0XE1, 0XBF, 0X5D, 0X03, 0X80, 0XDE, 0X3C, 0X62, 0XBE, 0XE0, 0X02, 0X5C,
    0XDF, 0X81, 0X63, 0X3D, 0X7C, 0X22, 0XC0, 0X9E, 0X1D, 0X43, 0XA1, 0XFF, 0X46,
    0X18, 0XFA, 0XA4, 0X27, 0X79, 0X9B, 0XC5, 0X84, 0XDA, 0X38, 0X66, 0XE5, 0XBB,
    0X59, 0X07, 0XDB, 0X85, 0X67, 0X39, 0XBA, 0XE4, 0X06, 0X58, 0X19, 0X47, 0XA5,
    0XFB, 0X78, 0X26, 0XC4, 0X9A, 0X65, 0X3B, 0XD9, 0X87, 0X04, 0X5A, 0XB8, 0XE6,
    0XA7, 0XF9, 0X1B, 0X45, 0XC6, 0X98, 0X7A, 0X24, 0XF8, 0XA6, 0X44, 0X1A, 0X99,
    0XC7, 0X25, 0X7B, 0X3A, 0X64, 0X86, 0XD8, 0X5B, 0X05, 0XE7, 0XB9, 0X8C, 0XD2,
    0X30, 0X6E, 0XED, 0XB3, 0X51, 0X0F, 0X4E, 0X10, 0XF2, 0XAC, 0X2F, 0X71, 0X93,
    0XCD, 0X11, 0X4F, 0XAD, 0XF3, 0X70, 0X2E, 0XCC, 0X92, 0XD3, 0X8D, 0X6F, 0X31,
    0XB2, 0XEC, 0X0E, 0X50, 0XAF, 0XF1, 0X13, 0X4D, 0XCE, 0X90, 0X72, 0X2C, 0X6D,
    0X33, 0XD1, 0X8F, 0X0C, 0X52, 0XB0, 0XEE, 0X32, 0X6C, 0X8E, 0XD0, 0X53, 0X0D,
    0XEF, 0XB1, 0XF0, 0XAE, 0X4C, 0X12, 0X91, 0XCF, 0X2D, 0X73, 0XCA, 0X94, 0X76,
    0X28, 0XAB, 0XF5, 0X17, 0X49, 0X08, 0X56, 0XB4, 0XEA, 0X69, 0X37, 0XD5, 0X8B,
    0X57, 0X09, 0XEB, 0XB5, 0X36, 0X68, 0X8A, 0XD4, 0X95, 0XCB, 0X29, 0X77, 0XF4,
    0XAA, 0X48, 0X16, 0XE9, 0XB7, 0X55, 0X0B, 0X88, 0XD6, 0X34, 0X6A, 0X2B, 0X75,
    0X97, 0XC9, 0X4A, 0X14, 0XF6, 0XA8, 0X74, 0X2A, 0XC8, 0X96, 0X15, 0X4B, 0XA9,
    0XF7, 0XB6, 0XE8, 0X0A, 0X54, 0XD7, 0X89, 0X6B, 0X35,
};

/**
 * @brief     check the crc
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @param[in] crc is the checked crc
 * @return    status code
 *            - 0 success
 *            - 1 check failed
 * @note      none
 */
static uint8_t a_ds18b20_check_crc(uint8_t *buf, uint8_t len, uint8_t crc)
{
    uint8_t i;
    uint8_t crc8 = 0;
    
    for (i = 0; i < len; i++)
    {
        crc8 = gc_ds18b20_crc_table[crc8 ^ buf[i]];        /* calculate crc */
    }
    if (crc8 == crc)                                       /* check crc */
    {
        return 0;                                          /* return right */
    }
    else
    {
        return 1;                                          /* return wrong */
    }
}

/**
 * @brief     reset the chip
 * @param[in] *handle points to a ds18b20 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 reset failed
 * @note      none
 */
static uint8_t a_ds18b20_reset(ds18b20_handle_t *handle)
{
    uint8_t retry = 0;
    uint8_t res;
    
    handle->disable_irq();                                              /* disable irq */
    if (handle->bus_write(0) != 0)                                      /* write 0 */
    {
        handle->enable_irq();                                           /* enable irq */
        handle->debug_print("ds18b20: bus write failed.\n");            /* write failed */
        
        return 1;                                                       /* return error */
    }
    handle->delay_us(750);                                              /* wait 750 us */
    if (handle->bus_write(1) != 0)                                      /* write 1 */
    {
        handle->enable_irq();                                           /* enable irq */
        handle->debug_print("ds18b20: bus write failed.\n");            /* write failed */
        
        return 1;                                                       /* return error */
    }
    handle->delay_us(15);                                               /* wait 15 us */
    res = 1;                                                            /* reset res */
    while ((res != 0) && (retry < 200))                                 /* wait 200 us */
    {
        if (handle->bus_read((uint8_t *)&res) != 0)                     /* read 1 bit */
        {
            handle->enable_irq();                                       /* enable irq */
            handle->debug_print("ds18b20: bus read failed.\n");         /* read failed */
            
            return 1;                                                   /* return error */
        }
        retry++;                                                        /* retry times++ */
        handle->delay_us(1);                                            /* delay 1 us */
    }
    if (retry >= 200)                                                   /* if retry times is over 200 times */
    {
        handle->enable_irq();                                           /* enable irq */
        handle->debug_print("ds18b20: bus read no response.\n");        /* no response */
        
        return 1;                                                       /* return error */
    }
    else
    {
        retry = 0;                                                      /* reset retry */
    }
    res = 0;                                                            /* reset res */
    while ((res == 0)&& (retry < 240))                                  /* wait 240 us */
    {
        if (handle->bus_read((uint8_t *)&res) != 0)                     /* read one bit */
        {
            handle->enable_irq();                                       /* enable irq */
            handle->debug_print("ds18b20: bus read failed.\n");         /* read failed */
            
            return 1;                                                   /* return error */
        }
        retry++;                                                        /* retry times++ */
        handle->delay_us(1);                                            /* delay 1 us */
    }
    if (retry >= 240)                                                   /* if retry times is over 240 times */
    {
        handle->enable_irq();                                           /* enable irq */
        handle->debug_print("ds18b20: bus read no response.\n");        /* no response */
        
        return 1;                                                       /* return error */
    }
    handle->enable_irq();                                               /* enable irq */
    
    return 0;                                                           /* success return 0 */
}

/**
 * @brief      read one bit from the chip
 * @param[in]  *handle points to a ds18b20 handle structure
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read bit failed
 * @note       none
 */
static uint8_t a_ds18b20_read_bit(ds18b20_handle_t *handle, uint8_t *data)
{
    if (handle->bus_write(0) != 0)                                  /* write 0 */
    {
        handle->debug_print("ds18b20: bus write failed.\n");        /* write failed */
        
        return 1;                                                   /* return error */
    }
    handle->delay_us(2);                                            /* wait 2 us */
    if (handle->bus_write(1) != 0)                                  /* write 1 */
    {
        handle->debug_print("ds18b20: bus write failed.\n");        /* write failed */
        
        return 1;                                                   /* return error */
    }
    handle->delay_us(12);                                           /* wait 12 us */
    if (handle->bus_read(data) != 0)                                /* read 1 bit */
    {
        handle->debug_print("ds18b20: bus read failed.\n");         /* read failed */
        
        return 1;                                                   /* return error */
    }
    handle->delay_us(50);                                           /* wait 50 us */
    
    return 0;                                                       /* success return 0 */
}

/**
 * @brief      read one byte from the chip
 * @param[in]  *handle points to a ds18b20 handle structure
 * @param[out] *byte points to a byte buffer
 * @return     status code
 *             - 0 success
 *             - 1 read byte failed
 * @note       none
 */
static uint8_t a_ds18b20_read_byte(ds18b20_handle_t *handle, uint8_t *byte)
{
    uint8_t i, j;
    
    *byte = 0;                                                              /* set byte 0 */
    handle->disable_irq();                                                  /* disable irq */
    for (i = 1; i <= 8; i++)
    {
        if (a_ds18b20_read_bit(handle, (uint8_t *)&j) != 0)                 /* read 1 bit */
        {
            handle->enable_irq();                                           /* enable irq */
            handle->debug_print("ds18b20: bus read byte failed.\n");        /* read byte failed */
            
            return 1;                                                       /* return error */
        }
        *byte = (j << 7) | ((*byte) >> 1);                                  /* set MSB */
    }
    handle->enable_irq();                                                   /* enable irq */
    
    return 0;                                                               /* success return 0 */
}

/**
 * @brief     write one byte to the chip
 * @param[in] *handle points to a ds18b20 handle structure
 * @param[in] byte is the written byte
 * @return    status code
 *            - 0 success
 *            - 1 write byte failed
 * @note      none
 */
static uint8_t a_ds18b20_write_byte(ds18b20_handle_t *handle, uint8_t byte)
{
    uint8_t j;
    uint8_t test_b;
    
    handle->disable_irq();                                                  /* disable irq */
    for (j = 1; j <= 8; j++)                                                /* run 8 times, 8 bist = 1 Byte */
    {
        test_b = byte & 0x01;                                               /* get 1 bit */
        byte = byte >> 1;                                                   /* right shift 1 bit */
        if (test_b != 0)                                                    /* write 1 */
        {
            if (handle->bus_write(0) != 0)                                  /* write 0 */
            {
                handle->enable_irq();                                       /* enable irq */
                handle->debug_print("ds18b20: bus write failed.\n");        /* write failed */
                
                return 1;                                                   /* return error */
            }
            handle->delay_us(2);                                            /* wait 2 us */
            if (handle->bus_write(1) != 0)                                  /* write 1 */
            {
                handle->enable_irq();                                       /* enable irq */
                handle->debug_print("ds18b20: bus write failed.\n");        /* write failed */
                
                return 1;                                                   /* return error */
            }
            handle->delay_us(60);                                           /* wait 60 us */
        }
        else                                                                /* write 0 */
        {
            if (handle->bus_write(0) != 0)                                  /* write 0 */
            {
                handle->enable_irq();                                       /* enable irq */
                handle->debug_print("ds18b20: bus write failed.\n");        /* write failed */
                
                return 1;                                                   /* return error */
            }
            handle->delay_us(60);                                           /* wait 60 us */
            if (handle->bus_write(1) != 0)                                  /* write 1 */
            {
                handle->enable_irq();                                       /* enable irq */
                handle->debug_print("ds18b20: bus write failed.\n");        /* write failed */
                
                return 1;                                                   /* return error */
            }
            handle->delay_us(2);                                            /* wait 2 us */
        }
    }
    handle->enable_irq();                                                   /* enable irq */
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t ds18b20_set_mode(ds18b20_handle_t *handle, ds18b20_mode_t mode)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->mode = (uint8_t)mode;   /* set mode */
    
    return 0;                       /* success return 0 */
}

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
uint8_t ds18b20_get_mode(ds18b20_handle_t *handle, ds18b20_mode_t *mode)
{
    if (handle == NULL)                            /* check handle */
    {
        return 2;                                  /* return error */
    }
    if (handle->inited != 1)                       /* check handle initialization */
    {
        return 3;                                  /* return error */
    }
    
    *mode = (ds18b20_mode_t)(handle->mode);        /* get mode */
    
    return 0;                                      /* success return 0 */
}

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
uint8_t ds18b20_set_rom(ds18b20_handle_t *handle, uint8_t rom[8])
{
    if (handle == NULL)                  /* check handle */
    {
        return 2;                        /* return error */
    }
    if (handle->inited != 1)             /* check handle initialization */
    {
        return 3;                        /* return error */
    }
    
    memcpy(handle->rom, rom , 8);        /* copy rom */
    
    return 0;                            /* success return 0 */
}

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
uint8_t ds18b20_get_rom(ds18b20_handle_t *handle, uint8_t rom[8])
{
    uint8_t i;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    if (a_ds18b20_reset(handle) != 0)                                   /* reset bus */
    {
        handle->debug_print("ds18b20: bus rest failed.\n");             /* reset bus failed */
        
        return 1;                                                       /* return error */
    }
    if (a_ds18b20_write_byte(handle, DS18B20_CMD_READ_ROM) != 0)        /* write read rom command */
    {
        handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
        
        return 1;                                                       /* return error */
    }
    for (i = 0; i < 8; i++)                                             /* read 8 bytes */
    {
        if (a_ds18b20_read_byte(handle, (uint8_t *)&rom[i]) != 0)       /* read 1 byte */
        {
            handle->debug_print("ds18b20: read rom failed.\n");         /* read failed */
            
            return 1;                                                   /* return error */
        }
    }
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t ds18b20_scratchpad_set_resolution(ds18b20_handle_t *handle, ds18b20_resolution_t resolution)
{
    uint8_t i, buf[9];
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    if (handle->mode == DS18B20_MODE_SKIP_ROM)                                  /* if use skip rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* bus reset failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_SKIP_ROM) != 0)            /* sent skip rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        } 
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_READ_SCRATCHPAD) != 0)     /* sent read scratchpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        } 
        for (i = 0; i < 9; i++)
        {
            if (a_ds18b20_read_byte(handle, (uint8_t *)&buf[i]) != 0)           /* read 9 bytes */
            {
                handle->debug_print("ds18b20: read data failed.\n");            /* read data failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_check_crc((uint8_t *)buf, 8, buf[8]) != 0)                /* check crc */
        {
            handle->debug_print("ds18b20: crc check error.\n");                 /* crc check error */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* bus reset failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_SKIP_ROM) != 0)            /* write skip rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_WRITE_SCRATCHPAD) != 0)    /* write scratchpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        buf[4] &= ~(3 << 5);                                                    /* clear resolution bits */
        buf[4] |= resolution << 5;                                              /* set resolution bits */
        for (i = 0; i < 3; i++)
        {
            if (a_ds18b20_write_byte(handle, buf[2+i]) != 0)                    /* write command */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
                
                return 1;                                                       /* return error */
            }
        }
        
        return 0;                                                               /* success return 0 */
    }
    else if (handle->mode == DS18B20_MODE_MATCH_ROM)                            /* if use match rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* bus reset failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_MATCH_ROM) != 0)           /* send match rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 8; i++)
        {
            if (a_ds18b20_write_byte(handle, handle->rom[i]) != 0)              /* send rom */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_READ_SCRATCHPAD) != 0)     /* send read scratchpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        } 
        for (i = 0; i < 9; i++)
        {
            if (a_ds18b20_read_byte(handle, (uint8_t *)&buf[i]) != 0)           /* read 9 bytes */
            {
                handle->debug_print("ds18b20: read data failed.\n");            /* read data failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_check_crc((uint8_t *)buf, 8, buf[8]) != 0)                /* check crc */
        {
            handle->debug_print("ds18b20: crc check error.\n");                 /* crc check error */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* reset bus failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_MATCH_ROM) != 0)           /* match rom */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 8; i++)
        {
            if (a_ds18b20_write_byte(handle, handle->rom[i]) != 0)              /* send rom */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_WRITE_SCRATCHPAD) != 0)    /* write scratchpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        buf[4] &= ~(3 << 5);                                                    /* clear resolution bits */
        buf[4] |= resolution << 5;                                              /* set resolution bits */
        for (i = 0; i < 3; i++)
        {
            if (a_ds18b20_write_byte(handle, buf[2 + i]) != 0)                  /* write command */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
                
                return 1;                                                       /* return error */
            }
        }
        
        return 0;                                                               /* success return 0 */
    }    
    else
    {
        handle->debug_print("ds18b20: mode invalid.\n");                        /* ds18b20 mode invlaid */
        
        return 1;                                                               /* return error */
    }
}

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
uint8_t ds18b20_scratchpad_get_resolution(ds18b20_handle_t *handle, ds18b20_resolution_t *resolution)
{
    uint8_t i, buf[9];
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    if (handle->mode == DS18B20_MODE_SKIP_ROM)                                  /* if use skip rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* reset bus */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_SKIP_ROM) != 0)            /* sent skip rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_READ_SCRATCHPAD) != 0)     /* write read scratchpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 9; i++)                                                 /* read 9 bytes */
        {
            if (a_ds18b20_read_byte(handle, (uint8_t *)&buf[i]) != 0)           /* read bytes */
            {
                handle->debug_print("ds18b20: read data failed.\n");            /* read data failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_check_crc((uint8_t *)buf, 8, buf[8]) != 0)                /* check crc */
        {
            handle->debug_print("ds18b20: crc check error.\n");                 /* crc check failed */
            
            return 1;                                                           /* return error */
        }
        *resolution = (ds18b20_resolution_t)(buf[4] >> 5);                      /* get resolution */
        
        return 0;                                                               /* success return 0 */
    }
    else if (handle->mode == DS18B20_MODE_MATCH_ROM)                            /* if use match rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* reset failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_MATCH_ROM) != 0)           /* sent match rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 8; i++)
        {
            if (a_ds18b20_write_byte(handle, handle->rom[i]) != 0)              /* send rom */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_READ_SCRATCHPAD) != 0)     /* sent read scratchpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
         } 
        for (i = 0; i < 9; i++)                                                     
        {
            if (a_ds18b20_read_byte(handle, (uint8_t *)&buf[i]) != 0)           /* read 9 bytes */
            {
                handle->debug_print("ds18b20: read dtaa failed.\n");            /* read failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_check_crc((uint8_t *)buf, 8, buf[8]) != 0)                /* check crc */
        {
            handle->debug_print("ds18b20: crc check error.\n");                 /* check crc failed */
            
            return 1;                                                           /* return error */
        }
        *resolution = (ds18b20_resolution_t)(buf[4] >> 5);                      /* get resolution */
        
        return 0;                                                               /* success return 0 */
    }
    else
    {
        handle->debug_print("ds18b20: mode invalid.\n");                        /* ds18b20 mode invalid */
        
        return 1;                                                               /* return error */
    }
}

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
uint8_t ds18b20_scratchpad_set_alarm_threshold(ds18b20_handle_t *handle, int8_t threshold_high, int8_t threshold_low)
{
    uint8_t i, buf[9];
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    if (handle->mode == DS18B20_MODE_SKIP_ROM)                                  /* if use skip rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* reset bus failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_SKIP_ROM) != 0)            /* sent skip rom commmand */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_READ_SCRATCHPAD) != 0)     /* sent read scrachpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 9; i++)
        {
            if (a_ds18b20_read_byte(handle, (uint8_t *)&buf[i]) != 0)           /* read 9 bytes */
            {
                handle->debug_print("ds18b20: read data failed.\n");            /* read data failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_check_crc((uint8_t *)buf, 8, buf[8]) != 0)                /* check crc */
        {
            handle->debug_print("ds18b20: crc check error.\n");                 /* crc check error */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* reset bus failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_SKIP_ROM) != 0)            /* sent skip rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_WRITE_SCRATCHPAD) != 0)    /* sent write scratchpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        buf[2] = (uint8_t)threshold_high;                                       /* set high threshold */
        buf[3] = (uint8_t)threshold_low;                                        /* set low threshold */
        for (i = 0; i < 3; i++)
        {
            if (a_ds18b20_write_byte(handle, buf[2 + i]) != 0)                  /* write command */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
                
                return 1;                                                       /* return error */
            }
        }
        
        return 0;                                                               /* success return 0 */
    }
    else if (handle->mode == DS18B20_MODE_MATCH_ROM)                            /* if use match rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* bus reset failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_MATCH_ROM) != 0)           /* sent match rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 8; i++)
        {
            if (a_ds18b20_write_byte(handle, handle->rom[i]) != 0)              /* send rom */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_READ_SCRATCHPAD) != 0)     /* sent read scrachpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 9; i++)
        {
            if (a_ds18b20_read_byte(handle, (uint8_t *)&buf[i]) != 0)           /* read 9 byte */
            {
                handle->debug_print("ds18b20: read data failed.\n");            /* read data failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_check_crc((uint8_t *)buf, 8, buf[8]) != 0)                /* check crc */
        {
            handle->debug_print("ds18b20: crc check error.\n");                 /* crc check error */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* reset bus failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_MATCH_ROM) != 0)           /* sent match rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 8; i++)
        {
            if (a_ds18b20_write_byte(handle, handle->rom[i]) != 0)              /* send rom */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_WRITE_SCRATCHPAD) != 0)    /* write scratchpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        buf[2] = (uint8_t)threshold_high;                                       /* set high threshold */
        buf[3] = (uint8_t)threshold_low;                                        /* set low threshold */
        for (i = 0; i < 3; i++)
        {
            if (a_ds18b20_write_byte(handle, buf[2 + i]) != 0)                  /* write command */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
                
                return 1;                                                       /* return error */
            }
        }
        
        return 0;                                                               /* success return 0 */
    }    
    else
    {
        handle->debug_print("ds18b20: mode invalid.\n");                        /* ds18b20 mode is invalid */
        
        return 1;                                                               /* return error */
    }
}

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
uint8_t ds18b20_scrachpad_get_alarm_threshold(ds18b20_handle_t *handle, int8_t *threshold_high, int8_t *threshold_low)
{
    uint8_t i, buf[9];
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    if (handle->mode == DS18B20_MODE_SKIP_ROM)                                  /* if use skip rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* reset bus failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_SKIP_ROM) != 0)            /* sent skip rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_READ_SCRATCHPAD) != 0)     /* sent read scrachpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 9; i++)
        {
            if (a_ds18b20_read_byte(handle, (uint8_t *)&buf[i]) != 0)           /* read 9 bytes */
            {
                handle->debug_print("ds18b20: read data failed.\n");            /* read data failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_check_crc((uint8_t *)buf, 8, buf[8]) != 0)                /* check crc */
        {
            handle->debug_print("ds18b20: crc check error.\n");                 /* crc check error */
            
            return 1;                                                           /* return error */
        }
        *threshold_high = (int8_t)(buf[2]);                                     /* get high threshold */
        *threshold_low = (int8_t)(buf[3]);                                      /* get low threshold */
        
        return 0;                                                               /* success return 0 */
    }
    else if (handle->mode == DS18B20_MODE_MATCH_ROM)                            /* if use match rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* reset bus failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_MATCH_ROM) != 0)           /* write match rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 8; i++)
        {
            if (a_ds18b20_write_byte(handle, handle->rom[i]) != 0)              /* send rom */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_READ_SCRATCHPAD) != 0)     /* sent read scratchpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 9; i++)                                                 /* read 9 bytes */
        {
            if (a_ds18b20_read_byte(handle, (uint8_t *)&buf[i]) != 0)           /* read bytes */
            {
                handle->debug_print("ds18b20: read data failed.\n");            /* read byte failed */
                
                return 1;                                                       /* return error */ 
            }
        }
        if (a_ds18b20_check_crc((uint8_t *)buf, 8, buf[8]) != 0)                /* check crc */
        {
            handle->debug_print("ds18b20: crc check error.\n");                 /* crc error */
            
            return 1;                                                           /* return error */
        }
        *threshold_high = (int8_t)(buf[2]);                                     /* get high threshold */
        *threshold_low = (int8_t)(buf[3]);                                      /* get low threshold */
        
        return 0;                                                               /* success return 0 */
    }
    else
    {
        handle->debug_print("ds18b20: mode invalid.\n");                        /* ds18b20 mode invalid */
        
        return 1;                                                               /* return error */
    }
}

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
uint8_t ds18b20_copy_scratchpad_to_eeprom(ds18b20_handle_t *handle)
{
    uint8_t i;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    if (handle->mode == DS18B20_MODE_SKIP_ROM)                                  /* if use skip rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* reset bus failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_SKIP_ROM) != 0)            /* sent skip rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_COPY_SCRATCHPAD) != 0)     /* write copy scratchpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        
        return 0;                                                               /* success return 0 */
    }
    else if (handle->mode == DS18B20_MODE_MATCH_ROM)                            /* if use match rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* bus reset failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_MATCH_ROM) != 0)           /* write match rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }    
        for (i = 0; i < 8; i++)
        {
            if (a_ds18b20_write_byte(handle, handle->rom[i]) != 0)              /* send rom */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_COPY_SCRATCHPAD) != 0)     /* write copy scratchpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        
        return 0;                                                               /* success return 0 */
    }
    else
    {
        handle->debug_print("ds18b20: mode invalid.\n");                        /* ds18b20 mode is invalid */
        
        return 1;                                                               /* return error */
    }
}

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
uint8_t ds18b20_copy_eeprom_to_scratchpad(ds18b20_handle_t *handle)
{
    uint8_t i;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    if (handle->mode == DS18B20_MODE_SKIP_ROM)                                  /* if use in skip rom */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* reset bus failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_SKIP_ROM) != 0)            /* sent skip rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_RECALL_EE) != 0)           /* write recall ee command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        
        return 0;                                                               /* success return 0 */
    }
    else if (handle->mode == DS18B20_MODE_MATCH_ROM)                            /* if use match rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* reset bus failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_MATCH_ROM) != 0)           /* sent match rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 8; i++)
        {
            if (a_ds18b20_write_byte(handle, handle->rom[i]) != 0)              /* send rom */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_RECALL_EE) != 0)           /* sent recall ee command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        
        return 0;                                                               /* success return 0 */
    }
    else
    {
        handle->debug_print("ds18b20: mode invalid.\n");                        /* ds18b20 mode is invalid */
        
        return 1;                                                               /* return error */
    }
}

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
uint8_t ds18b20_alarm_convert_to_register(ds18b20_handle_t *handle, float temp, int8_t *reg)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *reg = (int8_t)(temp);          /* convert real data to register data */
    
    return 0;                       /* success return 0 */
}

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
uint8_t ds18b20_alarm_convert_to_data(ds18b20_handle_t *handle, int8_t reg, float *temp)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *temp = (float)(reg);           /* convert raw data to real data */
    
    return 0;                       /* success return 0 */
}

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
uint8_t ds18b20_init(ds18b20_handle_t *handle)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->debug_print == NULL)                                   /* check debug_print */
    {
        return 3;                                                      /* return error */
    }
    if (handle->bus_init == NULL)                                      /* check bus_init */
    {
        handle->debug_print("ds18b20: bus_init is null.\n");           /* bus_init is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->bus_deinit == NULL)                                    /* check bus_deinit */
    {
        handle->debug_print("ds18b20: bus_deinit is null.\n");         /* bus_read is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->bus_read == NULL)                                      /* check bus_read */
    {
        handle->debug_print("ds18b20: bus_read is null.\n");           /* bus_read is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->bus_write == NULL)                                     /* check bus_write */
    {
        handle->debug_print("ds18b20: bus_write is null.\n");          /* bus_write is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->delay_ms == NULL)                                      /* check delay_ms */
    {
        handle->debug_print("ds18b20: delay_ms is null.\n");           /* delay_ms is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->delay_us == NULL)                                      /* check delay_us */
    {
        handle->debug_print("ds18b20: delay_us is null.\n");           /* delay_us is null */
       
        return 3;                                                      /* return error */
    }
    if (handle->enable_irq == NULL)                                    /* check enable_irq */
    {
        handle->debug_print("ds18b20: enable_irq is null.\n");         /* enable_irq is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->disable_irq == NULL)                                   /* check disable_irq */
    {
        handle->debug_print("ds18b20: disable_irq is null.\n");        /* disable_irq is null */
        
        return 3;                                                      /* return error */
    }
    
    if (handle->bus_init() != 0)                                       /* initialize bus */
    {
        handle->debug_print("ds18b20: bus init failed.\n");            /* bus innit failed */
        
        return 1;                                                      /* return error */
    }
    if (a_ds18b20_reset(handle) != 0)                                  /* reset chip */
    {
        handle->debug_print("ds18b20: reset failed.\n");               /* reset chip failed */
        (void)handle->bus_deinit();                                    /* close bus */
        
        return 4;                                                      /* return error */
    }
    handle->inited = 1;                                                /* flag finish initialization */
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t ds18b20_deinit(ds18b20_handle_t *handle)
{
    if (handle == NULL)                                          /* check handle */
    {
        return 2;                                                /* return error */
    }
    if (handle->inited != 1)                                     /* check handle initialization */
    {
        return 3;                                                /* return error */
    }
    
    if (handle->bus_deinit() != 0)                               /* close bus */
    {
        handle->debug_print("ds18b20: deinit failed.\n");        /* deinit failed */
        
        return 1;                                                /* return error */
    }   
    handle->inited = 0;                                          /* flag close */
    
    return 0;                                                    /* success return 0 */
}

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
uint8_t ds18b20_read(ds18b20_handle_t *handle, int16_t *raw, float *temp)
{
    uint8_t i, buf[9];
    uint8_t res;
    uint32_t cnt;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    if (handle->mode == DS18B20_MODE_SKIP_ROM)                                  /* if use skip rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* bus reset failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_SKIP_ROM) != 0)            /* sent skip rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_CONVERT_T) != 0)           /* sent convert temp command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        cnt = 0;                                                                /* reset cnt */
        res = 0;                                                                /* reset res */
        while ((res == 0) && (cnt < 100))                                       /* wait 1 s */
        {
            if (a_ds18b20_read_bit(handle, (uint8_t *)&res) != 0)               /* read 1 bit */
            {
                handle->debug_print("ds18b20: read bit failed.\n");             /* read bit failed */
                
                return 1;                                                       /* return error */
            }
            handle->delay_ms(10);                                               /* delay 10 ms */
            cnt++;                                                              /* cnt++ */
        }
        if (cnt >= 100)                                                         /* if cnt is over 100 times */
        {
            handle->debug_print("ds18b20: bus read timeout.\n");                /* bus read timeout */
            
            return 1;                                                           /* reset error */
        }
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* bus reset failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_SKIP_ROM) != 0)            /* send skip rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_READ_SCRATCHPAD) != 0)     /* write read scrachpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 9; i++)                                                 /* read 9 bytes */
        {
            if (a_ds18b20_read_byte(handle, (uint8_t *)&buf[i]) != 0)           /* read bytes */
            {
                handle->debug_print("ds18b20: read byte failed.\n");            /* read byte failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_check_crc((uint8_t *)buf, 8, buf[8]) != 0)                /* check crc */
        {
            handle->debug_print("ds18b20: crc check error.\n");                 /* crc check failed */
            
            return 1;                                                           /* return error */
        }
        *raw = (int16_t)(((uint16_t)buf[1]) << 8) | buf[0];                     /* get raw data */
        if (((buf[4] >> 5) & 0x03) == DS18B20_RESOLUTION_9BIT)                  /* if 9 bit resolution */
        {
            if ((((uint16_t)(*raw)) & (1 << 15)) != 0)                          /* if negtive */
            {
                *raw = (*raw ) >> 3;                                            /* right shift 3 */
                *raw = (*raw) | 0xE000U;                                        /* set negtive part */
            }
            else                                                                /* if positive */
            {
                *raw = (*raw ) >> 3;                                            /* right shift 3 */
            }
            *temp = (float)(*raw) * 0.5f;                                       /* convert to real data */
        }
        else if (((buf[4] >> 5) & 0x03) == DS18B20_RESOLUTION_10BIT)            /* if 10 bit resolution */
        {
            if ((((uint16_t)(*raw)) & (1 << 15)) != 0)                          /* if negtive */
            {
                *raw = (*raw ) >> 2;                                            /* right shift 2 */
                *raw = (*raw) | 0xC000U;                                        /* set negtive part */
            }
            else
            {
                *raw = (*raw ) >> 2;                                            /* right shift 2 */
            }
            *temp = (float)(*raw) * 0.25f;                                      /* convert to real data */
        }
        else if (((buf[4] >> 5) & 0x03) == DS18B20_RESOLUTION_11BIT)            /* if 11 bit resolution */
        {
            if ((((uint16_t)(*raw)) & (1 << 15)) != 0)                          /* if negtive */
            {
                *raw = (*raw ) >> 1;                                            /* right shift 1 */
                *raw = (*raw) | 0x8000U;                                        /* set negtive part */
            }
            else
            {
                *raw = (*raw ) >> 1;                                            /* right shift 1 */
            }
            *temp = (float)(*raw) * 0.125f;                                     /* convert to real data */
        }
        else if (((buf[4] >> 5) & 0x03) == DS18B20_RESOLUTION_12BIT)            /* if 12 bit resolution */
        {
            *raw = (*raw ) >> 0;                                                /* right shift 0 */
            *temp = (float)(*raw) * 0.0625f;                                    /* convert to real data */
        }
        else
        {
            handle->debug_print("ds18b20: resolution invalid.\n");              /* resolution is invalid */
            
            return 1;                                                           /* return error */
        }
        
        return 0;                                                               /* success return 0 */
    }
    else if (handle->mode == DS18B20_MODE_MATCH_ROM)                            /* if use match rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* reset bus failed */
        
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_MATCH_ROM) != 0)           /* sent match rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */

            return 1;                                                           /* return error */
        }
        for (i = 0; i < 8; i++)
        {
            if (a_ds18b20_write_byte(handle, handle->rom[i]) != 0)              /* send rom */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* wrtie command failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_CONVERT_T) != 0)           /* sent convert temp command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        cnt = 0;                                                                /* reset cnt */
        res = 0;                                                                /* reset res */
        while ((res == 0) && (cnt < 100))                                       /* read max 1s */
        {
            if (a_ds18b20_read_bit(handle, (uint8_t *)&res) != 0)               /* read 1 bit */
            {
                handle->debug_print("ds18b20: read bit failed.\n");             /* read 1 bit failed */
                
                return 1;                                                       /* return error */
            }
            handle->delay_ms(10);                                               /* delay 10 ms */
            cnt++;                                                              /* cnt++ */
        }
        if (cnt >= 100)                                                         /* if cnt is over 100 times */
        {
            handle->debug_print("ds18b20: bus read timeout.\n");                /* read timeout */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_reset(handle) != 0)                                       /* reset bus */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* reset bus failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_MATCH_ROM) != 0)           /* sent match rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 8; i++)
        {
            if (a_ds18b20_write_byte(handle, handle->rom[i]) != 0)              /* send rom */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
                
                return 1;                                                       /* return error */
            }
        } 
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_READ_SCRATCHPAD) != 0)     /* send read scrachpad command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        for (i = 0; i < 9; i++)                                                 /* read 9 bytes */
        {
            if (a_ds18b20_read_byte(handle, (uint8_t *)&buf[i]) != 0)           /* read byte */
            {
                handle->debug_print("ds18b20: read byte failed.\n");            /* read failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_check_crc((uint8_t *)buf, 8, buf[8]) != 0)                /* check crc */
        {
            handle->debug_print("ds18b20: crc check error.\n");                 /* crc check error */
            
            return 1;                                                           /* return error */
        }
        *raw = (int16_t)(((uint16_t)buf[1]) << 8) | buf[0];                     /* get raw data */
        if (((buf[4] >> 5) & 0x03) == DS18B20_RESOLUTION_9BIT)                  /* if 9 bit resolution */
        {
            if ((((uint16_t)(*raw)) & (1 << 15)) != 0)                          /* if negtive */
            {
                *raw = (*raw ) >> 3;                                            /* right shift 3 */
                *raw = (*raw) | 0xE000U;                                        /* set negtive part */
            }
            else                                                                /* if positive */
            {
                *raw = (*raw ) >> 3;                                            /* right shift 3 */
            }
            *temp = (float)(*raw) * 0.5f;                                       /* convert to real data */
        }
        else if (((buf[4] >> 5) & 0x03) == DS18B20_RESOLUTION_10BIT)            /* if 10 bit resolution */
        {
            if ((((uint16_t)(*raw)) & (1 << 15)) != 0)                          /* if negtive */
            {
                *raw = (*raw ) >> 2;                                            /* right shift 2 */
                *raw = (*raw) | 0xC000U;                                        /* set negtive part */
            }
            else
            {
                *raw = (*raw ) >> 2;                                            /* right shift 2 */
            }
            *temp = (float)(*raw) * 0.25f;                                      /* convert to real data */
        }
        else if (((buf[4] >> 5) & 0x03) == DS18B20_RESOLUTION_11BIT)            /* if 11 bit resolution */
        {
            if ((((uint16_t)(*raw)) & (1 << 15)) != 0)                          /* if negtive */
            {
                *raw = (*raw ) >> 1;                                            /* right shift 1 */
                *raw = (*raw) | 0x8000U;                                        /* set negtive part */
            }
            else
            {
                *raw = (*raw ) >> 1;                                            /* right shift 1 */
            }
            *temp = (float)(*raw) * 0.125f;                                     /* convert to real data */
        }
        else if (((buf[4] >> 5) & 0x03) == DS18B20_RESOLUTION_12BIT)            /* if 12 bit resolution */
        {
            *raw = (*raw ) >> 0;                                                /* right shift 0 */
            *temp = (float)(*raw) * 0.0625f;                                    /* convert to real data */
        }
        else
        {
            handle->debug_print("ds18b20: resolution invalid.\n");              /* resolution is invalid */
            
            return 1;                                                           /* return error */
        }
        
        return 0;                                                               /* success return 0 */
    }
    else
    {
        handle->debug_print("ds18b20: mode invalid.\n");                        /* ds18b20 mode is invalid */
        
        return 1;                                                               /* return error */
    }
}

/**
 * @brief      read 2 bits from the bus
 * @param[in]  *handle points to a ds18b20 handle structure
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_ds18b20_read_2bit(ds18b20_handle_t *handle, uint8_t *data)    
{
    uint8_t i;
    uint8_t res;
    
    *data = 0;                                                          /* reset data */
    handle->disable_irq();                                              /* disable irq */
    for (i = 0; i < 2; i++)                                             /* read 2 bit */
    {
        *data <<= 1;                                                    /* left shift 1 */
        if (a_ds18b20_read_bit(handle, (uint8_t *)&res) != 0)           /* read one bit */
        {
            handle->enable_irq();                                       /* enable irq */
            handle->debug_print("ds18b20: read bit failed.\n");         /* read bit failed */
            
            return 1;                                                   /* return error */
        }
        *data = (*data) | res;                                          /* get 1 bit */
    }
    handle->enable_irq();                                               /* enable irq */
    
    return 0;                                                           /* success return 0 */
}


/**
 * @brief     write 1 bit to the bus
 * @param[in] *handle points to a ds18b20 handle structure
 * @param[in] bit is the written bit
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ds18b20_write_bit(ds18b20_handle_t *handle, uint8_t bit)
{    
    handle->disable_irq();                                          /* disable irq */
    if (handle->bus_write(0) != 0)                                  /* write 0 */
    {
        handle->enable_irq();                                       /* enable irq */
        handle->debug_print("ds18b20: write bit failed.\n");        /* write bit failed */
        
        return 1;                                                   /* return error */
    }
    handle->delay_us(12);                                           /* wait 12 us */
    if (handle->bus_write(bit) != 0)                                /* write bit */
    {
        handle->enable_irq();                                       /* enable irq */
        handle->debug_print("ds18b20: write bit failed.\n");        /* write bit failed */
        
        return 1;                                                   /* return error */
    } 
    handle->delay_us(30);                                           /* wait 30 us */
    if (handle->bus_write(1) != 0)                                  /* write 1 */
    {
        handle->enable_irq();                                       /* enable irq */
        handle->debug_print("ds18b20: write bit failed.\n");        /* write bit failed */
        
        return 1;                                                   /* return error */
    }
    handle->delay_us(5);                                            /* wait 5 us */
    handle->enable_irq();                                           /* enable irq */
    
    return 0;                                                       /* success return 0 */
}    

/**
 * @brief         search the ds18b20 bus
 * @param[in]     *handle points to a ds18b20 handle structure
 * @param[out]    **pid points to a rom array
 * @param[in]     cmd is the search command
 * @param[in,out] *number points to a array size buffer
 * @return        status code
 *                - 0 success
 *                - 1 search failed
 * @note          none
 */
static uint8_t a_ds18b20_search(ds18b20_handle_t *handle, uint8_t (*pid)[8], uint8_t cmd, uint8_t *number)
{     
    uint8_t k, l = 0, conflict_bit, m, n;
    uint8_t buffer[DS18B20_MAX_SEARCH_SIZE];
    uint8_t ss[64];
    uint8_t s = 0;
    uint8_t num = 0;
    
    if ((*number) > DS18B20_MAX_SEARCH_SIZE)                                              /* check number */
    {
        handle->debug_print("ds18b20: number is over DS18B20_MAX_SEARCH_SIZE.\n");        /* number is over */
        
        return 1;                                                                         /* return error */
    }
    memset((uint8_t *)buffer, 0, DS18B20_MAX_SEARCH_SIZE);                                /* clear buffer */
    memset((uint8_t *)ss, 0, sizeof(uint8_t) * 64);                                       /* clear buffer */
    do
    {
        if (a_ds18b20_reset(handle) != 0)                                                 /* reset bus */
        {
            handle->debug_print("ds18b20: reset failed.\n");                              /* reset bus failed */
            
            return 1;                                                                     /* return error */
        }
        if (a_ds18b20_write_byte(handle, cmd) != 0)                                       /* write 1 byte */
        {
            handle->debug_print("ds18b20: write command failed.\n");                      /* write command failed */
            
            return 1;                                                                     /* return error */
        }
        for (m = 0; m < 8; m++)                                                           /* read 8 byte */
        {
            for (n = 0; n < 8; n++)                                                       /* read 8 bit */
            {
                if (a_ds18b20_read_2bit(handle, (uint8_t *)&k) != 0)                      /* read 2 bit */
                {
                    handle->debug_print("ds18b20: read 2bit failed.\n");                  /* read 2 bit failed */
                    
                    return 1;                                                             /* return error */
                }
                k = k & 0x03;                                                             /* get valid bits */
                s = s >> 1;                                                               /* right shift 1 bit */
                if (k == 0x01)                                                            /* 0000 0001 */
                {
                    if (a_ds18b20_write_bit(handle, 0) != 0)                              /* write 0 */
                    {
                        handle->debug_print("ds18b20: write bit failed.\n");              /* write bit failed */
                        
                        return 1;                                                         /* return error */
                    }
                    ss[(m * 8 + n)] = 0;                                                  /* set 0 */
                }
                else if (k == 0x02)                                                       /* 0000 0010 */
                {
                    s = s | 0x80;                                                         /* set 7 bit */
                    if (a_ds18b20_write_bit(handle, 1) != 0)                              /* write 1 */
                    {
                        handle->debug_print("ds18b20: write bit failed.\n");              /* write bit failed */
                        
                        return 1;                                                         /* return error */
                    }
                    ss[(m * 8 + n)] = 1;                                                  /* set 1 */
                }    
                else if (k == 0x00)                                                       /* if 0000 */
                {
                    conflict_bit = (uint8_t)(m * 8 + n + 1);                              /* flag conflict bit */
                    if (conflict_bit > buffer[l])                                         /* check buffer */
                    {
                        if (a_ds18b20_write_bit(handle, 0) != 0)                          /* write 0 */
                        {
                            handle->debug_print("ds18b20: write bit failed.\n");          /* write bit failed */
                            
                            return 1;                                                     /* return error */
                        }
                        ss[(m * 8 + n)] = 0;                                              /* set 0 */
                        buffer[++l] = conflict_bit;                                       /* set conflict bit */
                    }
                    else if (conflict_bit < buffer[l])                                    /* if > buffer */
                    {
                        s = s|((ss[(m * 8 + n)] & 0x01) << 7);                            /* get s */
                        if (a_ds18b20_write_bit(handle, ss[(m*8+n)]) != 0)                /* write data */
                        {
                            handle->debug_print("ds18b20: write bit failed.\n");          /* write bit failed */
                            
                            return 1;                                                     /* return error */
                        }
                    }    
                    else if (conflict_bit == buffer[l])                                   /* if == buffer */
                    {
                        s = s | 0x80;                                                     /* set 7 bit */
                        if (a_ds18b20_write_bit(handle, 1) != 0)                          /* write 1 */
                        {
                            handle->debug_print("ds18b20: write bit failed.\n");          /* write bit failed */
                            
                            return 1;                                                     /* return error */
                        }
                        ss[(m * 8 + n)] = 1;                                              /* set 1 */
                        l = l-1;                                                          /* l-- */
                    }
                    else
                    {
                        
                    }
                }
                else
                {
                    *number = num;                                                        /* save num */
                    
                    return 0;                                                             /* success return 0 */
                }
                handle->delay_us(5);                                                      /* delay 5 us */
            }
            pid[num][m] = s;                                                              /* save s */
            s = 0;                                                                        /* reset s */
        }
        num++;                                                                            /* num++ */
        if (num > (*number))                                                              /* check num range */
        {
            break;                                                                        /* break */
        }
    } while (buffer[l] != 0);                                                             /* check buffer[l] */
    *number = num;                                                                        /* set number */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t ds18b20_search_rom(ds18b20_handle_t *handle, uint8_t (*rom)[8], uint8_t *num)
{
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    return a_ds18b20_search(handle, rom, DS18B20_CMD_SEARCH_ROM, num);       /* return search result */
}

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
uint8_t ds18b20_search_alarm(ds18b20_handle_t *handle, uint8_t (*rom)[8], uint8_t *num)
{
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    return a_ds18b20_search(handle, rom, DS18B20_CMD_ALARM_SEARCH, num);       /* return search result */
}

/**
 * @brief      get the power mode
 * @param[in]  *handle points to a ds18b20 handle structure
 * @param[out] *power_mode points to a power mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get power mode failed
 * @note       none
 */
uint8_t ds18b20_get_power_mode(ds18b20_handle_t *handle, ds18b20_power_mode_t *power_mode)
{
    uint8_t i;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    if (handle->mode == DS18B20_MODE_SKIP_ROM)                                  /* if use skip rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* bus reset */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* bus reset failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_SKIP_ROM) != 0)            /* sent skip rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_READ_POWER_SUPPLY) != 0)   /* write read power supply command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_read_bit(handle, (uint8_t *)power_mode) != 0)             /* get power mode */
        {
            handle->debug_print("ds18b20: read bit failed.\n");                 /* read bit failed */
            
            return 1;                                                           /* return error */
        }
        
        return 0;                                                               /* success return 0 */
    }
    else if (handle->mode == DS18B20_MODE_MATCH_ROM)                            /* if use match rom mode */
    {
        if (a_ds18b20_reset(handle) != 0)                                       /* bus reset */
        {
            handle->debug_print("ds18b20: bus reset failed.\n");                /* bus reset failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_MATCH_ROM) != 0)           /* sent match rom command */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */

            return 1;                                                           /* return error */
        }
        for (i = 0; i < 8; i++)
        {
            if (a_ds18b20_write_byte(handle, handle->rom[i]) != 0)              /* send rom */
            {
                handle->debug_print("ds18b20: write command failed.\n");        /* write command failed */
                
                return 1;                                                       /* return error */
            }
        }
        if (a_ds18b20_write_byte(handle, DS18B20_CMD_READ_POWER_SUPPLY) != 0)   /* write read power supply */
        {
            handle->debug_print("ds18b20: write command failed.\n");            /* write command failed */
            
            return 1;                                                           /* return error */
        }
        if (a_ds18b20_read_bit(handle, (uint8_t *)power_mode) != 0)             /* get power mode */
        {
            handle->debug_print("ds18b20: read bit failed.\n");                 /* read bit failed */
            
            return 1;                                                           /* return error */
        }
        
        return 0;                                                               /* success return 0 */
    }
    else
    {
        handle->debug_print("ds18b20: mode invalid.\n");                        /* mode is invalid */
        
        return 1;                                                               /* return error */
    }
}

/**
 * @brief      get chip's information
 * @param[out] *info points to a ds18b20 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ds18b20_info(ds18b20_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(ds18b20_info_t));                        /* initialize ds18b20 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "GPIO", 8);                            /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver verison */
    
    return 0;                                                       /* success return 0 */
}
