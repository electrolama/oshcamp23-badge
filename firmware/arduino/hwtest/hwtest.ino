// Quick hardware test firmware for the OSHCamp 2023 Badge
// Details at: http://lab.electrolama.com/ob23
// Requires the ESP8266 Arduino core from:
//   https://github.com/esp8266/Arduino

#include <ESP8266WiFi.h>

// install the following libraries via Sketch -> Include Library -> Manage Libraries
#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel
#include <CircularBuffer.h>    // https://github.com/rlogiacco/CircularBuffer
#include <BitBang_I2C.h>       // https://github.com/bitbank2/BitBang_I2C

#define BTN1    5
#define BTN2    4
#define BTN3    0    // has ext pullup
#define BTN4    2    // has ext pulup, LED on ESP-12 module
#define BTN5    12
#define BTN6    14
#define BTN7    15   // has ext pulldown
#define BUZZER  13
#define RGBLED  BTN1
#define I2C_SDA BTN6
#define I2C_SCL BTN5

#define NOTE_A 440
#define NOTE_B 494
#define NOTE_C 523
#define NOTE_D 587
#define NOTE_E 659
#define NOTE_F 698
#define NOTE_G 784
#define DURATION 100

#define CIRCULAR_BUFFER_INT_SAFE
CircularBuffer<int,20> melody;

//#define USE_I2C
#ifdef USE_I2C
BBI2C bbi2c;
#endif

Adafruit_NeoPixel rgb(1, RGBLED, NEO_GRB + NEO_KHZ400);

const char* ssid     = "wifi-ap";
const char* password = "supersecretpassword";


ICACHE_RAM_ATTR void btn1_isr(){
  Serial.println("btn1 pressed!");
  melody.push(NOTE_A);
}

ICACHE_RAM_ATTR void btn2_isr(){
  Serial.println("btn2 pressed!");
  melody.push(NOTE_B);
}

ICACHE_RAM_ATTR void btn3_isr(){
  Serial.println("btn3 pressed!");
  melody.push(NOTE_C);
}

ICACHE_RAM_ATTR void btn4_isr(){
  Serial.println("btn4 pressed!");
  melody.push(NOTE_D);
}

ICACHE_RAM_ATTR void btn5_isr(){
  Serial.println("btn5 pressed!");
  melody.push(NOTE_E);
}

ICACHE_RAM_ATTR void btn6_isr(){
  Serial.println("btn6 pressed!");
  melody.push(NOTE_F);
}

ICACHE_RAM_ATTR void btn7_isr(){
  Serial.println("btn7 pressed!");
  melody.push(NOTE_G);
}


void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(16, INPUT); // not used, prevent clash on RevA1 boards

  Serial.println("rgb led init begin");
  pinMode(RGBLED, OUTPUT);
  rgb.begin();
  rgb.setBrightness(20);
  rgb.clear();
  rgb.setPixelColor(0, rgb.Color(0, 0, 50));
  rgb.show();
  delay(100);
  Serial.println("rgb led init done");

  pinMode(BTN1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN1), btn1_isr, FALLING);
  pinMode(BTN2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN2), btn2_isr, FALLING);
  pinMode(BTN3, INPUT);  // ext pullup
  attachInterrupt(digitalPinToInterrupt(BTN3), btn3_isr, FALLING);
  pinMode(BTN4, INPUT);  // ext pullup
  attachInterrupt(digitalPinToInterrupt(BTN4), btn4_isr, FALLING);

#ifdef USE_I2C
  Serial.println("Using bitbanged I2C so BTN_5 and BTN_6 are INOP");
  bbi2c.bWire = 0; // use bit banging
  bbi2c.iSDA = I2C_SDA;
  bbi2c.iSCL = I2C_SCL;
  I2CInit(&bbi2c, 100000); // 100KHz
#else
  pinMode(BTN5, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN5), btn5_isr, FALLING);
  pinMode(BTN6, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN6), btn6_isr, FALLING);
#endif

  pinMode(BTN7, INPUT);  // ext pulldown
  attachInterrupt(digitalPinToInterrupt(BTN7), btn7_isr, RISING);

  pinMode(BUZZER, OUTPUT);
  
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int x = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++x); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer

///////////////////////////
#ifdef USE_I2C
  // I2C_Detector.ino 
  uint8_t map[16];
  char szTemp[32];
  uint8_t i;
  int iDevice, iCount;
  uint32_t u32Caps;

  I2CScan(&bbi2c, map); // get bitmap of connected I2C devices
  if (map[0] == 0xfe) // something is wrong with the I2C bus
  {
    Serial.println("I2C pins are not correct or the bus is being pulled low by a bad device; unable to run scan");
  }
  else
  {
    iCount = 0;
    for (i=1; i<128; i++) // skip address 0 (general call address) since more than 1 device can respond
    {
      if (map[i>>3] & (1 << (i & 7))) // device found
      {
        iCount++;
        Serial.print("Device found at 0x");
        Serial.print(i, HEX);
        iDevice = I2CDiscoverDevice(&bbi2c, i, &u32Caps);
        Serial.print(", type = ");
        I2CGetDeviceName(iDevice, szTemp);
        Serial.print(szTemp); // show the device name as a string
        Serial.print(", capability bits = 0x");
        Serial.println(u32Caps, HEX);
      }
    } // for i
    Serial.print(iCount, DEC);
    Serial.println(" I2C device(s) found");
  }
#endif
//////////////////
}


void loop() {
  // TODO: add debounce logic
  
  while (!melody.isEmpty()) {
    tone(BUZZER, melody.pop());
    delay(DURATION);
    noTone(BUZZER);
  }
}
