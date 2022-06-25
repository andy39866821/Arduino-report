# Technical Report for Arduino

In this report, I will introduce three kinds of modules, including spec and usage that I used in my Arduino final project: **LED snake**.

## I2C master & slave

### Introduction

Inter-Integrated Circuit (I2C) is a two-wire protocol that enable multiple masters and multiple slaves communicate on two one-bit wires. This kind of protocol was propsed by Koninklijke Philips N.V. in 1982, which is designed for internal control chip in the system they manufactured.
For the "two-wire", I2C only need two one-bit wire when communication, please refer to figure 1, this protocal includes two signals, SCL (serial clock) and SDA (serial data). SCL is a clock for synchronization, SDA is data wire for data transaction.
![I2C architecture](https://i.imgur.com/h1tmGxz.jpg)
Figure1. I2C architecture

### Specification

A transaction is synchronized by SCL, it can be provided by any master or slave. At the begining (**state S**), SDA should pull-down by one cycle whil SCL is high to start the trasaction. At the data trasaction phase (**state B**), Data should be prepare when SCL is high. At the finish phase (**state P**), SDA should be pull-up while SCL stay high.
![Waveform of I2C communication](https://i.imgur.com/FX6ZQ3E.jpg)
Figure2. Waveform of I2C communication

### Usage

I2C transaction is provided in Arduino library, here introduce some common functions and usage.

* Board connection
Unlike UART, there is **no need** to connect SDA and SCL inversly.
![Two board connection](https://i.imgur.com/tNMl2nL.png)
Figure 3. Two board connection (source: Fritzing)

* Master
In the setup(), we use Wire.begin() to inialize I2C configuration in Arduino board. In the loop(), we first use **Wire.beginTransmission(7);** to find the slave address 7, then send a 5-word string **"test\n"** and use **Wire.endTransmission();** to finish the transaction.

```c++
    #include <Wire.h> 

    void setup(){ 
        Wire.begin(); 
    } 

    void loop(){ 
        Wire.beginTransmission(7);
        Wire.write("test\n"); 
        Wire.endTransmission(); 
        delay(100)
    } 

```

* Slave
    Just like master part, but this time we need to register our slave address in **setup()** by **Wire.begin(7)**, where 7 is the address that we target in master part. Then we register **recieveevent()** by **Wire.onReceive(receiveEvent);**, which will be triggered whenever slave recieve data from I2C bus. In  **recieveevent()**, we use **Wire.available()** to check whether data is available on I2C bus, and read them if valid.

```c++

    void setup(){
        Wire.begin(7);
        Wire.onReceive(receiveEvent);
        Serial.begin(9600);
    }

    void loop(){
        delay(100);
    }
    
    void receiveEvent(int numBytes){
        while(Wire.available()){ 
        char ch = Wire.read();
        Serial.print(ch);
    }
}
```

## 8×8 LED Matrix

### Introduction

8x8 LED matrix can be controled by 8 row pins and eight column pins. The reason for why use 8+8 pins but not 64 pins to control 64 LED nodes is that human eyes can not tell the picture has over 60 frames per second (FPS). By the reason above, we can light-up and down very quickly to cheat our eye that all 64 LEDs are lighting at same time.  
![8*8 LED matrix](https://i.imgur.com/KxTDObs.jpg)
Figure 4. 8*8 LED matrix

### Specification

Figure 5 shows the diagram of 8x8 LED matrix, we have 8 row pins to activate rows, and 8 column pins tp activate columns. For example, if we activate row 3 and  activate column 5, the node (3, 5) will light up.
![ 8x8 LED matrix block diagram](https://i.imgur.com/Q9SIkfR.png)
Figure 5. 8x8 LED matrix block diagram

### Usage

First, we connect 16 pins to our Arduino board loke figure 6.
![](https://i.imgur.com/BzJgmAZ.jpg)
Figure 6. Arduino board connection
We use a 8x8 2D array **map** to denote the state of LED state. Since human eye can not tell the what they seen is picture switching or not over 60FPS, we need to update the entire map every 1 sec / 60 = 16 ms. Thus we light up and down a LED one by one between 100us in the for loop. The total update latency will be 100usx64=6.4 ms, which is fast enough to cheat our eyes.

``` c++
const int ROW_PIN[8] = {3, 6, 13, 7, 11, A2, A1, 8};
const int COL_PIN[8] = {12, 5, 4, 9, 2, 10, A0, A3};
bool map[8][8];

void loop(){
    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            if(led_map[i][j]==1){
                digitalWrite(ROW_PIN[i], HIGH);
                digitalWrite(COL_PIN[j], LOW);
                _delay_us(100);
                digitalWrite(ROW_PIN[i], LOW);
                digitalWrite(COL_PIN[j], HIGH);
            }
        }
    }

```

## Joystick

### Introduction

Joystick is a simple module that provide user to send two dimension signals, X-axis and Y-axis, and push-down signal.

![](https://i.imgur.com/eIeT2JL.png)
Figure 6. Joystick

### Specification

As figure 6 shows, there are five pins: GND, +5v(VDD), VRx, VRy, and SW.
GND and +5V are for power connection, VRx and VRy are analog signals that output the Amplitude of joystick swing, value is between 0 to 1023 in ATmega328p, which analog pins are 10 bits.

![](https://i.imgur.com/K0q2xkD.png)

### Usage

In the function **get_roll_direction()**, we first read two axis value from analog ports. We set the threshold of 100 and 900, where the input value is between 0 to 1023. In the main if-else part, we identify which direction is the joystick moving by the threshold above.

``` c++

    #define UP 0
    #define DOWN 1
    #define LEFT 2
    #define RIGHT 3
    int move_direction = LEFT;

    void get_roll_direction(){

        int x = analogRead(ROLL_X_PIN);
        int y = analogRead(ROLL_Y_PIN);

        if(x > 900){
            move_direction = DOWN; 
        }
        else if(x < 100){
            move_direction = UP;
        }
        else if(y > 900){
            move_direction = LEFT;
        }
        else if(y < 100){
            move_direction = RIGHT;
        }
    }

```
