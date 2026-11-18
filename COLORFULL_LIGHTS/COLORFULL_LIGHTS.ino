
#include <Adafruit_NeoPixel.h>
#include <WebServer.h>
#include <WiFi.h>

// Xiao ESPC3
#define LED_PIN D7
#define LED_COUNT 21

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
// WEB DASHBOARD - RESPONSIVE MOBILE + DESKTOP
// =====================================================

void handleRoot() {

  char colorHex[8];

  sprintf(colorHex, "#%02X%02X%02X",
          (staticColor >> 16) & 0xFF,
          (staticColor >> 8) & 0xFF,
          staticColor & 0xFF);

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta name="theme-color" content="#080d19">

<title>MyPixel LED Controller</title>

<style>
* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
}

body {
  font-family: Arial, Helvetica, sans-serif;
  background: #080d19;
  color: #f5f7ff;
  min-height: 100vh;
}

header {
  padding: 20px 5%;
  display: flex;
  justify-content: space-between;
  align-items: center;
  border-bottom: 1px solid #202a3d;
  background: #0d1423;
}

.logo {
  font-size: 23px;
  font-weight: bold;
  letter-spacing: 1px;
}

.logo span {
  color: #35d9ff;
}

.connection {
  color: #43e69b;
  font-size: 13px;
}

.container {
  width: 92%;
  max-width: 1150px;
  margin: 28px auto;
}

.hero {
  background: linear-gradient(120deg, #172744, #202052, #172744);
  border: 1px solid #33476b;
  border-radius: 20px;
  padding: 28px;
  margin-bottom: 22px;
  position: relative;
  overflow: hidden;
}

.hero h1 {
  font-size: 28px;
  margin-bottom: 10px;
}

.hero p {
  color: #b4c2df;
  font-size: 14px;
}

.hero-glow {
  position: absolute;
  right: 45px;
  top: -65px;
  width: 180px;
  height: 180px;
  border-radius: 50%;
  background: #25caff;
  filter: blur(90px);
  opacity: .35;
}

.section-title {
  font-size: 19px;
  margin: 25px 0 15px;
}

.panel {
  background: #111a2b;
  border: 1px solid #25324a;
  border-radius: 17px;
  padding: 20px;
  margin-bottom: 20px;
}

.top-controls {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 15px;
}

.status {
  display: flex;
  flex-direction: column;
  gap: 8px;
}

.small-label {
  color: #9baac4;
  font-size: 12px;
}

.value {
  font-size: 21px;
  font-weight: bold;
}

.power-btn {
  width: 100%;
  padding: 15px;
  border: none;
  border-radius: 12px;
  font-size: 16px;
  font-weight: bold;
  cursor: pointer;
  color: white;
  background: #26364e;
}

.power-btn.on {
  background: linear-gradient(100deg, #00b878, #00d69a);
  color: #061b15;
}

.pattern-grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 13px;
}

.pattern {
  background: #111a2b;
  border: 1px solid #273550;
  border-radius: 15px;
  padding: 12px;
  cursor: pointer;
  transition: .2s;
  color: white;
  text-align: left;
  min-width: 0;
}

.pattern:hover {
  border-color: #35d9ff;
  transform: translateY(-3px);
}

.pattern.active {
  border: 2px solid #35d9ff;
  background: #172b42;
}

.pattern-name {
  font-size: 13px;
  font-weight: bold;
  margin-top: 12px;
}

.pattern-number {
  color: #8192b0;
  font-size: 11px;
  margin-top: 4px;
}

.preview {
  height: 65px;
  border-radius: 9px;
  background: #080d19;
  display: flex;
  justify-content: center;
  align-items: center;
  gap: 5px;
  overflow: hidden;
}

.dot {
  width: 9px;
  height: 9px;
  border-radius: 50%;
  background: #35d9ff;
  box-shadow: 0 0 12px currentColor;
  animation: blink 1s infinite alternate;
}

.dot:nth-child(2) { animation-delay: .2s; }
.dot:nth-child(3) { animation-delay: .4s; }
.dot:nth-child(4) { animation-delay: .6s; }
.dot:nth-child(5) { animation-delay: .8s; }

.p1 .dot { animation: none; background: #ff3d63; }
.p2 .dot { animation-duration: .3s; background: #ffe04d; }
.p3 .dot { animation-duration: .5s; }
.p4 .dot { animation: none; background: #b35cff; }
.p5 .dot { animation-duration: .7s; }
.p6 .dot { background: #ff9c35; }
.p7 .dot { background: #43ffae; animation-duration: .25s; }
.p8 .dot { background: #ffb347; animation-duration: 1.2s; }
.p9 .dot { background: #ff50c8; animation-duration: .4s; }
.p10 .dot { background: #35d9ff; animation-duration: 1.5s; }

@keyframes blink {
  from { opacity: .2; transform: scale(.7); }
  to { opacity: 1; transform: scale(1.5); }
}

.control-row {
  margin: 18px 0;
}

.control-head {
  display: flex;
  justify-content: space-between;
  margin-bottom: 12px;
  font-size: 14px;
}

input[type=range] {
  width: 100%;
  accent-color: #35d9ff;
  cursor: pointer;
}

input[type=color] {
  width: 100%;
  height: 52px;
  border: 2px solid #33445f;
  border-radius: 10px;
  background: transparent;
  cursor: pointer;
  padding: 4px;
}

.footer {
  text-align: center;
  color: #74839c;
  font-size: 12px;
  padding: 25px;
}

@media(max-width:650px) {
  header {
    padding: 17px 4%;
  }

  .logo {
    font-size: 18px;
  }

  .container {
    width: 94%;
    margin: 18px auto;
  }

  .hero {
    padding: 22px 17px;
  }

  .hero h1 {
    font-size: 22px;
  }

  .pattern-grid {
    grid-template-columns: repeat(2, 1fr);
    gap: 9px;
  }

  .pattern {
    padding: 9px;
  }

  .preview {
    height: 55px;
  }

  .top-controls {
    gap: 10px;
  }

  .panel {
    padding: 15px;
  }
}

@media(max-width:350px) {
  .pattern-grid {
    grid-template-columns: 1fr;
  }
}
</style>
</head>

<body>

<header>
  <div class="logo">MY<span>PIXEL</span> LED</div>
  <div class="connection">● ESP32 CONTROLLER</div>
</header>

<div class="container">

  <div class="hero">
    <div class="hero-glow"></div>
    <h1>LED Control Studio</h1>
    <p>Control your lights, explore effects and customize colors.</p>
  </div>

  <h2 class="section-title">Device Control</h2>

  <div class="panel">
    <div class="top-controls">

      <div class="status">
        <span class="small-label">LED STATUS</span>
        <span class="value" id="powerStatus">OFF</span>
      </div>

      <div class="status">
        <span class="small-label">ACTIVE PATTERN</span>
        <span class="value" id="modeStatus">Breathe</span>
      </div>

    </div>

    <br>

    <button class="power-btn" id="powerBtn" onclick="togglePower()">
      POWER ON
    </button>
  </div>

  <h2 class="section-title">Lighting Patterns</h2>

  <div class="pattern-grid" id="patternGrid">

    <button class="pattern" data-mode="0">
      <div class="preview p0">
        <i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i>
      </div>
      <div class="pattern-name">Breathe</div>
      <div class="pattern-number">Pattern 01</div>
    </button>

    <button class="pattern" data-mode="1">
      <div class="preview p1">
        <i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i>
      </div>
      <div class="pattern-name">Static Color</div>
      <div class="pattern-number">Pattern 02</div>
    </button>

    <button class="pattern" data-mode="2">
      <div class="preview p2">
        <i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i>
      </div>
      <div class="pattern-name">Blink</div>
      <div class="pattern-number">Pattern 03</div>
    </button>

    <button class="pattern" data-mode="3">
      <div class="preview p3">
        <i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i>
      </div>
      <div class="pattern-name">Chase</div>
      <div class="pattern-number">Pattern 04</div>
    </button>

    <button class="pattern" data-mode="4">
      <div class="preview p4">
        <i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i>
      </div>
      <div class="pattern-name">Sparkle</div>
      <div class="pattern-number">Pattern 05</div>
    </button>

    <button class="pattern" data-mode="5">
      <div class="preview p5">
        <i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i>
      </div>
      <div class="pattern-name">Comet</div>
      <div class="pattern-number">Pattern 06</div>
    </button>

    <button class="pattern" data-mode="6">
      <div class="preview p6">
        <i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i>
      </div>
      <div class="pattern-name">Diwali Jhalar</div>
      <div class="pattern-number">Pattern 07</div>
    </button>

    <button class="pattern" data-mode="7">
      <div class="preview p7">
        <i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i>
      </div>
      <div class="pattern-name">Color Change</div>
      <div class="pattern-number">Pattern 08</div>
    </button>

    <button class="pattern" data-mode="8">
      <div class="preview p8">
        <i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i>
      </div>
      <div class="pattern-name">Deepak Glow</div>
      <div class="pattern-number">Pattern 09</div>
    </button>

    <button class="pattern" data-mode="9">
      <div class="preview p9">
        <i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i>
      </div>
      <div class="pattern-name">Random Sparkle</div>
      <div class="pattern-number">Pattern 10</div>
    </button>

    <button class="pattern" data-mode="10">
      <div class="preview p10">
        <i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i><i class="dot"></i>
      </div>
      <div class="pattern-name">Color Pulse</div>
      <div class="pattern-number">Pattern 11</div>
    </button>

  </div>

  <h2 class="section-title">Customize Lighting</h2>

  <div class="panel">

    <div class="control-row">
      <div class="control-head">
        <span>Brightness</span>
        <b id="brightnessValue">120</b>
      </div>

      <input type="range" id="brightness"
             min="0" max="255" value="120"
             oninput="updateBrightness(this.value)"
             onchange="sendBrightness(this.value)">
    </div>

    <div class="control-row">
      <div class="control-head">
        <span>Choose LED Color</span>
        <span id="colorLabel">#FF0000</span>
      </div>

      <input type="color" id="colorPicker"
             value="#FF0000"
             onchange="updateColor(this.value)">
    </div>

  </div>

</div>

<div class="footer">
  MyPixel LED Controller | Powered by ESP32
</div>

<script>

let isOn = false;
let currentMode = 0;

const names = [
  "Breathe", "Static Color", "Blink",
  "Chase", "Sparkle", "Comet",
  "Diwali Jhalar", "Color Change",
  "Deepak Glow", "Random Sparkle", "Color Pulse"
];

function sendCommand(params) {
  fetch("/set?" + new URLSearchParams(params))
    .catch(err => console.log("Connection error:", err));
}

function togglePower() {
  isOn = !isOn;

  sendCommand({on: isOn ? 1 : 0});

  document.getElementById("powerStatus").textContent =
    isOn ? "ON" : "OFF";

  const btn = document.getElementById("powerBtn");

  btn.textContent = isOn ? "POWER OFF" : "POWER ON";
  btn.classList.toggle("on", isOn);
}

function selectMode(mode) {
  currentMode = mode;

  sendCommand({mode: mode});

  document.getElementById("modeStatus").textContent =
    names[mode];

  document.querySelectorAll(".pattern").forEach(card => {
    card.classList.toggle(
      "active",
      Number(card.dataset.mode) === mode
    );
  });
}

document.querySelectorAll(".pattern").forEach(card => {
  card.addEventListener("click", () => {
    selectMode(Number(card.dataset.mode));
  });
});

function updateBrightness(value) {
  document.getElementById("brightnessValue").textContent = value;
}

function sendBrightness(value) {
  sendCommand({brightness: value});
}

function updateColor(hex) {

  document.getElementById("colorLabel").textContent =
    hex.toUpperCase();

  const r = parseInt(hex.substring(1, 3), 16);
  const g = parseInt(hex.substring(3, 5), 16);
  const b = parseInt(hex.substring(5, 7), 16);

  sendCommand({r: r, g: g, b: b});
}

// INITIAL DEVICE VALUES

isOn = __POWER__;
currentMode = __MODE__;

document.getElementById("brightness").value = __BRIGHTNESS__;
document.getElementById("brightnessValue").textContent = __BRIGHTNESS__;

document.getElementById("colorPicker").value = "__COLOR__";
document.getElementById("colorLabel").textContent = "__COLOR__";

document.getElementById("powerStatus").textContent =
  isOn ? "ON" : "OFF";

document.getElementById("powerBtn").textContent =
  isOn ? "POWER OFF" : "POWER ON";

document.getElementById("powerBtn").classList.toggle("on", isOn);

selectMode(currentMode);

</script>

</body>
</html>
)rawliteral";

  html.replace("__POWER__", isOn ? "true" : "false");
  html.replace("__MODE__", String(constrain(currentMode, 0, 10)));
  html.replace("__BRIGHTNESS__", String(brightness));
  html.replace("__COLOR__", String(colorHex));

  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "text/html", html);
}


// =====================================================
// HANDLE SET
// =====================================================

void handleSet() {

  if (server.hasArg("on")) {
    isOn = server.arg("on").toInt() != 0;

    if (!isOn) {
      strip.clear();
      strip.show();
    }
  }

  if (server.hasArg("mode")) {
    currentMode = constrain(server.arg("mode").toInt(), 0, 10);
    lastUpdate = 0;
  }

  if (server.hasArg("brightness")) {
    brightness = constrain(server.arg("brightness").toInt(), 0, 255);
    strip.setBrightness(brightness);
  }

  if (server.hasArg("r") &&
      server.hasArg("g") &&
      server.hasArg("b")) {

    uint8_t r = constrain(server.arg("r").toInt(), 0, 255);
    uint8_t g = constrain(server.arg("g").toInt(), 0, 255);
    uint8_t b = constrain(server.arg("b").toInt(), 0, 255);

    staticColor = strip.Color(r, g, b);
  }

  server.send(200, "text/plain", "OK");
}
