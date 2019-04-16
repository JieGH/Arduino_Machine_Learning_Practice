// Arduino, linear regression, LEDs
const int buttonPin = 7;    // the number of the pushbutton pin
int buttonState = 0;    // variable for reading the pushbutton status
int LDRvalue = 0;   // The ADC value for the LDR which read from ping A1,
// Attention !! this value actually is the ADC value of R5 see schematic for detail,
int LDRpin = A1;    // analogue pin A1 to read ADC value from LDR
int GREENled = 9;   // the PWM pin to control green led
int REDled = 10;    // the PWM pin to control red led
int brightness_red = 0; // how bright the LED is
int brightness_green = 0;// how bright the LED is
int percent20 = 51; // change LED intensity by 20% is change 51 out of 255,
int percent30 = 76; // 255 is the Decimal Number of 1111 1111 of 8 bit PWM output from D9 and D10, same wil 30%
double distance = 0; // the predicted distance after tranining
unsigned char state, nxtstate; // state and next state for FSM
int x[15] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; // the set distance from 1 to 5 to train the system
int y[15]; //y array store the ADC value from LDR, this array will be used in calculation of linear regression line
int cont = 0; // cont for let system log the training value, search cont in code to see detail

int mean_of_x; // pre-result of calculating coefficient for linear regression line
int mean_of_y;  // same
int x_x_mean[15];   // same
int y_y_mean[15];   // same
double b1 = 0;  // coefficient of linear regression line
double b0 = 0;  // which is y = b0 + b1 * x

class Get_b1_b0 { // the class for calculate coefficient b1 and b0
private:
    int sum;
    int mean;
    int i;
    int j;
    int b1_down;
    int l;
    int b1_up;
    int o;  // all the private variable during the calculation
public: // all the public function for calculation, prototype declearition
    int get_arrya_mean(int array[]); // to get the mean value for x amd y
    void get_x_x_mean(int array[]); // to get x - x_mean
    void get_y_y_mean(int array[]); // to get y - y_mean
    int get_x_xmean_X_y_ymean();    // to get sum of (x - x_mean[]) * (y - y_mean[])
    int get_square_x_xmean_sum();   // to get sum of (x - x_mean[])^2
};

int Get_b1_b0::get_arrya_mean(int array[]) { // see line 37-42
    sum = 0;    // pre-result of mean calculation
    mean = 0; // pre-result of mean calculation
    for (int i = 0; i < 15; ++i) { //get sum of the array
        sum = sum + array[i];
    }
    mean = sum / 15; // get mean
    return mean; // return the mean value
}

void Get_b1_b0::get_x_x_mean(int array[]) {// see line 37-42
    for (i = 0; i < 15; ++i) {
        x_x_mean[i] = array[i] - mean_of_x; // simple math
    }
}

void Get_b1_b0::get_y_y_mean(int array[]) {// see line 37-42
    for (j = 0; j < 15; ++j) {
        y_y_mean[j] = array[j] - mean_of_y; // simple math
    }
}

int Get_b1_b0::get_x_xmean_X_y_ymean() {// see line 37-42
    b1_down = 0;
    for (l = 0; l < 15; ++l) {
        b1_down = x_x_mean[l] * y_y_mean[l] + b1_down; // to get b1_down  >> b1 = b1_up / b1_down
    }
    return b1_down;
}

int Get_b1_b0::get_square_x_xmean_sum() {// see line 37-42
    b1_up = 0;
    for (o = 0; o < 15; ++o) {
        b1_up = x_x_mean[o] * x_x_mean[o] + b1_up; // to get b1_up  >> b1 = b1_up / b1_down
    }
    return b1_up;
}

void state0(void) { // see report for FSM digram // state0 to train the system
    buttonState = digitalRead(buttonPin); // read the digital input from
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
        digitalWrite(LED_BUILTIN, HIGH); // little indication
        Serial.println("Collecting calebration data...."); //if button pressed print this to show data is collecting
        readLDR(); // to get ADC value for LDR,
        delay(600); // human is slow, allow human to release the button, otherwise machine thought you pressed many times
        cont++; // when pressed cont incremnet
        Serial.print("The light intendisty in "); // show which value you collect corronspodend with the distance
        Serial.print(cont);
        Serial.print(" cm\n");
        y[cont - 1] = LDRvalue; // store this ADC value to array for calculation in state 1
      // Attention !! this value actually is the ADC value of R5, see schematic for detail,
        if (cont == 15) {   // 15 data need be collect to train the system which from 1cm to 15 cm
            Serial.println("\nFinished data Collection, the data collected are:");
            for (int i = 0; i < 15; i++) {  // show all the data the machine collected
                Serial.print("\nThe light intendisty in ");
                Serial.print(i + 1);
                Serial.print(" cm is ");
                Serial.print(y[i]);
                cont = 0;   // reset cont, get ready for next training
            }
            Serial.println("\nPress again to continue to next step: calculate regression line.");
            nxtstate = 1; // press again to change state
        }
    } else
        nxtstate = 0; // wait human to finish training the system otherwise still wait
}//state0

void state1(void) { // to calculate b0 and b1
    buttonState = digitalRead(buttonPin); // read button press
    if (buttonState == HIGH) { // if pressed, do calculation
        Get_b1_b0 xy; // use class Get_b1_b0, declair it as xy
        mean_of_x = xy.get_arrya_mean(x); // assign the returned value, detail of this function see class definition
        mean_of_y = xy.get_arrya_mean(y);// same above

        xy.get_x_x_mean(x); // same with above calling member function of class to do math
        xy.get_y_y_mean(y); // same
        // calculate b1, make sure the division result is not interger
        b1 = xy.get_x_xmean_X_y_ymean() / (double) xy.get_square_x_xmean_sum();
        b0 = (double) mean_of_y - b1 * mean_of_x; // to get b0
        Serial.println("\nAfter calculation, the regression line is:");
        Serial.print(" y = ");// display the linnear regression function the machine calculated
        Serial.print(b0);
        Serial.print(" + ");
        Serial.print(b1);
        Serial.print(" * x");
        Serial.print("\t");
        Serial.println("\nPress again to continue to next step");

        nxtstate = 2; // press to go to next state which will predict distance and turn on LEDs
        delay(600); // give human a break
    } else
        nxtstate = 1;
}//state1

void state2(void) {
    LDRvalue = analogRead(LDRpin); // read LDR value  Attention !! this value actually is the ADC value of R5,
    distance = (LDRvalue - b0) / b1;
    Serial.print("The pridicted distance is:");
    Serial.print(distance);
    Serial.println(" cm"); // print the real time distance pridiction
    // based on the requirements turn on the LED on different brightness
    if (distance > 10) {
        brightness_green = 0;
        brightness_red = 0;
    } else if (distance > 9 & distance < 10) {
        brightness_green = 51;
        brightness_red = 0;
    } else if (distance > 8 & distance < 9) {
        brightness_green = 51 * 2;
        brightness_red = 0;
    } else if (distance > 7 & distance < 8) {
        brightness_green = 51 * 3;
        brightness_red = 0;
    } else if (distance > 6 & distance < 7) {
        brightness_green = 51 * 4;
        brightness_red = 0;
    } else if (distance > 5 & distance < 6) {
        brightness_green = 51 * 5;
        brightness_red = 0;
    } else if (distance > 4 & distance < 5) {
        brightness_green = 51 * 5;
        brightness_red = 76;
    } else if (distance > 3 & distance < 4) {
        brightness_green = 51 * 5;
        brightness_red = 76 * 2;
    } else if (distance > 2 & distance < 3) {
        brightness_green = 51 * 5;
        brightness_red = 76 * 3;
    } else if (distance < 2) {
        brightness_green = 255;
        brightness_red = 255;
    }

    analogWrite(GREENled, brightness_green); // use PWM to control the brightness of LEDs
    analogWrite(REDled, brightness_red);

    delay(500); // this delay not even gave human eyes a break when reading the print out
    // but also construct a long press function
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
        // turn LED on:
        Serial.println("Restart from beginning"); // go back to state 0 to re-train the system
        nxtstate = 0;
        delay(600);
    } else
        nxtstate = 2; // you can play the distance pridiction function as long as you want
}//state2

void setup() {
    // put your setup code here, to run once:
    pinMode(buttonPin, INPUT); // setup build in led as output
    Serial.begin(9600); // set baud rate for serial print out
    Serial.println("Come on in! start train the systm, hold your hand above LDR, moving away your hand from LDR by 1cm in every press, start from 1cm. ");
    pinMode(GREENled, OUTPUT); // set up PWM digital output pin for controlling LEDs
    pinMode(REDled, OUTPUT);
}

void readLDR() {
    LDRvalue = analogRead(LDRpin); // the ADC value read actually is ADC value for R5, see schematic for detail
}

void loop() {
    // main program contain a FSM
    state = 0;  // initial  state 0 or reset funcntion
    nxtstate = 0;
    while (1) { // explain the FSM model, state jump from 0 to 1 to 2 back to 0
        switch (state) {
            case 0: {
                state0();   // training state, to log the light intensity
                state = nxtstate;
                break;
            }
            case 1: {
                state1();   // calculation state, to get coefficient for linear regression
                state = nxtstate;
                break;
            }
            case 2: {
                state2();   // prediction state, to control LED based on the light and predict the distance.
                state = nxtstate;
                break;
            }
        }//switch
    }//while
}

