/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "../lib/neopixel/src/neopixel.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

const int LIGHT_COUNT = 24;
Adafruit_NeoPixel bar(LIGHT_COUNT, SPI1, WS2812);

int currentPos = 0;
int frequency = 250;
bool wave = false;

char colorName = 'Y';

int setColor(String inColorName){
  colorName = inColorName.c_str()[0];
  bar.setBrightness(50);
  return 0;
}

int setBrightness(String brightness){
  bar.setBrightness(atoi(brightness.c_str()));
  bar.show();
  return 0;
}

int setFrequency(String inFrequency){
  frequency = atoi(inFrequency.c_str());
  return 0;
}

int setMode(String _){
  wave = !wave;
  return 0;
}

int confirmSeq(String _){
  bar.setBrightness(50);
  for (int i = 0; i < LIGHT_COUNT; i++){
    bar.setPixelColor(i, 0, 255, 0);
  }
  bar.show();
  delay(1000);
  bar.setBrightness(0);
  return 0;
}

// setup() runs once, when the device is first turned on
void setup() {
  Particle.function("C", setColor);
  Particle.function("B", setBrightness);
  Particle.function("F", setFrequency);
  Particle.function("M", setMode);
  Particle.function("I", confirmSeq);
  bar.begin();
  bar.setBrightness(0);
  bar.setPixelColor(0, 255, 255, 0);
  bar.show();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  uint8_t red = 0, green = 0, blue = 0;
  switch (colorName)
  {
  case 'R':
    red = 255; green = 0; blue = 0;
    break;
  case 'G':
    red = 0; green = 255; blue = 0;
    break;
  case 'B':
    red = 0; green = 0; blue = 255;
    break;
  case 'Y':
    red = 255; green = 255; blue = 0;
    break;
  case 'W':
    red = green = blue = 255;
    break;
  default:
    break;
  }
  if (wave){
    bar.setPixelColor(currentPos, 0, 0, 0);
    bar.setPixelColor((currentPos + 1) % LIGHT_COUNT, red, green, blue);
    currentPos = (currentPos + 1) % LIGHT_COUNT;
  }else{
    for (int i = 0; i < LIGHT_COUNT; i++){
      bar.setPixelColor(i, red, green, blue);
    }
    bar.show();
    delay(frequency);
    for (int i = 0; i < LIGHT_COUNT; i++){
      bar.setPixelColor(i, 0, 0, 0);
    }
  }
  bar.show();
  delay(frequency);
}
