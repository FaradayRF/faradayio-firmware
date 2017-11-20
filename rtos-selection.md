#RTOS Selection

An RTOS or simple scheduler is desired to provide a layer of abstraction and program threading within the Faraday hardware. The CC430F6137 is quite minimal and many larger OS's cannout but used. The selection criteria is listed below:

* Adequate community support and documentation
* ROM and RAM needed to operate must be reasonable within the 4kb RAM nd 32Kb ROM
* An "open source" license
  * GPL, BSD, etc...

## RTOS's Available

* [RIOT - RTOS](https://riot-os.org/)
  * LGPL license
  * A very minimal, popular, supported RTOS for the internet of things
  * Has been [used on CC430F6137](https://github.com/RIOT-OS/RIOT/wiki/Board:-eZ430-Chronos)
* [TI-RTOS](http://www.ti.com/tool/TI-RTOS)
  * BSD license
  * TI supported but fairly large
* [TinyOS](http://www.tinyos.net/)
  * BSD license
  * Small but still significant portion of CC430F6137
  * Prior use on MSP430 devices but not sure about CC430F6137
* [Freertos](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&cad=rja&uact=8&ved=0ahUKEwjPytKIvM3XAhWKg1QKHUf3DVEQFggoMAA&url=http%3A%2F%2Fwww.freertos.org%2F&usg=AOvVaw3zB3DXX1lQcsp2QaeRMeva)
  * FreeRTOS Open Source License?
  * Use on MSP430 line devices
  * Well supported but fairly large?
