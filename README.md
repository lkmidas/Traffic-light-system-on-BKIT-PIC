# Traffic light system on BKIT PIC
This is my project for HCMUT Computer Engineering course CO2021 - *Logic Design Project*: **Simulating a 4-way intersection traffic light system on the BKIT PIC development board**.
## Introduction
Traffic light system is a common application of an embedded system. In this project, I will simulating such a system using the BKIT PIC development board, which is based on the PIC18F4620 microcontroller and developed by BKIT Hardware Club.
## Features
- The system that can work in 3 different mode: automatic countdown, manual, and standby.
- In automatic countdown mode, the lights will change its state when the countdown reaches zero.
- In manual mode, the lights' state can only be changed manually using the `B` button.
- Switch between these 2 modes using button `A`.
- In standby mode, only yellow lights will blink, switch to this mode using button `#`.
- Next 2 features are about the countdown timer. A special condition feature is simulated using 2 buttons `C` and `D` which stands for "entering special condition" and "leaving special condition", which will change the timer accordingly using 2 sets of predefined timers. In the real world, special conditions can be something like rush hour, or the state of the traffic.
- In addition to those 2 predefined timers, you can set it manually using the `*` and the number buttons.
- The last feature is force switching: the lights can go immediately from red to green and green to red using button `0`. This is to simulate the case when there is a prioritized vehicle like an ambulance or a fire truck approaching the intersection. In the real application, the button can be replaced by something like a camera or a sensor.
- The output signals are simulated on a traffic light demonstration board, the design of that board on Altium Designer can be found in this repository.
## Program structure
The program uses 4 modules: lcd, button, interrupt and timer. Its structure is shown in the diagram below:


![structure](https://github.com/LKMDang/Traffic-light-system-on-BKIT-PIC/blob/master/img/structure.png?raw=true)
## Implementation
The system is designed using a state diagram based design with 14 states in total. The state diagram is shown below:


![structure](https://github.com/LKMDang/Traffic-light-system-on-BKIT-PIC/blob/master/img/states.png?raw=true)
## Demonstration
The video where I demonstrate the product in details can be accessed at this link: [https://youtu.be/UaOuD4EGTHY](https://youtu.be/UaOuD4EGTHY)
## Contributors
DangLKM: [https://github.com/LKMDang](https://github.com/LKMDang)
