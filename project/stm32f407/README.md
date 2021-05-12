### 1. chip

#### 1.1 chip info

chip name : STM32F407ZGT6.

extern oscillator : 8MHz.

uart pin: TX/RX PA9/PA10.

data pin: PA8.

### 2. shell

#### 2.1 shell parameter

baud rate: 115200

data bits : 8

stop bits: 1

parity: none

flow control: none

### 3. ds18b20

#### 3.1 command Instruction

​          ds18b20 is a basic command which can test all ds18b20 driver function:

​          -i        show ds18b20 chip and driver information.

​          -h       show ds18b20 help.

​          -p       show ds18b20 pin connections of the current board.

​          -t (reg | read <times>| search)

​          -t reg        run ds18b20 register test.

​          -t read <times>        run ds18b20 read test. times means the test times.

​          -t search        run ds18b20 search test.

​          -c (read <times>| match <times> -rom <r1> <r2> <r3> <r4> <r5> <r6> <r7> <r8>| search |alarm (set <lowthreshold> <highthreshold> -rom <r1> <r2> <r3> <r4> <r5> <r6> <r7> <r8> | get -rom  <r1> <r2> <r3> <r4> <r5> <r6> <r7> <r8> | search))

​          -c read <times>        run ds18b20 read function. times means the read times.

​          -c match <times> -rom <r1> <r2> <r3> <r4> <r5> <r6> <r7> <r8>        run ds18b20 match function. times means the read times. <r1> - <r8> mean the rom id and they are hexadecimal.

​          -c search        run ds18b20 search rom function.

​          -c alarm set <lowthreshold> <highthreshold> -rom <r1> <r2> <r3> <r4> <r5> <r6> <r7> <r8>           run  alarm set threshold function. lowthreshold means the low threshold.highthreshold means the high threshold. <r1> - <r8> mean the rom id and they are hexadecimal.

​          -c alarm get  -rom  <r1> <r2> <r3> <r4> <r5> <r6> <r7> <r8>        run  alarm get threshold function. <r1> - <r8> mean the rom id and they are hexadecimal.

​          -c alarm search        run ds18b20 search alarm rom function.

#### 3.2 command example

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
ds18b20: scrachpad set alarm high threshold 104.
ds18b20: scrachpad set alarm low threshold -7.
ds18b20: check alarm high threshold ok.
ds18b20: check alarm low threshold ok.
ds18b20: ds18b20_copy_scratchpad_to_eeprom/ds18b20_copy_eeprom_to_scratchpad test.
ds18b20: copy scratchpad to eeprom.
ds18b20: check scratchpad eeprom ok.
ds18b20: ds18b20_alarm_convert_to_register/ds18b20_alarm_convert_to_data test.
ds18b20: set alarm convert to register 20.74.
ds18b20: get convert to data 20.00.
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
ds18b20: rom 0x28 0x56 0x5F 0xAF 0x33 0x19 0x01 0x71 .
ds18b20: search alarm rom...
ds18b20: find 0 alarm rom.
ds18b20: finish search test.
```

```shell
ds18b20 -t read 3

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
ds18b20: temperature: 29.5C.
ds18b20: temperature: 29.5C.
ds18b20: temperature: 29.5C.
ds18b20: scrachpad set resolution 10bit.
ds18b20: temperature: 29.2C.
ds18b20: temperature: 29.2C.
ds18b20: temperature: 29.2C.
ds18b20: scrachpad set resolution 11bit.
ds18b20: temperature: 29.0C.
ds18b20: temperature: 29.0C.
ds18b20: temperature: 29.0C.
ds18b20: scrachpad set resolution 12bit.
ds18b20: temperature: 28.8C.
ds18b20: temperature: 28.8C.
ds18b20: temperature: 28.8C.
ds18b20: set match rom.
ds18b20: scrachpad set resolution 9bit.
ds18b20: temperature: 29.0C.
ds18b20: temperature: 29.0C.
ds18b20: temperature: 29.0C.
ds18b20: scrachpad set resolution 10bit.
ds18b20: temperature: 29.0C.
ds18b20: temperature: 29.0C.
ds18b20: temperature: 29.0C.
ds18b20: scrachpad set resolution 11bit.
ds18b20: temperature: 28.9C.
ds18b20: temperature: 28.9C.
ds18b20: temperature: 28.9C.
ds18b20: scrachpad set resolution 12bit.
ds18b20: temperature: 28.9C.
ds18b20: temperature: 28.9C.
ds18b20: temperature: 28.9C.
ds18b20: finish read test.
```

```shell
ds18b20 -c search

ds18b20: find 1 rom(s).
ds18b20: 1/1 is 28 56 5F AF 33 19 01 71 .
```

```shell
ds18b20 -c read 3

ds18b20: 1/3.
ds18b20: temperature is 27.38C.
ds18b20: 2/3.
ds18b20: temperature is 27.38C.
ds18b20: 3/3.
ds18b20: temperature is 27.45C.
```

```shell
ds18b20 -c match 3 -rom 28 56 5F AF 33 19 01 71

ds18b20: 1/3.
ds18b20: temperature is 27.25C.
ds18b20: 2/3.
ds18b20: temperature is 27.25C.
ds18b20: 3/3.
ds18b20: temperature is 27.25C.
```

```shell
ds18b20 -c alarm set 20.0 50.0 -rom 28 56 5F AF 33 19 01 71

ds18b20: set alarm low threshold 20.00.
ds18b20: set alarm high threshold 50.00.
```

```shell
ds18b20 -c alarm get -rom 28 56 5F AF 33 19 01 71

ds18b20: alarm low threshold is -7.00.
ds18b20: alarm high threshold is 104.00.
```

```shell
ds18b20 -c alarm search 

ds18b20: find 0 alarm rom(s).
```

```shell
ds18b20 -h

ds18b20 -i
	show ds18b20 chip and driver information.
ds18b20 -h
	show ds18b20 help.
ds18b20 -p
	show ds18b20 pin connections of the current board.
ds18b20 -t reg
	run ds18b20 register test.
ds18b20 -t read <times>
	run ds18b20 read test.times means the test times.
ds18b20 -t search
	run ds18b20 search test.
ds18b20 -c read <times>
	run ds18b20 read function.times means the read times.
ds18b20 -c match <times> -rom <r1> <r2> <r3> <r4> <r5> <r6> <r7> <r8>
	run ds18b20 match function.times means the read times.<r1>-<r8> mean the rom id and they are hexadecimal.
ds18b20 -c search
	run ds18b20 search rom function.
ds18b20 -c alarm set <lowthreshold> <highthreshold> -rom <r1> <r2> <r3> <r4> <r5> <r6> <r7> <r8>
	run alarm set threshold function.lowthreshold means the low threshold.highthreshold means the high threshold.<r1>-<r8> mean the rom id and they are hexadecimal.
ds18b20 -c alarm get -rom <r1> <r2> <r3> <r4> <r5> <r6> <r7> <r8>
	run alarm get threshold function.<r1>-<r8> mean the rom id and they are hexadecimal.
ds18b20 -c alarm search
	run ds18b20 search alarm rom function.
```

