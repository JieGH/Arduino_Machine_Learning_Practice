# Arduino_Machine_Learning_Practice
Arduino Machine Learning Practice with linear regression

This program allow user to train the system to calculate the relationship between distance between your hand and the LDR. After traiining the system, the arduino will predict the distance and turn on/off the led in certain pattern. 

The pattern are: 
If the hand is more than (e.g.) 1o cm away, both LEDs should be off. 
As the hand approaches the green LED intensity should increase; every 1cm movement towards the 
light sensor should increase the green LED intensity by 20%. 
When the hand is within (e.g.) 5 cm distance, the red LED will also start to glow. Every 1 cm movement 
should increase the red LED intensity by 30%. 
When the hand is positioned very near the sensor, e.g. 1 or 2cm both LEDs should glow at full intensity. 
use appropriate time delays in each iteration of your loop, to ensure suitably responsive solution (e.g. 
not too sensitive, not too insensitive).

The schematic of this project is stated below:


![overview](https://github.com/JieGH/Arduino_Machine_Learning_Practice/blob/master/sc.PNG)


The connection of this hardware set up is clearly shown in breadboard and schematic. The LDR (R4) will convert light intensity to resistance then convert it to voltage, then input this voltage to Analogue pin A1 on the Arduino to let Arduino read the voltage level of the current light intensity.  

The two 5mm LEDs (LED1 and LED2) from university lab connect to D10 and D9 pin on the Arduino which allow it to control the LED by PWM. 
A button had used in this project which is KEY1 which is a system input to allow the changing state of FSM and notify the Arduino when to remember the training data, the detail about this button please see code. 

The resisters R1 R2 is allow enough current go through LEDs. The resister R3 is to remove floating voltage of pin D7 to allow the digital input signal from button became binary which either 5V or 0V. The R5 is protection resister for LDR which divide some of voltage from LDR, the voltage ADC value read from A1 actually is the value of R5.  


Instructions of How to use this set up 
 	Follow the serial print out of Arduino  
1.	Press 15 times to measure the light intensity in 1cm to 15 cm while moving your hand, the Serial print out will display some sentence, read please.  
2.	Press again to let Arduino do the calculation  
3.	Press again, moving your hand above LDR to see serial print out and pay attention to LEDs. 

The results of this program can be shown as :
![overview](https://github.com/JieGH/Arduino_Machine_Learning_Practice/blob/master/demo1.PNG)
![overview](https://github.com/JieGH/Arduino_Machine_Learning_Practice/blob/master/demo2.PNG)
