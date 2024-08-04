[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS18B20

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds18b20/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

DS18B20数字温度计提供9位至12位温度测量，并具有非易失性用户可编程上限报警功能。DS18B20通信在定义上只需要一个单总线与微处理器通信。此外，DS18B20还可以直接从数据线供电（“寄生电源”），无需外部电源。每个DS18B20都有一个唯一的64位串行序列号，允许多个DS18B20在同一单线上工作使用一个微处理器来控制大面积分布的多个DS18B20。该芯片被用于暖通空调环境控制、温度监控建筑物、设备或机械内部的系统，以及过程监控系统。

LibDriver DS18B20是LibDriver推出的DS18B20全功能驱动，该驱动提供温度读取、ROM匹配温度读取、报警读取和搜索ROM等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example match](#example-match)
    - [example alarm](#example-alarm)
    - [example search](#example-search)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver DS18B20的源文件。

/interface目录包含了LibDriver DS18B20与平台无关的onewire总线模板。

/test目录包含了LibDriver DS18B20驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver DS18B20编程范例。

/doc目录包含了LibDriver DS18B20离线文档。

/datasheet目录包含了DS18B20数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的onewire总线模板，完成指定平台的onewire总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

#### example basic

```C
#include "driver_ds18b20_basic.h"

uint8_t res;
uint8_t i;
float temperature;

res = ds18b20_basic_init();
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    ds18b20_interface_delay_ms(2000);
    res = ds18b20_basic_read((float *)&temperature);
    if (res != 0)
    {
        (void)ds18b20_basic_deinit();

        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: temperature is %0.2fC.\n", temperature);
    
    ...
    
}

...

(void)ds18b20_basic_deinit();

return 0;
```

#### example match

```C
#include "driver_ds18b20_match.h"

uint8_t res;
uint8_t i;
float temperature;
uint8_t rom[8];

res = ds18b20_match_init();
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    ds18b20_interface_delay_ms(2000);
    res = ds18b20_match_read((uint8_t *)rom, (float *)&temperature);
    if (res != 0)
    {
        (void)ds18b20_match_deinit();

        return 1;
    }
    ds18b20_interface_debug_print("ds18b20: temperature is %0.2fC.\n", temperature);
    
    ...
    
}

...

(void)ds18b20_match_deinit();

return 0;
```

#### example alarm

```C
#include "driver_ds18b20_alarm.h"

uint8_t res;
uint8_t rom[8][8];
uint8_t num;
uint8_t i, j;

res = ds18b20_alarm_init();
if (res != 0)
{
    return 1;
}

...

num = 8;
res = ds18b20_alarm_search((uint8_t (*)[8])rom, (uint8_t *)&num);
if (res != 0)
{
    (void)ds18b20_alarm_deinit();

    return 1;
}
ds18b20_interface_debug_print("ds18b20: find %d alarm rom(s).\n", num);

...

for (i = 0; i < num; i++)
{
    for (j = 0; j < 8; j++)
    {
        ds18b20_interface_debug_print("%02X ", rom[i][j]);
    }
    ds18b20_interface_debug_print(".\n");
    
    ...
    
}

...

(void)ds18b20_alarm_deinit();

return 0;
```

#### example search

```C
#include "driver_ds18b20_search.h"

uint8_t res, i, j;
uint8_t rom[8][8];
uint8_t num;

res = ds18b20_search_init();
if (res != 0)
{
    return 1;
}

...

num = 8;
res = ds18b20_search((uint8_t (*)[8])rom, (uint8_t *)&num);
if (res != 0)
{
    (void)ds18b20_search_deinit();

    return 1;
}
ds18b20_interface_debug_print("ds18b20: find %d rom(s).\n", num);

...

for (i = 0; i < num; i++)
{
    ds18b20_interface_debug_print("ds18b20: %d/%d is ", (uint32_t)(i+1), (uint32_t)num);
    for (j = 0; j < 8; j++)
    {
        ds18b20_interface_debug_print("%02X ", rom[i][j]);
    }
    ds18b20_interface_debug_print(".\n");
    
    ...
    
}

...
    
(void)ds18b20_search_deinit();

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/ds18b20/index.html](https://www.libdriver.com/docs/ds18b20/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。