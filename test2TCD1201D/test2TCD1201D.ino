#define SH	PORTB0	//digital pin 8
#define PHI1	PORTB1	//digital pin 9 (PWM)
#define PHI2	PORTB2	//digital pin 10 (PWM)
#define RS	PORTB3	//digital pin 11 (PWM)
#define BT	PORTB4	//digital pin 12
#define OS	A0	//A0

byte sadc[1024];  // signal output values read
int sdacCounter;

void setup() {
  Serial.begin(115200);

  pinMode(OS, INPUT);
  // set all OUTPUT
  DDRB = B00011111;
  // take all to LOW
  PORTB = B00000000;
  
  // set up the ADC
  ADCSRA &= ~(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // remove bits set by Arduino library, prescaler = 2
  
  unsigned long start = micros();
  for(int i=0; i < 10000; i++){
    PORTD = B00001000;
    PORTD = B00000000;
  }
  start = micros()-start;
}

void loop() {
  //maybe here you want a couple of read to decrease the integration time caused by the long serial write
  readCCD();

  //here we write 1024 byte @ 115200baud: this mean 89mS
  for (int i=0; i<1024;i+=1){
    Serial.write(sadc[i]);
  }
  Serial.flush();
}

void runCCD() {
  PORTB = (0 << RS) | (1 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
  PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
  
  for (byte b = 0; b < 1; b++){
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (1 << SH);
    PORTB = (1 << RS) | (0 << BT) | (0 << PHI2) | (1 << PHI1) | (1 << SH);
    PORTB = (1 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (1 << SH);
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (1 << SH);
  }
  
  for (byte b = 0; b < 1; b++){
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (1 << RS) | (0 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (1 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
  }
  
  for (int i=0; i<16+1024+14;i++){
    PORTB = (0 << RS) | (0 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
    PORTB = (1 << RS) | (0 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
    PORTB = (1 << RS) | (1 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
    PORTB = (0 << RS) | (1 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    
    PORTB = (0 << RS) | (0 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (1 << RS) | (0 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (1 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (0 << RS) | (1 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
  }
}

inline void readCCD() {
  
  noInterrupts();
  
  sdacCounter = 0;

  PORTB = (0 << RS) | (1 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
  PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
  
  for (byte b = 0; b < 1; b++){
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (1 << SH);
    PORTB = (1 << RS) | (0 << BT) | (0 << PHI2) | (1 << PHI1) | (1 << SH);
    PORTB = (1 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (1 << SH);
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (1 << SH);
  }
  
  for (byte b = 0; b < 1; b++){
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (1 << RS) | (0 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (1 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
  }
  
  //start reading
  for (int i=0; i<16;i++){
    read1Low();
    read1High();
  }

  for (int i=0; i<1024;i++){
    sdacCounter++;
    read1Low();
    read1High();
  }
  
  
  
  for (int i=0; i<7;i++){
    read1Low();
    read1High();
  }
  
  interrupts();
}

inline void clockNoClock(){
  PORTB ^= (1 << RS) | (0 << BT);
  PORTB ^= (1 << RS) | (1 << BT);
  PORTB ^= (0 << RS) | (1 << BT);
  PORTB ^= (0 << RS) | (0 << BT);
}

inline void read1Low(){
  PORTB = (0 << RS) | (0 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
  //if (sdacCounter>>1)
  sadc[sdacCounter] = analogRead(OS); //ADC is 10 bit, so keep 8 MSB
  PORTB = (1 << RS) | (0 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
  PORTB = (1 << RS) | (1 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
  //PORTB = (0 << RS) | (1 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
  //PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
  PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
}

inline void read1High(){
  PORTB = (0 << RS) | (0 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
  sadc[sdacCounter] = analogRead(OS); //ADC is 10 bit, so keep 8 MSB
  PORTB = (1 << RS) | (0 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
  PORTB = (1 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
  //PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
  //PORTB = (0 << RS) | (1 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
  PORTB = (0 << RS) | (1 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
}
