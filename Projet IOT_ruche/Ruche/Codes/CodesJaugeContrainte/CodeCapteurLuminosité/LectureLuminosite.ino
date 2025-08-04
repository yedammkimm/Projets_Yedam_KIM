/* TSL2591 Digital Light Sensor */
/* Dynamic Range: 600M:1 */
/* Maximum Lux: 88K */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
// #define DEBBUG

// Example for demonstrating the TSL2591 library - public domain!

// connect SCL to I2C Clock
// connect SDA to I2C Data
// connect Vin to 3.3-5V DC
// connect GROUND to common ground

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)


/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2591
*/
/**************************************************************************/
void configureSensor(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  // tsl.setGain(TSL2591_GAIN_MAX);      // 9876x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS); // compromis jour/nuit
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  /* Display the gain and integration time for reference sake */  
  #ifdef DEBBUG
    Serial.println(F("------------------------------------"));
    Serial.print  (F("Timing:       "));
    Serial.print((tsl.getTiming() + 1) * 100, DEC); 
    Serial.println(F(" ms"));
    Serial.println(F("------------------------------------"));
    Serial.println(F(""));
  #endif
}


/**************************************************************************/
/*
    Program entry point for the Arduino sketch
*/
/**************************************************************************/
void setup(void) 
{
  Serial.begin(9600);

  if (tsl.begin()) 
  {
    // Serial.println(F("Found a TSL2591 sensor"));
  } 
  else 
  {
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }
  
  /* Configure the sensor */
  configureSensor();
}



/**************************************************************************/
/*
    Show how to read IR and Full Spectrum at once and convert to lux
*/
/**************************************************************************/
void advancedRead(void)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity(); //récupère l'intensité lumineuse de tous le spectre
  uint16_t ir, full;
  ir = lum >> 16; //séparation de l'IR dans les bits de poids forts
  full = lum & 0xFFFF; 
  #ifdef DEBBUG
    Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
    Serial.print(F("IR: ")); Serial.print(ir);  Serial.print(F("  "));
    Serial.print(F("Full: ")); Serial.print(full); Serial.print(F("  "));
    Serial.print(F("Visible: ")); Serial.print(full - ir); Serial.print(F("  "));
  #endif
  
  Serial.print(F("Intensité Lumineuse : ")); Serial.print(tsl.calculateLux(full, ir), 6); Serial.println(F(" Lux"));

}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void) 
{ 
  advancedRead(); 
  
  delay(500);
}
