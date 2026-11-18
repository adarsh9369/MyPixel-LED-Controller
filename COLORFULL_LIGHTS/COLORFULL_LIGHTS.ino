
#include <Adafruit_NeoPixel.h>
#include <WebServer.h>

// Xiao ESPC3
#define LED_PIN     9
#define LED_COUNT   21

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// WiFi Credentials
// const char* ssids[] = {
//   "Nidhi Mishra",
//   "Test",
//   "ERAS_MEDICAL_COLLEGE"
// };

// const char* passwords[] = {
//   "negi8877",
//   "9369614100",
//   "Eras786@"
// };

const char* ssids[] = {
  
  "ERAS_MEDICAL_COLLEGE",
  "Nidhi Mishra",
  "Test"
};

const char* passwords[] = {
  "Eras786@",
  "negi8877",
  "9369614100",
};

const int WIFI_COUNT = sizeof(ssids) / sizeof(ssids[0]);

WebServer server(80);

int currentMode = 0;
int brightness = 120;

uint32_t staticColor = strip.Color(255, 0, 0);

unsigned long lastUpdate = 0;

bool isOn = false;
bool blinkState = false;

// =====================================================
// FUNCTION DECLARATION
// =====================================================

void connectToWiFi();
void handleRoot();
void handleSet();

void diwaliJhalar();
void blinkingWithColorChange();
void deepakGlow();
void randomSparkleWithColor();
void colorPulse();

void blinkColor(uint32_t color, int times, int delayTime);

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  strip.begin();
  strip.setBrightness(brightness);
  strip.clear();
  strip.show();

  // ESP START -> RED BLINK 2 TIMES
  blinkColor(strip.Color(255, 0, 0), 2, 300);

  connectToWiFi();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/set", HTTP_GET, handleSet);

  server.begin();

  Serial.println("HTTP server started");
}

// =====================================================
// LOOP
// =====================================================

void loop() {

  server.handleClient();

  unsigned long now = millis();

  if (!isOn) {
    strip.clear();
    strip.show();
    return;
  }

  strip.setBrightness(brightness);

  switch (currentMode) {

    // =================================================
    // BREATHE
    // =================================================
    case 0:

      if (now - lastUpdate > 20) {

        lastUpdate = now;

        static int breatheVal = 0;
        static int dir = 1;

        breatheVal += dir * 5;

        if (breatheVal >= 255 || breatheVal <= 0) {
          dir = -dir;
        }

        float ratio = breatheVal / 255.0;

        uint8_t r = (staticColor >> 16) & 0xFF;
        uint8_t g = (staticColor >> 8) & 0xFF;
        uint8_t b = staticColor & 0xFF;

        strip.fill(strip.Color(r * ratio, g * ratio, b * ratio));

        strip.show();
      }

      break;

    // =================================================
    // STATIC COLOR
    // =================================================
    case 1:

      strip.fill(staticColor);
      strip.show();

      break;

    // =================================================
    // BLINK
    // =================================================
    case 2:

      if (now - lastUpdate > 500) {

        lastUpdate = now;

        blinkState = !blinkState;

        if (blinkState) {
          strip.fill(staticColor);
        } else {
          strip.clear();
        }

        strip.show();
      }

      break;

    // =================================================
    // CHASE
    // =================================================
    case 3:

      if (now - lastUpdate > 100) {

        lastUpdate = now;

        static int chaseIndex = 0;

        strip.clear();

        strip.setPixelColor(chaseIndex, staticColor);

        strip.show();

        chaseIndex++;

        if (chaseIndex >= strip.numPixels()) {
          chaseIndex = 0;
        }
      }

      break;

    // =================================================
    // SPARKLE
    // =================================================
    case 4:

      if (now - lastUpdate > 50) {

        lastUpdate = now;

        strip.clear();

        strip.setPixelColor(random(strip.numPixels()), staticColor);

        strip.show();
      }

      break;

    // =================================================
    // COMET
    // =================================================
    case 5:

      if (now - lastUpdate > 50) {

        lastUpdate = now;

        static int cometIndex = 0;

        strip.clear();

        for (int j = 0; j < 5; j++) {

          int index = (cometIndex - j + strip.numPixels()) % strip.numPixels();

          strip.setPixelColor(index, staticColor);
        }

        strip.show();

        cometIndex++;

        if (cometIndex >= strip.numPixels()) {
          cometIndex = 0;
        }
      }

      break;

    // =================================================
    // DIWALI JHALAR
    // =================================================
    case 6:
      diwaliJhalar();
      break;

    // =================================================
    // BLINK COLOR CHANGE
    // =================================================
    case 7:
      blinkingWithColorChange();
      break;

    // =================================================
    // DEEPAK GLOW
    // =================================================
    case 8:
      deepakGlow();
      break;

    // =================================================
    // RANDOM SPARKLE
    // =================================================
    case 9:
      randomSparkleWithColor();
      break;

    // =================================================
    // COLOR PULSE
    // =================================================
    case 10:
      colorPulse();
      break;
  }
}

// =====================================================
// WIFI CONNECT
// =====================================================

void connectToWiFi() {

  WiFi.mode(WIFI_STA);

  for (int i = 0; i < WIFI_COUNT; i++) {

    Serial.printf("Trying WiFi %d/%d : %s\n", i + 1, WIFI_COUNT, ssids[i]);

    WiFi.begin(ssids[i], passwords[i]);

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - start < 5000) {

      delay(100);
    }

    if (WiFi.status() == WL_CONNECTED) {

      Serial.println("WiFi Connected");
      Serial.println(WiFi.localIP());

      // WIFI CONNECTED -> GREEN BLINK 2 TIMES
      blinkColor(strip.Color(0, 255, 0), 2, 300);

      return;
    }

    WiFi.disconnect(true);
  }

  Serial.println("WiFi Failed");

  WiFi.mode(WIFI_AP);

  WiFi.softAP("MyPixel-AP");

  Serial.println(WiFi.softAPIP());
}

// =====================================================
// BLINK FUNCTION
// =====================================================

void blinkColor(uint32_t color, int times, int delayTime) {

  for (int i = 0; i < times; i++) {

    strip.fill(color);

    strip.show();

    delay(delayTime);

    strip.clear();

    strip.show();

    delay(delayTime);
  }
}

// =====================================================
// DIWALI JHALAR
// =====================================================

void diwaliJhalar() {

  unsigned long now = millis();

  if (now - lastUpdate > 100) {

    lastUpdate = now;

    static int counter = 0;
    static int hue = 0;

    strip.clear();

    for (int i = 0; i < strip.numPixels(); i++) {

      if ((i + counter) % 3 == 0) {

        strip.setPixelColor(
          i,
          strip.gamma32(strip.ColorHSV(hue))
        );
      }
    }

    strip.show();

    counter++;

    if (counter >= 3) {
      counter = 0;
    }

    hue += 512;

    if (hue >= 65536) {
      hue = 0;
    }
  }
}

// =====================================================
// BLINK COLOR CHANGE
// =====================================================

void blinkingWithColorChange() {

  unsigned long now = millis();

  if (now - lastUpdate > 500) {

    lastUpdate = now;

    static int hue = 0;

    blinkState = !blinkState;

    if (blinkState) {

      uint32_t newColor =
        strip.gamma32(strip.ColorHSV(hue));

      strip.fill(newColor);

    } else {

      strip.clear();
    }

    strip.show();

    hue += 2048;

    if (hue >= 65536) {
      hue = 0;
    }
  }
}

// =====================================================
// DEEPAK GLOW
// =====================================================

void deepakGlow() {

  unsigned long now = millis();

  if (now - lastUpdate > 100) {

    lastUpdate = now;

    for (int i = 0; i < strip.numPixels() - 5; i++) {

      strip.setPixelColor(i, staticColor);
    }

    for (int i = strip.numPixels() - 5;
         i < strip.numPixels();
         i++) {

      int flicker = random(-20, 20);

      int red = constrain(255 + flicker, 0, 255);
      int green = constrain(100 + flicker, 0, 255);
      int blue = constrain(0 + flicker / 2, 0, 255);

      strip.setPixelColor(
        i,
        strip.Color(red, green, blue)
      );
    }

    strip.show();
  }
}

// =====================================================
// RANDOM SPARKLE
// =====================================================

void randomSparkleWithColor() {

  unsigned long now = millis();

  if (now - lastUpdate > 50) {

    lastUpdate = now;

    strip.clear();

    int randomPixel = random(strip.numPixels());

    int randomHue = random(65536);

    strip.setPixelColor(
      randomPixel,
      strip.gamma32(strip.ColorHSV(randomHue))
    );

    strip.show();
  }
}

// =====================================================
// COLOR PULSE
// =====================================================

void colorPulse() {

  unsigned long now = millis();

  if (now - lastUpdate > 20) {

    lastUpdate = now;

    static int pulseHue = 0;

    strip.fill(
      strip.gamma32(strip.ColorHSV(pulseHue))
    );

    strip.show();

    pulseHue += 128;

    if (pulseHue >= 65536) {
      pulseHue = 0;
    }
  }
}

// =====================================================
// WEB PAGE
// =====================================================

void handleRoot() {

  String html = "<h1>ESP32 LED Controller</h1>";

  server.send(200, "text/html", html);
}

// =====================================================
// HANDLE SET
// =====================================================

void handleSet() {

  if (server.hasArg("on")) {

    isOn = server.arg("on").toInt();
  }

  if (server.hasArg("mode")) {

    currentMode = server.arg("mode").toInt();
  }

  if (server.hasArg("brightness")) {

    brightness = server.arg("brightness").toInt();

    strip.setBrightness(brightness);
  }

  if (server.hasArg("r") &&
      server.hasArg("g") &&
      server.hasArg("b")) {

    uint8_t r = server.arg("r").toInt();
    uint8_t g = server.arg("g").toInt();
    uint8_t b = server.arg("b").toInt();

    staticColor = strip.Color(r, g, b);
  }

  server.send(200, "text/plain", "OK");
}