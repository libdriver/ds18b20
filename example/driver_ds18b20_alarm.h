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
 * @file      driver_ds18b20_alarm.h
 * @brief     driver ds18b20 alarm header file
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
  
#ifndef DRIVER_DS18B20_ALARM_H
#define DRIVER_DS18B20_ALARM_H

#include "driver_ds18b20_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief  alarm example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t ds18b20_alarm_init(void);

/**
 * @brief  alarm example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ds18b20_alarm_deinit(void);

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
uint8_t ds18b20_alarm_set_threshold(uint8_t rom[8], float low, float high);

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
uint8_t ds18b20_alarm_get_threshold(uint8_t rom[8], float *low, float *high);

/**
 * @brief         alarm example find
 * @param[in]     *rom points to a rom buffer
 * @param[in,out] *num points to a number buffer
 * @return        status code
 *                - 0 success
 *                - 1 search failed
 * @note          none
 */
uint8_t ds18b20_alarm_search(uint8_t (*rom)[8], uint8_t *num);

#ifdef __cplusplus
}
#endif

#endif
