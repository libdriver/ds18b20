[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS18B20

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds18b20/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

DS18B20デジタル体温計は9ビットから12ビットの摂氏温度測定を提供し、不揮発性のユーザープログラム可能な上下のトリガーポイントを備えたアラーム機能を備えています。 DS18B20は、中央マイクロプロセッサとの通信に定義上1本のデータライン（およびグランド）のみを必要とする1-Wireバスを介して通信します。さらに、DS18B20はデータラインから直接電力（「寄生電力」）を引き出すことができるため、外部電源の必要性。各DS18B20には固有の64ビットシリアルコードがあり、複数のDS18B20が同じ1-Wireバス上で機能できるようにします。したがって、1つのマイクロプロセッサを使用して、広範囲に分散された多数のDS18B20を制御するのは簡単です。この機能の恩恵を受けることができるアプリケーションには、HVAC環境制御、建物、機器、または機械内の温度監視システム、およびプロセス監視および制御システムが含まれます。

LibDriver DS18B20は、LibDriverによって起動されたフル機能のDS18B20ドライバーです。温度読み取り、ROMマッチング温度読み取り、アラーム読み取り、ROM検索機能の機能を提供します。 LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example match](#example-match)
    - [example alarm](#example-alarm)
    - [example search](#example-search)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver DS18B20のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver DS18B20用のプラットフォームに依存しないonewireバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver DS18B20ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver DS18B20プログラミング例が含まれています。

/ docディレクトリには、LibDriver DS18B20オフラインドキュメントが含まれています。

/ datasheetディレクトリには、DS18B20データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないonewireバステンプレートを参照して、指定したプラットフォームのonewireバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

```c
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

```c
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

```c
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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/ds18b20/index.html](https://www.libdriver.com/docs/ds18b20/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。