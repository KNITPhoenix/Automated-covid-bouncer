# Automated-covid-bouncer
This project aims to develop an automated electronic bouncer for a restaurant that controls patrons’ entrance to, and exit from the restaurant. The bouncer is installed at the restaurant’s entrance door, keeps track of the number of people in the restaurant, and manages a queue system for people who want to enter the restaurant.

The system is implemented using an Arduino Mega 2560 microcontroller board, two infrared (IR) distance sensors, two 4-digit 7-segment displays, a servo motor, a buzzer, wires, a battery, an enclosure, and other necessary hardware components, as well as a C program. This is just a prototype to represent the use of algo. It can be built on a large scale.

Algorithm:
1. Include the following modules: microcontroller board, two infrared (IR) distance sensors, two 4-digit 7-segment displays, a servo motor, a buzzer, wires, a battery, and an enclosure. It can include other parts. The servo motor operates a boomgate. 
2. It must operate as follows:
Maximum number of people that can be in the restaurant is set to 5.
Number of people currently in the restaurant is set to 00.
Ticket number of the person that can enter the restaurant is set to 00.
Ticket number of last person in the queue is set to 00.
Displays 1 and 2 start with 00 00 and 05 00. Displays 1 counts between 05 00 to 05 05 only. Display 2 counts between 00 00 to 99 99. We assume that no more than 99 people will come to the restaurant.
Buzzer is off.
Boomgate is closed.
When a person is detected at 30cm away from Sensor 1:
a. If number of people currently in the restaurant is less than 5, the boomgate is opened, once person is detected at 30cm away from Sensor 2, the boomgate is closed, and the right-hand-side two digits of Display 2 are incremented by one eg 05 01.
b. Otherwise, buzzer emits access denied Tone 1 for 1 sec. Then, Ticket number of last person in the queue is incremented by 1, and left-hand-side two digits of Display 1 are incremented by 1, eg 01 00.
When a person is detected at 30cm away from Sensor 2:
a. The boomgate is opened, once person is detected at 30cm away from Sensor 1, the boomgate is closed, and the right-hand-side two digits of Display 2 are decremented by one eg 05 00.
b. If anyone in the queue, buzzer emits next person to enter Tone 2 for 3 sec. The right-hand-side two digits of Display 1 are incremented by 1, eg 01 01. The person whose ticket number is that showed on the right-hand-side two digits of Display 1 come to 30cm from Sensor 1. The boomgate is opened, the person moves past the boomgate, and once they are detected at 30cm away from Sensor 2, the boomgate is closed, and the right-hand-side two digits of Display 2 is incremented by one, eg 05 01.
Assume that when one person is interacting with the Bouncer (going in, coming out, or getting a ticket), no other people come near the bouncer.
6. You can make other necessary assumptions if they have not been listed above.

