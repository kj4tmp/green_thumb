#include <Arduino.h>
#include <Servo.h>

/* 
D2 -> SW1
D3 -> MTR_ENBL
D4 -> MTR_LATCH
D5 -> MTR_SCLK
D6 -> MTR_SDATIN
D7 -> SERVO1
D8 -> SERVO2
D9 -> SERVO3
D10 -> SERVO4
D11 -> MOTOR_FOR
D12 -> MOTOR_BACK

A4 -> LIGHT
A2 -> SW2
A1 -> BAT_SENSE
A0 -> SOIL_SIG
D13 -> SSR1

Motor Phases connected to shift registers:
A1 -> Bit 0
B1 -> Bit 1
C1 -> Bit 2
D1 -> But 3
A2 -> Bit 4
B2 -> Bit 5
C2 -> Bit 6
D2 -> Bit 7
A3 -> Bit 8
B3 -> Bit 9
C3 -> Bit 10
D3 -> Bit 11
NC -> Bit 12
NC -> Bit 13
NC -> Bit 14
NC -> Bit 15
 */

uint8_t pEnable = 3;
uint8_t pData = 6;
uint8_t pCLK = 5;
uint8_t pLatch = 4;
uint8_t MOTOR_FOR = 11;
uint8_t MOTOR_BACK = 12;
uint8_t LIGHT = A4;
uint8_t SSR1 = 13;
uint8_t SOIL = A0;
uint8_t SERVO1 = 7;
uint8_t SERVO2 = 8;
uint8_t SERVO3 = 9;
uint8_t SERVO4 = 10;



uint16_t pA1 = 0b1000000000000000;
uint16_t pB1 = 0b0100000000000000;
uint16_t pC1 = 0b0010000000000000;
uint16_t pD1 = 0b0001000000000000;
uint16_t pOff = 0b0000000000000000;
uint16_t pOn = 0b1111111111111111;

Servo servo;

void phaseOut(uint16_t phases);
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, byte val);

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pEnable, OUTPUT);
  pinMode(pData, OUTPUT);
  pinMode(pCLK, OUTPUT);
  pinMode(pLatch, OUTPUT);

  servo.attach(SERVO4);
  servo.write(0);
  
}

// the loop function runs over and over again forever
void loop() {
  // Status LED Blink
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(500);                       
  digitalWrite(LED_BUILTIN, LOW);    
  delay(500);


  //digitalWrite(LED_BUILTIN, HIGH);
  //delay(50);
  digitalWrite(pEnable, HIGH);
  delayMicroseconds(20);

  phaseOut(pOn);
  //delay(50);
  //phaseOut(pOn);
  delay(2000);
  phaseOut(pOff);
  //delay(50);
  //phaseOut(pOff);
  delay(2000);

  for (int i = 0; i <= 90; i++) {
    servo.write(i);
    delay(20);
  }
  for (int i = 90; i >= 0; i--) {
    servo.write(i);
    delay(20);
  }


  //for (int i = 0; i <= analogRead(SOIL)/100; i++) {
    //digitalWrite(LED_BUILTIN, HIGH);   
    //delay(200);                       
    //digitalWrite(LED_BUILTIN, LOW);    
    //delay(200);
  //}

}

void phaseOut(uint16_t phases){
  digitalWrite(pLatch, LOW);
  delayMicroseconds(20);
  shiftOut(pData, pCLK,LSBFIRST, lowByte(phases));
  shiftOut(pData, pCLK,LSBFIRST, highByte(phases));
  delayMicroseconds(20);
  digitalWrite(pLatch, HIGH);
  delayMicroseconds(20);
  digitalWrite(pLatch, LOW);
  delayMicroseconds(20);
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, byte val)
{
      int i;

      for (i = 0; i < 8; i++)  {
            if (bitOrder == LSBFIRST)
                  digitalWrite(dataPin, !!(val & (1 << i)));
            else      
                  digitalWrite(dataPin, !!(val & (1 << (7 - i))));
                  
            digitalWrite(clockPin, HIGH);
            delayMicroseconds(20);
            digitalWrite(clockPin, LOW);
            delayMicroseconds(20);
      }
}

