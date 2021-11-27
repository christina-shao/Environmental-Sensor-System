# Environmental-Sensor-System
Code for an Arduino-based sensor system to be implemented on a car rig for an experiment to inform the propulsion selection for an unmanned aerial system to be used in the SAE aero design competition in May 2022.

Developed for the University of Toronto Aerospace Team's SAE aero design team propulsion team.

## Code
The system uses pressure readings from a pitot tube with static and a DHS temperature and humidity sensor to supplement the data obtained from our propellor test bench. The arduino then streams the airspeed, temperature, and humidity readings to a monitor while storing them to the on-board SD card.

Two different versions of the code was developed, one for a sensor system without a dedicated differential pressure sensor and one with while adjusting to equipment availability throughout the development process. The code takes 10 readings of differential pressure from the static and pitot pressure ports on a pitot tube and returns the average airspeed using Bernoulli's principle, humidity, and temperature from the DHS sensor every 2 seconds.
