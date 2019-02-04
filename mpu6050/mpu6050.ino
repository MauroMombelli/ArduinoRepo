#include "Wire.h" // This library allows you to communicate with I2C devices.

const uint8_t MPU_ADDR = 0x68;

void write(uint8_t * const res, uint8_t len){
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  for (int i = 0; i < len; i++){
    Wire.write(res[i]);
  }
  Wire.endTransmission(true);
}

uint8_t read(uint8_t reg, uint8_t * const res, uint8_t len) {
  uint8_t value;

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU_ADDR, len, (uint8_t)1);
  for(uint8_t i = 0; i < len; i++){
    res[i] = Wire.read();
  }

  return value;
}

void setup() {
  Serial1.begin(115200);
  delay(3000);
  Wire.setSDA(PB11);
  Wire.setSCL(PB10);
  Wire.begin();

  uint8_t res;
  read(0x75, &res, 1);
  if(res == MPU_ADDR){
    Serial1.println("MPU6050 found");
  }else{
    while (1){
      Serial1.println("MPU6050");
      delay(1000);
    }
  }

  uint8_t cmd[2] = {0x6B, 0};
  write(cmd, 2);
}

void loop() {
  if (Serial.read() == 'R'){
    NVIC_SystemReset();
  }
  
  int16_t accelerometer[3];
  int16_t temperature;
  int16_t gyro[3];
  
  uint8_t raw[7*2];

  read(0x3B, raw, 7*2);

  for (uint8_t i = 0; i < 3; i++){
    accelerometer[i] = raw[0+i*2]<<8 | raw[1+i*2];
  }
  temperature = raw[6]<<8 | raw[7];
  for (uint8_t i = 0; i < 3; i++){
    gyro[i] = raw[8+i*2]<<8 | raw[9+i*2];
  }

  // print out data
  Serial1.print("aX = "); Serial1.print(accelerometer[0]);
  Serial1.print(" | aY = "); Serial1.print(accelerometer[1]);
  Serial1.print(" | aZ = "); Serial1.print(accelerometer[2]);
  
  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  Serial1.print(" | tmp = "); Serial1.print(temperature / 340.0 + 36.53);
  
  Serial1.print(" | gX = "); Serial1.print(gyro[0]);
  Serial1.print(" | gY = "); Serial1.print(gyro[1]);
  Serial1.print(" | gZ = "); Serial1.print(gyro[2]);
  Serial1.println();
  
  // delay
  delay(500);
}
