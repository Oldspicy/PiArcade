// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


/****************************************************************************************************************************
 * Power button = pin 3 
 * In signal    = pin 6, A7
 * Out signal   = pin 2
 * Light string = pin 12 
 * Button light = pin 9
 * Relay        = pin 10
 * 
****************************************************************************************************************************/
#define NUMPIXELS       24
#define PIN             12
int PowerButton = 3; // pin 3
int Insignal = 6;   // analogRead A7
int Outsignal = 2;
int ButtonLight = 9;
int Relay = 10;
int threshold = 1000;
int colorChooser = 1;


long int countDown = 10000;
int delayTime = 10;

int delayval = 25;
byte powerFlag = 0;
byte stateFlag = 0;
byte runFlag = 0;
byte buzzFlag = 1;
int randomChooser = 20;
unsigned long previousMillis = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


/****************************************************************************************************************************/

void setup() 
{
  CircuitPlayground.begin();
  Serial.begin(9600);
  pinMode(PIN, OUTPUT);
  pinMode(PowerButton, INPUT_PULLUP);
  pinMode(Outsignal, OUTPUT);
  pinMode(Relay, OUTPUT);
  pinMode(ButtonLight, OUTPUT);
  pinMode(Insignal, INPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  attachInterrupt(digitalPinToInterrupt(PowerButton), power, FALLING);
  randomSeed(analogRead(A7));
  digitalWrite(Relay,0);
  digitalWrite(Outsignal, 0);
  for (int r = 0; r<=3; r++)
  {
    for (int x = 0; x <= NUMPIXELS; x++)
    {
      pixels.setPixelColor(x, 255,  255,   255);
      pixels.show();
      delay(25);
      pixels.setPixelColor(x, 0,   0,   0);
    }
  }
  
  
  //CircuitPlayground.playTone(750, 200);
  delay(1000);
  CircuitPlayground.clearPixels();
  Serial.println(analogRead(A7));
  CircuitPlayground.speaker.end(); 
}

void loop()
{
  //CircuitPlayground.setPixelColor(8, 0,0,255);
  if ((powerFlag == 1)&& (stateFlag == 0)) startupFunction();
  else if ((powerFlag == 0)&& (stateFlag == 1)) shutdownFunction();
  else if ((runFlag == 1)&& (powerFlag == 1)) lightsFunction();
//  else if ((runFlag == 0) && (powerFlag == 0)) off();
  else if (analogRead(A7) <= threshold) off();
}

/****************************************************************************************************************************/

void power()
{
  CircuitPlayground.setPixelColor(1, 0,0,255);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >=1200)
  {
    powerFlag =! powerFlag;
    runFlag =! runFlag;
    CircuitPlayground.playTone(1500, 200);
    digitalWrite(ButtonLight,0);
    previousMillis = currentMillis;
    CircuitPlayground.speaker.end();
  }
  
}

/****************************************************************************************************************************/

void startupFunction()
{
  Serial.println(analogRead(A7));
  CircuitPlayground.setPixelColor(2, 0,0,255);
//  Serial.println("Startup");
  digitalWrite(Relay, 1);
  digitalWrite(Outsignal, 0);
  while (analogRead(A7) <= threshold - 1)
//  while (Insignal == 0)
  {
    Serial.println(analogRead(A7));
    CircuitPlayground.setPixelColor(2, 255,   0,   0);
    digitalWrite(ButtonLight, 1);
    delay(500);
    CircuitPlayground.setPixelColor(2, 0,   0,   0);
    digitalWrite(ButtonLight, 0);
    delay(500);
  }
  Serial.println(analogRead(A7));
  digitalWrite(ButtonLight, 1);
  stateFlag = 1;
  buzzFlag = 0;
  //CircuitPlayground.playTone(1000, 200);
  CircuitPlayground.clearPixels();
}
/****************************************************************************************************************************/

void shutdownFunction()
{
  Serial.println(analogRead(A7));
  digitalWrite(Outsignal, 1);
  CircuitPlayground.setPixelColor(3, 0,0,255);
  //CircuitPlayground.playTone(750, 200);
  //digitalWrite(Relay, 1);
  while (analogRead(A7) >= threshold)
//  while (Insignal == 1)
  {
    Serial.println(analogRead(A7));
    CircuitPlayground.setPixelColor(3, 255,   0,   0);
    digitalWrite(ButtonLight, 1);
    delay(500);
    CircuitPlayground.setPixelColor(3, 0,   0,   0);
    digitalWrite(ButtonLight, 0);
    delay(500);
  }
  Serial.println(analogRead(A7));
  digitalWrite(ButtonLight, 0);
  digitalWrite(Relay, 0);
  CircuitPlayground.clearPixels();
  stateFlag = 0;
  
  off();
}

/****************************************************************************************************************************/

void lightsFunction()
{
  Serial.println(analogRead(A7));
  CircuitPlayground.setPixelColor(4, 0,0,255);
  Serial.println("Lights");
  switch (colorChooser)
  {
    case 1:
    if (powerFlag == 0) break;
      redGlow();
      break;
    case 2:
    if (powerFlag == 0) break;
      greenGlow();
      break;
    case 3:
    if (powerFlag == 0) break;
      purpleGlow();
      break;
    case 4:
    if (powerFlag == 0) break;
      blueGlow();
      break;
    case 5:
    if (powerFlag == 0) break;
      yellowGlow();
      break;
    case 6:
    if (powerFlag == 0) break;
      lightning();
      break;
  }
  CircuitPlayground.clearPixels();
}

void colorchoose()
{
  colorChooser++;
  if (colorChooser >=7) colorChooser = 1;
}
/****************************************************************************************************************************/

void off()
{
  Serial.println(analogRead(A7));
  digitalWrite(Outsignal, 0);
  for(int i=0;i<NUMPIXELS;i++)
  {
    Serial.println(analogRead(A7));
    pixels.setPixelColor(i, pixels.Color(0,0,0)); 
    pixels.show(); 
  }
  Serial.println(analogRead(A7));
  digitalWrite(Outsignal, 0);
  //digitalWrite(Relay, 1);
  digitalWrite(ButtonLight, 0);
  if (buzzFlag == 0) 
  {
    Serial.println(analogRead(A7));
//    CircuitPlayground.playTone(500, 200);
//    CircuitPlayground.playTone(250, 200);
//    CircuitPlayground.playTone(125, 200);
    
    CircuitPlayground.speaker.end();
    powerFlag = 0;
    runFlag = 0;
    stateFlag = 0;
    buzzFlag = 1;
  }
  digitalWrite(Relay,0);
  Serial.println(analogRead(A7));
}

/****************************************************************************************************************************/

void redGlow()
{
  while (runFlag == 1 && (colorChooser == 1))
    {
      Serial.println(analogRead(A7));
      CircuitPlayground.setPixelColor(5, 128, 128,   0);
      Serial.println("Red");
      if (powerFlag == 0) break;
        for (int x = 0; x <= 100; x++)
        {
          pixels.setBrightness(x);
          for(int i=0;i<NUMPIXELS;i++)
          {
            pixels.setPixelColor(i, pixels.Color(255,0,0)); 
            pixels.show(); 
            if (powerFlag == 0) break;
          }
          if (powerFlag == 0) break;
          delay(delayTime);
          if (powerFlag == 0) break;
          
    }
  
    for (int x = 100; x >= 0; x--)
    {
      pixels.setBrightness(x);
      for(int i=0;i<NUMPIXELS;i++)
      {
        pixels.setPixelColor(i, pixels.Color(255,0,0)); 
        pixels.show(); 
        if (powerFlag == 0) break;
      }
      pixels.show();
      if (powerFlag == 0) break;
      delay(delayTime);
      if (powerFlag == 0) break;
    }
    colorchoose();
  }
}
/****************************************************************************************************************************/
void greenGlow()
{
  while (runFlag == 1 && (colorChooser == 2))
  {
    Serial.println(analogRead(A7));
    if (powerFlag == 0) break;
    CircuitPlayground.setPixelColor(6, 128, 128,   0);
    Serial.println("Green");
    for (int x = 0; x <= 100; x++)
    {
      pixels.setBrightness(x);
      for(int i=0;i<NUMPIXELS;i++)
      {
        pixels.setPixelColor(i, pixels.Color(0,255,0)); 
        pixels.show(); 
        if (powerFlag == 0) break;
      }
      if (powerFlag == 0) break;
      delay(delayTime);
      if (powerFlag == 0) break;
    }
  
    for (int x = 100; x >= 0; x--)
    {
      pixels.setBrightness(x);
      for(int i=0;i<NUMPIXELS;i++)
      {
        pixels.setPixelColor(i, pixels.Color(0,255,0)); 
        pixels.show(); 
        if (powerFlag == 0) break;
      }
      pixels.show();
      if (powerFlag == 0) break;
      delay(delayTime);
      if (powerFlag == 0) break;
    }
    colorchoose();
  }
}
/****************************************************************************************************************************/
void purpleGlow()
{
  while (runFlag == 1 && (colorChooser == 3))
  {
    Serial.println(analogRead(A7));
    CircuitPlayground.setPixelColor(7, 128, 128,   0);
    if (powerFlag == 0) break;
    for (int x = 0; x <= 100; x++)
    {
      pixels.setBrightness(x);
      for(int i=0;i<NUMPIXELS;i++)
      {
        pixels.setPixelColor(i, pixels.Color(255,51,200)); 
        pixels.show(); 
        if (powerFlag == 0) break;
      }
      if (powerFlag == 0) break;
      delay(delayTime);
      if (powerFlag == 0) break; 
    }
  
    for (int x = 100; x >= 0; x--)
    {
      pixels.setBrightness(x);
      for(int i=0;i<NUMPIXELS;i++)
      {
        pixels.setPixelColor(i, pixels.Color(255,51,200)); 
        pixels.show(); 
        if (powerFlag == 0) break;
      }
      pixels.show();
      if (powerFlag == 0) break;
      delay(delayTime);
      if (powerFlag == 0) break;
    }
    colorchoose();
  }
}
/****************************************************************************************************************************/

void blueGlow()
{
  Serial.println(analogRead(A7));
  while (runFlag == 1 && (colorChooser == 4))
  {
    CircuitPlayground.setPixelColor(8, 128, 128,   0);
    if (powerFlag == 0) break;
    Serial.println("blue");
    for (int x = 0; x <= 100; x++)
    {
      pixels.setBrightness(x);
      for(int i=0;i<NUMPIXELS;i++)
      {
        pixels.setPixelColor(i, pixels.Color(0,0,255)); 
        pixels.show(); 
        if (powerFlag == 0) break;
      }
      if (powerFlag == 0) break;
      delay(delayTime);
      if (powerFlag == 0) break;
    }
  
    for (int x = 100; x >= 0; x--)
    {
      pixels.setBrightness(x);
      for(int i=0;i<NUMPIXELS;i++)
      {
        pixels.setPixelColor(i, pixels.Color(0,0,255)); 
        pixels.show(); 
        if (powerFlag == 0) break;
      }
      pixels.show();
      if (powerFlag == 0) break;
      delay(delayTime);
      if (powerFlag == 0) break;
    }
    colorchoose();
  }
}

/****************************************************************************************************************************/

void yellowGlow()
{
  while (runFlag == 1 && (colorChooser == 5))
  {
    Serial.println(analogRead(A7));
    CircuitPlayground.setPixelColor(9, 128, 128,   0);
    if (powerFlag == 0) break;
    Serial.println("yellow");
    for (int x = 0; x <= 100; x++)
    {
      pixels.setBrightness(x);
      for(int i=0;i<NUMPIXELS;i++)
      {
        pixels.setPixelColor(i, pixels.Color(255,255,51)); 
        pixels.show(); 
        if (powerFlag == 0) break;
      }
      if (powerFlag == 0) break;
      delay(delayTime);
      if (powerFlag == 0) break;
    }
  
    for (int x = 100; x >= 0; x--)
    {
      pixels.setBrightness(x);
      for(int i=0;i<NUMPIXELS;i++)
      {
        pixels.setPixelColor(i, pixels.Color(255,255,51)); 
        pixels.show(); 
        if (powerFlag == 0) break;
      }
      pixels.show();
      if (powerFlag == 0) break;
      delay(delayTime);
      if (powerFlag == 0) break;
    }
    colorchoose();
  }
}
/****************************************************************************************************************************/

void lightning()
{
  while (runFlag == 1 && (colorChooser == 6))
  {
    Serial.println(analogRead(A7));
    CircuitPlayground.setPixelColor(0, 128, 128,   0);
    if (powerFlag == 0) break;
    int grabby;
    int reader = analogRead(A7);
    long int timer = millis();
    if (powerFlag == 0) break;
    for (int x = 0; x <= 100; x++)
    {
      pixels.setBrightness(x);
      grabby = random(0,randomChooser);
      if (grabby == 15) crack();
      if (powerFlag == 0) break;
      for(int i=0;i<NUMPIXELS;i++)
      {
        pixels.setPixelColor(i, pixels.Color(0,0,255)); 
        pixels.show(); 
        if (powerFlag == 0) break;
      }
      if (powerFlag == 0) break;
      delay(delayTime);
      if (powerFlag == 0) break;
    }
    while (reader >= 500)
    {
      grabby = random(0,randomChooser);
      if (grabby == 15) crack();
      for(int i=0;i<NUMPIXELS;i++)
      {
        pixels.setPixelColor(i, pixels.Color(0,0,255)); 
        pixels.show(); 
      }
      if (powerFlag == 0) break;
      if (millis()-timer >= countDown)
      {
        reader = 0;
        colorchoose();
      }
    }
    
  
    for (int x = 100; x >= 0; x--)
    {
      pixels.setBrightness(x);
      grabby = random(0,randomChooser);
      if (grabby == 15) crack();
      for(int i=0;i<NUMPIXELS;i++)
      {
        pixels.setPixelColor(i, pixels.Color(0,0,255)); 
        pixels.show(); 
        if (powerFlag == 0) break;
      }
      pixels.show();
      if (powerFlag == 0) break;
      delay(delayTime);
      if (powerFlag == 0) break;
    }
  }
  
}

/****************************************************************************************************************************/

void crack()
{
    int rundown = random(0,10);
    int maxTime = 100;
    int timer = random(10,maxTime);
      int cloud = random(0,NUMPIXELS);
      pixels.setPixelColor(cloud, pixels.Color(255,255,255));
      pixels.setPixelColor(cloud+1, pixels.Color(255,255,255));
      pixels.setPixelColor(cloud-1, pixels.Color(255,255,255));
      if (rundown == 5)
      {
        for (int i = 0; i <=NUMPIXELS; i++)
        {
          pixels.setPixelColor(i, pixels.Color(255,255,255));
        }
      }
      pixels.show();
      if (timer >=maxTime/2)
      {
        delay(maxTime/3);
        pixels.setPixelColor(cloud, pixels.Color(0,0,0));
        pixels.setPixelColor(cloud+1, pixels.Color(0,0,0));
        pixels.setPixelColor(cloud-1, pixels.Color(0,0,0));
        pixels.show();
        delay(20);
        pixels.setPixelColor(cloud, pixels.Color(255,255,255));
        pixels.setPixelColor(cloud+1, pixels.Color(255,255,255));
        pixels.setPixelColor(cloud-1, pixels.Color(255,255,255));
        pixels.show();
        delay(maxTime/2);
        pixels.setPixelColor(cloud, pixels.Color(0,0,0));
        pixels.setPixelColor(cloud+1, pixels.Color(0,0,0));
        pixels.setPixelColor(cloud-1, pixels.Color(0,0,0));
        pixels.show();
        delay(20);
        pixels.setPixelColor(cloud, pixels.Color(255,255,255));
        pixels.setPixelColor(cloud+1, pixels.Color(255,255,255));
        pixels.setPixelColor(cloud-1, pixels.Color(255,255,255));
        pixels.show();
        delay(50);
      }
      else delay(timer);
}

