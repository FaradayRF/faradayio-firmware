# faradayio-firmware
Firmware repository for FaradayIO generic data interface.

## Overview

This repository contains the firmware for the Faraday open source digital ham radio hardware. The hardware utilizes the CC430F6137 microcontroller. FaradayIO is the software and firmware that creates the generic data functionality of Faraday, creating a simple network interface on a linux computer and firmware that provides generael hardware control and as needed network layering/configuration (layer 2 - datalink).

## Firmware Path
The original Faraday firmware was a custom coded baremetal implementation that used FIFO's, while loops, and general "heartbeat" timers to create a psuedo "computer" that would perform IO and timing functionality. The path forward is the minimize firmware and use standards where ever possible. FaradayRF is looking to implement a basic operating system on the CC430F6137 to abstract functionality while providing easy application and hardware driver developement.
