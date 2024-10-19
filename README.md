# Under Water Vehicle 
Our team is developing an advanced underwater vehicle designed for operation inshallow water environments, with a depth range of 2-5 meters. The robot is equippedwith six thrusters, enabling maneuverability with 5 Degrees of Freedom (DOF).

![image](https://github.com/user-attachments/assets/d058e469-873a-4238-8e82-6f5734f3a49f)

### Primary Functions:
1. **Multidirectional Movement**: The robot can move in various directions:
○ Up and down (vertical movement)
○ Left and right (lateral movement)
○ Forward and backward (longitudinal movement)
○ Yaw (rotation around the vertical axis)
○ Roll (rotation around the longitudinal axis)
2. **Extended Underwater Operation**: The vehicle is capable of staying submergedfor more than 5 minutes, allowing for prolonged underwater tasks andexploration.
3. **Underwater Mapping**: Equipped with sensors and imaging capabilities, the robotcan perform underwater mapping, providing valuable data about the submergedenvironment.

### The System-Level Wiring Diagram of the Controll System
![Under_Water_Vehicle 1](https://github.com/user-attachments/assets/aa94af0c-facf-407e-80cf-7571781d4e8f)

The proposed underwater vehicle system consists of two primary components: asurface-level floating station and a submersible vehicle. The floating station serves asthe communication interface, receiving radio frequency (RF) signals via an RC receiver.These signals are then processed as follows:
1. The RC receiver captures SBUS signals from the radio transmitter.
2. An ESP32 microcontroller, designated as the transmitter-side unit, receives anddecodes the SBUS signals, converting them into Pulse Width Modulation (PWM)signals.
3. The decoded PWM signals are transmitted via a CAT6 Ethernet cable to asecond ESP32 microcontroller, designated as the receiver-side unit, locatedwithin the submersible vehicle.
4. The receiver-side ESP32 interprets the incoming PWM signals and distributesthem to the appropriate Bidirectional Electronic Speed Controllers (ESCs).
5. The Bidirectional ESCs regulate the rotation of the underwater thrusters, therebycontrolling the vehicle's movement and orientation.




