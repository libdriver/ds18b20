[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS18B20
[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds18b20/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Das digitale Thermometer DS18B20 bietet 9-Bit- bis 12-Bit-Celsius-Temperaturmessungen und verfügt über eine Alarmfunktion mit nichtflüchtigen, vom Benutzer programmierbaren oberen und unteren Triggerpunkten. Der DS18B20 kommuniziert über einen 1-Wire-Bus, der per Definition nur eine Datenleitung (und Masse) für die Kommunikation mit einem zentralen Mikroprozessor benötigt. Darüber hinaus kann der DS18B20 Strom direkt von der Datenleitung ableiten („parasitäre Stromversorgung“), wodurch die Notwendigkeit einer externen Stromversorgung. Jeder DS18B20 verfügt über einen eindeutigen 64-Bit-Seriencode, der es mehreren DS18B20 ermöglicht, auf demselben 1-Wire-Bus zu funktionieren. Somit ist es einfach, einen Mikroprozessor zu verwenden, um viele über einen großen Bereich verteilte DS18B20 zu steuern. Zu den Anwendungen, die von dieser Funktion profitieren können, gehören HVAC-Umgebungssteuerungen, Temperaturüberwachungssysteme in Gebäuden, Geräten oder Maschinen sowie Prozessüberwachungs- und -steuerungssysteme.

LibDriver DS18B20 ist ein voll funktionsfähiger DS18B20-Treiber, der von LibDriver gestartet wurde. Es bietet die Funktionen Temperaturablesung, ROM-Abgleichtemperaturablesung, Alarmablesung und ROM-Suchfunktion. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example match](#example-match)
    - [example alarm](#example-alarm)
    - [example search](#example-search)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver DS18B20-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver DS18B20 onewire.

/test enthält den Testcode des LibDriver DS18B20-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver DS18B20-Beispielcode.

/doc enthält das LibDriver DS18B20-Offlinedokument.

/Datenblatt enthält DS18B20-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige onewire-Schnittstellenvorlage und stellen Sie Ihren Plattform-onewire-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/ds18b20/index.html](https://www.libdriver.com/docs/ds18b20/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.