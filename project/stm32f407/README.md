### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

DATA Pin: PA8.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. DS18B20

#### 3.1 Command Instruction

1. Show ds18b20 chip and driver information.

    ```shell
    ds18b20 (-i | --information)
    ```

2. Show ds18b20 help.

   ```shell
   ds18b20 (-h | --help)
   ```

3. Show ds18b20 pin connections of the current board.

   ```shell
   ds18b20 (-p | --port)
   ```

4. Run ds18b20 register test.

   ```shell
   ds18b20 (-t reg | --test=reg)
   ```

5.  Run ds18b20 read test, num means the test times.

   ```shell
   ds18b20 (-t read | --test=read) [--times=<num>]
   ```

6. Run ds18b20 search test.

   ```shell
   ds18b20 (-t search | --test=search)
   ```

7. Run ds18b20 read function, num means the read times.

   ```shell
   ds18b20 (-e read | --example=read) [--times=<num>]
   ```

8. Run ds18b20 match function, num means the read times, code means the rom code with the length of 8 and it is hexadecimal.

   ```shell
   ds18b20 (-e match | --example=match) --rom=<code> [--times=<num>]
   ```

9. Run ds18b20 search rom function.

   ```shell
   ds18b20 (-e search | --example=search)
   ```

10. Run alarm set threshold function, low means the low threshold, high means the high threshold, code means the rom code with the length of 8 and it is hexadecimal.

    ```shell
    ds18b20 (-e alarm-set | --example=alarm-set) --low-threshold=<low> --high-threshold=<high> --rom=<code>
    ```

11. Run alarm get threshold function, code means the rom code with the length of 8 and it is hexadecimal.

    ```shell
    ds18b20 (-e alarm-get | --example=alarm-get) --rom=<code>
    ```

12. Run ds18b20 search alarm rom function.

    ```shell
    ds18b20 (-e alarm-search | --example=alarm-search)
    ```

#### 3.2 Command Example

```shell
ds18b20 -i

ds18b20: chip is Maxim Integrated DS18B20.
ds18b20: manufacturer is Maxim Integrated.
ds18b20: interface is GPIO.
ds18b20: driver version is 2.0.
ds18b20: min supply voltage is 3.0V.
ds18b20: max supply voltage is 5.5V.
ds18b20: max current is 4.00mA.
ds18b20: max temperature is 125.0C.
ds18b20: min temperature is -55.0C.
```

```shell
ds18b20 -p

ds18b20: DQ pin connected to GPIOA PIN8.
```

```shell
ds18b20 -t reg

ds18b20: chip is Maxim Integrated DS18B20.
ds18b20: manufacturer is Maxim Integrated.
ds18b20: interface is GPIO.
ds18b20: driver version is 2.0.
ds18b20: min supply voltage is 3.0V.
ds18b20: max supply voltage is 5.5V.
ds18b20: max current is 4.00mA.
ds18b20: max temperature is 125.0C.
ds18b20: min temperature is -55.0C.
ds18b20: start register test.
ds18b20: ds18b20_set_mode/ds18b20_get_mode test.
ds18b20: set skip rom mode.
ds18b20: check mode ok.
ds18b20: set match rom mode.
ds18b20: check mode ok.
ds18b20: ds18b20_set_rom/ds18b20_get_rom test.
ds18b20: set rom.
ds18b20: check rom ok.
ds18b20: ds18b20_get_power_mode test.
ds18b20: check power mode externally power.
ds18b20: ds18b20_scrachpad_set_resolution/ds18b20_scrachpad_get_resolution test.
ds18b20: scrachpad set resolution 9bit.
ds18b20: check resolution ok.
ds18b20: scrachpad set resolution 10bit.
ds18b20: check resolution ok.
ds18b20: scrachpad set resolution 11bit.
ds18b20: check resolution ok.
ds18b20: scrachpad set resolution 12bit.
ds18b20: check resolution ok.
ds18b20: ds18b20_scrachpad_set_alarm_threshold/ds18b20_scrachpad_get_alarm_threshold test.
ds18b20: scrachpad set alarm high threshold 48.
ds18b20: scrachpad set alarm low threshold -75.
ds18b20: check alarm high threshold ok.
ds18b20: check alarm low threshold ok.
ds18b20: ds18b20_copy_scratchpad_to_eeprom/ds18b20_copy_eeprom_to_scratchpad test.
ds18b20: copy scratchpad to eeprom.
ds18b20: check scratchpad eeprom ok.
ds18b20: ds18b20_alarm_convert_to_register/ds18b20_alarm_convert_to_data test.
ds18b20: set alarm convert to register 18.41.
ds18b20: get convert to data 18.00.
ds18b20: finish register test.
```

```shell
ds18b20 -t search

ds18b20: chip is Maxim Integrated DS18B20.
ds18b20: manufacturer is Maxim Integrated.
ds18b20: interface is GPIO.
ds18b20: driver version is 2.0.
ds18b20: min supply voltage is 3.0V.
ds18b20: max supply voltage is 5.5V.
ds18b20: max current is 4.00mA.
ds18b20: max temperature is 125.0C.
ds18b20: min temperature is -55.0C.
ds18b20: start search test.
ds18b20: search rom...
ds18b20: find 1 rom.
ds18b20: rom 28565FAF33190171.
ds18b20: search alarm rom...
ds18b20: find 0 alarm rom.
ds18b20: finish search test.
```

```shell
ds18b20 -t read --times=3

ds18b20: chip is Maxim Integrated DS18B20.
ds18b20: manufacturer is Maxim Integrated.
ds18b20: interface is GPIO.
ds18b20: driver version is 2.0.
ds18b20: min supply voltage is 3.0V.
ds18b20: max supply voltage is 5.5V.
ds18b20: max current is 4.00mA.
ds18b20: max temperature is 125.0C.
ds18b20: min temperature is -55.0C.
ds18b20: start basic read test.
ds18b20: set skip rom.
ds18b20: scrachpad set resolution 9bit.
ds18b20: temperature: 30.0C.
ds18b20: temperature: 30.0C.
ds18b20: temperature: 30.0C.
ds18b20: scrachpad set resolution 10bit.
ds18b20: temperature: 30.0C.
ds18b20: temperature: 30.0C.
ds18b20: temperature: 30.0C.
ds18b20: scrachpad set resolution 11bit.
ds18b20: temperature: 29.8C.
ds18b20: temperature: 29.8C.
ds18b20: temperature: 29.8C.
ds18b20: scrachpad set resolution 12bit.
ds18b20: temperature: 29.7C.
ds18b20: temperature: 29.7C.
ds18b20: temperature: 29.6C.
ds18b20: set match rom.
ds18b20: scrachpad set resolution 9bit.
ds18b20: temperature: 30.0C.
ds18b20: temperature: 30.0C.
ds18b20: temperature: 30.0C.
ds18b20: scrachpad set resolution 10bit.
ds18b20: temperature: 30.0C.
ds18b20: temperature: 30.0C.
ds18b20: temperature: 29.8C.
ds18b20: scrachpad set resolution 11bit.
ds18b20: temperature: 29.6C.
ds18b20: temperature: 29.6C.
ds18b20: temperature: 29.6C.
ds18b20: scrachpad set resolution 12bit.
ds18b20: temperature: 29.6C.
ds18b20: temperature: 29.6C.
ds18b20: temperature: 29.6C.
ds18b20: finish read test.
```

```shell
ds18b20 -e search

ds18b20: find 1 rom(s).
ds18b20: 1/1 is 28565FAF33190171.
```

```shell
ds18b20 -e read --times=3

ds18b20: 1/3.
ds18b20: temperature is 30.00C.
ds18b20: 2/3.
ds18b20: temperature is 30.00C.
ds18b20: 3/3.
ds18b20: temperature is 29.94C.
```

```shell
ds18b20 -e match --times=3 --rom=28565FAF33190171

ds18b20: 1/3.
ds18b20: temperature is 29.50C.
ds18b20: 2/3.
ds18b20: temperature is 29.50C.
ds18b20: 3/3.
ds18b20: temperature is 29.50C.
```

```shell
ds18b20 -e alarm-set --low=20.0 --high=30.0 --rom=28565FAF33190171

ds18b20: set alarm low threshold 20.00.
ds18b20: set alarm high threshold 30.00.
```

```shell
ds18b20 -e alarm-get --rom=28565FAF33190171

ds18b20: alarm low threshold is 20.00.
ds18b20: alarm high threshold is 30.00.
```

```shell
ds18b20 -e alarm-search 

ds18b20: find 0 alarm rom(s).
```

```shell
ds18b20 -h

Usage:
  ds18b20 (-i | --information)
  ds18b20 (-h | --help)
  ds18b20 (-p | --port)
  ds18b20 (-t reg | --test=reg)
  ds18b20 (-t read | --test=read) [--times=<num>]
  ds18b20 (-t search | --test=search)
  ds18b20 (-e read | --example=read) [--times=<num>]
  ds18b20 (-e match | --example=match) --rom=<code> [--times=<num>]
  ds18b20 (-e search | --example=search)
  ds18b20 (-e alarm-set | --example=alarm-set) --low-threshold=<low> --high-threshold=<high> --rom=<code>
  ds18b20 (-e alarm-get | --example=alarm-get) --rom=<code>
  ds18b20 (-e alarm-search | --example=alarm-search)

Options:
  -e <read | match | search | alarm-set | alarm-get | alarm-search>, --example=<read | match | search | alarm-set | alarm-get | alarm-search>
                                 Run the driver example.
  -h, --help                     Show the help.
      --high-threshold=<high>    Set the temperature interrupt high threshold.
  -i, --information              Show the chip information.
      --low-threshold=<low>      Set the temperature interrupt low threshold.
  -p, --port                     Display the pin connections of the current board.
      --rom=<code>               Set the rom with the length of 8 and it is hexadecimal.
  -t <reg | read | search>, --test=<reg | read | search>
                                 Run the driver test.
      --times=<num>              Set the running times.([default: 3])
```

