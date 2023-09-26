#include <Adafruit_MCP4728.h>
#include <Wire.h>

const int SWC_OFF = 13;
const int SWC_ON = 12;
const int SWB_OFF = 14;
const int SWB_ON = 27;

const int SWA = 25;
const int SWD = 26;

// MID CHAN A = 1830
// MAX CHAN A = 3760 (neg throttle...)
// MIN CHAN A = 15

// MID CHAN B = 1925
// MAX CHAN B = 2790
// MIN CHAN B = 335

Adafruit_MCP4728 mcp;

void setup() {
    Serial.begin(115200);

    Serial.println("Starting...");

  if (!mcp.begin(0x64)) {
    Serial.println("Failed to find MCP4728 chip");
    while (1) {
      delay(10);
    }
  }

  mcp.setChannelValue(MCP4728_CHANNEL_A, 1830);
  mcp.setChannelValue(MCP4728_CHANNEL_B, 1925);
  mcp.setChannelValue(MCP4728_CHANNEL_C, 0);
  mcp.setChannelValue(MCP4728_CHANNEL_D, 0);
  mcp.saveToEEPROM();

  pinMode(SWC_OFF, OUTPUT);
  pinMode(SWC_ON, OUTPUT);
  pinMode(SWB_OFF, OUTPUT);
  pinMode(SWB_ON, OUTPUT);
  pinMode(SWA, OUTPUT);
  pinMode(SWD, OUTPUT);

  digitalWrite(SWC_OFF, HIGH);
  digitalWrite(SWC_ON, HIGH);
  digitalWrite(SWB_OFF, HIGH);
  digitalWrite(SWB_ON, HIGH);
  digitalWrite(SWA, HIGH);
  digitalWrite(SWD, HIGH);
}

String input;
char a[5];
char b[5];
char c[5];
char d[3];

void loop() {
    if (Serial.available()) {
        input = Serial.readStringUntil('\n');

        input.substring(0, 4).toCharArray(a, 5);
        input.substring(4, 8).toCharArray(b, 5);
        input.substring(8, 12).toCharArray(c, 5);
        input.substring(12, 14).toCharArray(d, 3);

        uint16_t ai = strtoul(a, 0, 16);
        uint16_t bi = strtoul(b, 0, 16);
        uint16_t ci = strtoul(c, 0, 16);

        // 12 = switches

        uint8_t sw = strtoul(d, 0, 16);

        if ((sw & 0x80) == 0x80) {
            // Pulling to ground is ON
            digitalWrite(SWA, LOW);
        }
        else {
            digitalWrite(SWA, HIGH);
        }

        if ((sw & 0x40) == 0x40) {
            digitalWrite(SWD, LOW);
        }
        else {
            digitalWrite(SWD, HIGH);
        }

        // SWB Pos 2
        if ((sw & 0x20) == 0x20) {
            // pull ON side to ground
            digitalWrite(SWB_ON, LOW);
            digitalWrite(SWB_OFF, HIGH);
        } 
        else if ((sw & 0x10) == 0x10) {
            // sw pos 1 (middle)
            digitalWrite(SWB_ON, HIGH);
            digitalWrite(SWB_OFF, HIGH);
        }
        else {
            // sw pos off?
            digitalWrite(SWB_ON, HIGH);
            digitalWrite(SWB_OFF, LOW);
        }

        // SWC Pos 2
        if ((sw & 0x08) == 0x08) {
            // pull ON side to ground
            digitalWrite(SWC_ON, LOW);
            digitalWrite(SWC_OFF, HIGH);
        } 
        else if ((sw & 0x04) == 0x04) {
            // swc pos 1 (middle)
            digitalWrite(SWC_ON, HIGH);
            digitalWrite(SWC_OFF, HIGH);
        }
        else {
            // swc pos off?
            digitalWrite(SWC_ON, HIGH);
            digitalWrite(SWC_OFF, LOW);
        }

        mcp.fastWrite(ai, bi, ci, 0);
    }

    delay(10);
}