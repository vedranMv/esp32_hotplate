ESP32 Reflow Hotplate
=====================

This repository contains electronics and an accompanying firmware for ESP32-powered hotplate from reflow soldering. The full project description can be found on [hackaday](https://hackaday.io/project/202351-reflow-soldering-hotplate).

Electronics schematics can be found in `electronics/` folder, together with a kicad project file and an accompanying PCB layout.

The remainder of folders belong to a pltaformio project for compiling the firmware. 

Features overview
-----------------

The list below contains the list of features supported by this project. Checked features are already implemented, the rest is pending development:

- [x] Heat element ON/OF
- [x] Plate temperature feedback
- [x] Internal temperature feedback
- [x] Input voltage feedback
- [x] Current power consumption
- [x] "Manual mode" - heat up and maintain given temperature
- [x] Configuration storage in non-volatile memory
- [ ] Timer indicating the time spent while heating up, reset when setpoint is reached to indicate the time spent at a setpoint
- [ ] "Profile mode" - follow a reflow profile for pre-programmed solder pastes
- [ ] Option to program a reflow profile via HMI
- Safety features:
  - [ ] Error if expecting to be heating up, but no temperature change is detected
  - [ ] Error if heating up for more 15 min
  - [ ] Error if temperature is 0

Software Design
---------------

To interact with the device, there's a user interface (HMI) consisting of OLED display and a rotary encoder with push button. Using push button gestures (click, double click and long press) it's possible to perform actions like edit/confirm/start/stop/etc, while rotary encoder can be used to scroll and increment data.

To make the best use of HMI, software is designed with the concept of 'applications'. Each application implements a device mode, or a behavior. For example, a manual mode is implemented as an app that allows the user to set manually a setpoint to which the hotplate heats up. User can select an active application, there can only be one active application at the time and the active application gets exclusive access to display and input. To see device stats (e.g. voltage, power, temperatures...), it's possible to change to stats app that shows all of that.

Apps interact with low level hardware via board support, that gives it access to analog and digital inputs/outputs or a configuration interface to load and save parameters from/to non-volatile storage.

Getting Started
---------------

This being a platformio project, getting started is realtively simple. If you have platformio already installed, it's enough to just clone this repository locally and open it in platformio. From there on, it's possible to build and upload the project to the target chip.