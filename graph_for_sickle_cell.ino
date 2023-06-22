// // void setup() {
// //   // put your setup code here, to run once:

// // }

// // void loop() {
// //   // put your main code here, to run repeatedly:

// // }

// #include <Wire.h>
// #include "MAX30105.h"
// #include "spo2_algorithm.h"

// MAX30105 particleSensor;

// #define MAX_BRIGHTNESS 255

// #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
// uint16_t irBuffer[100]; //infrared LED sensor data
// uint16_t redBuffer[100];  //red LED sensor data
// #else
// uint32_t irBuffer[100]; //infrared LED sensor data
// uint32_t redBuffer[100];  //red LED sensor data
// #endif

// int32_t bufferLength;
// int32_t spo2;
// int8_t validSPO2;
// int32_t heartRate;
// int8_t validHeartRate;

// byte pulseLED = 11;
// byte readLED = 13;

// void setup()
// {
//   Serial.begin(115200);

//   pinMode(pulseLED, OUTPUT);
//   pinMode(readLED, OUTPUT);

//   if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
//   {
//     Serial.println(F("MAX30105 was not found. Please check wiring/power."));
//     while (1);
//   }

//   Serial.println(F("Attach sensor to finger with rubber band. Press any key to start conversion"));
//   while (Serial.available() == 0) ;
//   Serial.read();

//   byte ledBrightness = 60;
//   byte sampleAverage = 4;
//   byte ledMode = 2;
//   byte sampleRate = 100;
//   int pulseWidth = 411;
//   int adcRange = 4096;

//   particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
// }

// void loop()
// {
//   bufferLength = 100;

//   // Read the first 100 samples and determine the signal range
//   for (byte i = 0 ; i < bufferLength ; i++)
//   {
//     while (particleSensor.available() == false)
//       particleSensor.check();

//     redBuffer[i] = particleSensor.getRed();
//     irBuffer[i] = particleSensor.getIR();
//     particleSensor.nextSample();
//   }

//   // Calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
//   maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

//   // Continuously taking samples from MAX30102. Heart rate and SpO2 are calculated every 1 second
//   while (1)
//   {
//     // Dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
//     for (byte i = 25; i < 100; i++)
//     {
//       redBuffer[i - 25] = redBuffer[i];
//       irBuffer[i - 25] = irBuffer[i];
//     }

//     // Take 25 sets of samples before calculating the heart rate
//     for (byte i = 75; i < 100; i++)
//     {
//       while (particleSensor.available() == false)
//         particleSensor.check();

//       digitalWrite(readLED, !digitalRead(readLED));

//       redBuffer[i] = particleSensor.getRed();
//       irBuffer[i] = particleSensor.getIR();
//       particleSensor.nextSample();

//       // Send samples and calculation results to the terminal program through UART
//       // Serial.print("redBuffer");
//       Serial.print(redBuffer[i]);
//       Serial.print(", ");
//       // Serial.print("IRBuffer");
//       Serial.print(irBuffer[i]);
//       Serial.print(", ");
//       // Serial.print("heartRate");
//       Serial.print(heartRate);
//       Serial.print(", ");
//       //  Serial.print("validHeartRate");
//       Serial.print(validHeartRate);
//       Serial.print(", ");
//       // Serial.print("Oxygen Saturation ");
//       Serial.print(spo2);
//       Serial.print(", ");
// // Serial.print("Oxygen Saturation");
//       Serial.println(validSPO2);
//     }

//     // After gathering 25 new samples, recalculate HR and SP02
//     maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
//   }
// }

#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

MAX30105 particleSensor;

#define MAX_BRIGHTNESS 255

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data
#else
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
#endif

int32_t bufferLength;
int32_t spo2;
int8_t validSPO2;
int32_t heartRate;
int8_t validHeartRate;

byte pulseLED = 11;
byte readLED = 13;

void setup()
{
  Serial.begin(115200);

  pinMode(pulseLED, OUTPUT);
  pinMode(readLED, OUTPUT);

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }

  Serial.println(F("Attach sensor to finger with rubber band. Press any key to start conversion"));
  while (Serial.available() == 0) ;
  Serial.read();

  byte ledBrightness = 60;
  byte sampleAverage = 4;
  byte ledMode = 2;
  byte sampleRate = 100;
  int pulseWidth = 411;
  int adcRange = 4096;

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
}

void loop()
{
  bufferLength = 100;

  // Read the first 100 samples and determine the signal range
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false)
      particleSensor.check();

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample();
  }

  // Calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

  // Continuously taking samples from MAX30102. Heart rate and SpO2 are calculated every 1 second
  while (1)
  {
    // Dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

    // Take 25 sets of samples before calculating the heart rate
    for (byte i = 75; i < 100; i++)
    {
      while (particleSensor.available() == false)
        particleSensor.check();

      digitalWrite(readLED, !digitalRead(readLED));

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample();

      // Send samples and calculation results to the terminal program through UART
      // Serial.print("redBuffer");
      Serial.print(redBuffer[i]);
      Serial.print(", ");
      // Serial.print("IRBuffer");
      Serial.print(irBuffer[i]);
      Serial.print(", ");
      // Serial.print("heartRate");
      Serial.print(heartRate);
      Serial.print(", ");
      //  Serial.print("validHeartRate");
      Serial.print(validHeartRate);
      Serial.print(", ");
      // Serial.print("Oxygen Saturation ");
      Serial.print(spo2);
      Serial.print(", ");
// Serial.print("Oxygen Saturation");
      Serial.println(validSPO2);
    }

    // After gathering 25 new samples, recalculate HR and SP02
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  }
}


