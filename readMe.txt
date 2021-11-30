software architectural design:

Code Base core for various microcontrollers (Cortex M).

Code Base has been covered by unit tests.

There is a bootloader configuration.

That is the single-thread supercicle with interrupts code base firmware.

Most parameters are controlled with CLI through UART0 (460800 bit/s) and LoRa.

Lots of parameters can be stored in none volatile on chip Nor-Flash File system.

Configuration are verified in CD Jenkins.

Hardware independed part of code base has been built in X86 GCC 32bit invironment.



Links
https://blog.radiotech.kz/radio/launchxl-cc26x2r1-podgotovka-rabochego-okruzheniya-v-linux/
https://e2e.ti.com/support/wireless-connectivity/sub-1-ghz-group/sub-1-ghz/f/sub-1-ghz-forum/734222/launchxl-cc1312r1-adding-second-uart
http://microsin.net/adminstuff/others/u-blox-7-ubx-protocol-part-3.html
https://dev.to/gamegods3/how-to-install-gcc-in-windows-10-the-easier-way-422j
https://habr.com/ru/post/398229/
dBm to Watt Conversion
https://awt-global.com/wp3/resources/dbm-to-watt-conversion/
How to calculate the data rate of LoRa?
https://electronics.stackexchange.com/questions/277722/how-to-calculate-the-data-rate-of-lora

Онлайн-калькулятор теоретически достижимой дальности связи
https://r1ban.ru/calc/loss-calc-dist.htm


RTL-SDR V3 - Улучшенная версия популярного SDR приёмника
https://mysku.ru/blog/ebay/65825.html

Особенности применении на территории Российской Федерации радиоэлектронных средств, работающих в режиме IoT и в сетях транспортной телематики
https://rkn.gov.ru/communication/licensing-activity/p1064/

Обзор технологии LoRa
https://itechinfo.ru/content/%D0%BE%D0%B1%D0%B7%D0%BE%D1%80-%D1%82%D0%B5%D1%85%D0%BD%D0%BE%D0%BB%D0%BE%D0%B3%D0%B8%D0%B8-lora

Начало работы с TI CC13xx-CC26xx и Contiki под Code Composer Studio
https://habr.com/ru/company/unwds/blog/390815/