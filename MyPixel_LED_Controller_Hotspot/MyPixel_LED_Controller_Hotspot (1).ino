
#include <Adafruit_NeoPixel.h>
#include <WebServer.h>
#include <WiFi.h>

// Xiao ESPC3
#define LED_PIN D7
#define LED_COUNT 21

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// ESP32-C3 creates its own hotspot. No router credentials are used.
#define HOTSPOT_SSID "MyPixel-LED"
#define HOTSPOT_PASSWORD "12345678"

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

void startHotspot();
void handleRoot();
void handleSet();

void diwaliJhalar();
void blinkingWithColorChange();
void deepakGlow();
void randomSparkleWithColor();
void colorPulse();
void policeLights();
void ambulanceLights();
void rainbowCycle();
void theaterChase();
void fireEffect();

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

  startHotspot();

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

    // POLICE RED / BLUE FLASH
    case 11:
      policeLights();
      break;

    // AMBULANCE RED / WHITE FLASH
    case 12:
      ambulanceLights();
      break;

    // FULL RAINBOW CYCLE
    case 13:
      rainbowCycle();
      break;

    // THEATER CHASE
    case 14:
      theaterChase();
      break;

    // FIRE / FLAME EFFECT
    case 15:
      fireEffect();
      break;
  }
}

// =====================================================
// ESP32-C3 HOTSPOT MODE (NO ROUTER REQUIRED)
// =====================================================

void startHotspot() {

  // ESP32 creates its own Wi-Fi hotspot and keeps it available.
  WiFi.mode(WIFI_AP);
  WiFi.setSleep(false);

  // Fixed hotspot address: 192.168.4.1
  IPAddress localIP(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.softAPConfig(localIP, gateway, subnet);

  // Hotspot name and password (password must be at least 8 characters).
  bool started = WiFi.softAP(HOTSPOT_SSID, HOTSPOT_PASSWORD, 1, false, 4);

  if (started) {
    Serial.println();
    Serial.println("================================");
    Serial.println("MyPixel LED HOTSPOT STARTED");
    Serial.println("Wi-Fi Name : MyPixel-LED");
    Serial.println("Password   : 12345678");
    Serial.print("Open URL   : http://");
    Serial.println(WiFi.softAPIP());
    Serial.println("================================");

    // HOTSPOT READY -> GREEN BLINK 2 TIMES
    blinkColor(strip.Color(0, 255, 0), 2, 300);
  } else {
    Serial.println("ERROR: Hotspot could not be started!");
    blinkColor(strip.Color(255, 0, 0), 3, 250);
  }
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
// POLICE LIGHTS - RED / BLUE ALTERNATING
// =====================================================
void policeLights() {
  unsigned long now = millis();
  if (now - lastUpdate < 180) return;
  lastUpdate = now;
  static bool phase = false;
  phase = !phase;
  uint32_t red = strip.Color(255, 0, 0);
  uint32_t blue = strip.Color(0, 35, 255);
  for (int i = 0; i < strip.numPixels(); i++) {
    bool firstHalf = i < strip.numPixels() / 2;
    strip.setPixelColor(i, (firstHalf == phase) ? red : blue);
  }
  strip.show();
}

// =====================================================
// AMBULANCE LIGHTS - RED / WHITE ALTERNATING
// =====================================================
void ambulanceLights() {
  unsigned long now = millis();
  if (now - lastUpdate < 220) return;
  lastUpdate = now;
  static bool phase = false;
  phase = !phase;
  uint32_t red = strip.Color(255, 0, 0);
  uint32_t white = strip.Color(255, 255, 255);
  for (int i = 0; i < strip.numPixels(); i++) {
    bool redGroup = ((i / 3) % 2 == 0);
    strip.setPixelColor(i, (redGroup == phase) ? red : white);
  }
  strip.show();
}

// =====================================================
// RAINBOW CYCLE
// =====================================================
void rainbowCycle() {
  unsigned long now = millis();
  if (now - lastUpdate < 25) return;
  lastUpdate = now;
  static uint16_t hue = 0;
  for (int i = 0; i < strip.numPixels(); i++) {
    uint16_t pixelHue = hue + (uint32_t)i * 65536UL / strip.numPixels();
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
  }
  strip.show();
  hue += 256;
}

// =====================================================
// THEATER CHASE - MULTI COLOR
// =====================================================
void theaterChase() {
  unsigned long now = millis();
  if (now - lastUpdate < 100) return;
  lastUpdate = now;
  static uint8_t step = 0;
  strip.clear();
  uint32_t colors[] = {
    strip.Color(255, 0, 40),
    strip.Color(0, 100, 255),
    strip.Color(0, 255, 100)
  };
  for (int i = 0; i < strip.numPixels(); i++) {
    if ((i + step) % 3 == 0) {
      strip.setPixelColor(i, colors[(i / 3) % 3]);
    }
  }
  strip.show();
  step = (step + 1) % 3;
}

// =====================================================
// FIRE EFFECT
// =====================================================
void fireEffect() {
  unsigned long now = millis();
  if (now - lastUpdate < 65) return;
  lastUpdate = now;
  for (int i = 0; i < strip.numPixels(); i++) {
    int heat = random(100, 256);
    int red = heat;
    int green = max(0, heat - (int)random(80, 170));
    int blue = (heat > 220) ? random(0, 18) : 0;
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}

// =====================================================
// WEB PAGE - RESPONSIVE MYPIXEL DASHBOARD
// =====================================================
void handleRoot() {

  String wifiName;
  String ipAddress;
  String wifiState;

  // This firmware runs ONLY in Access Point (hotspot) mode.
  wifiName = HOTSPOT_SSID;
  ipAddress = WiFi.softAPIP().toString();
  wifiState = "Hotspot Active";

  char colorHex[8];
  sprintf(colorHex, "#%02X%02X%02X",
          (staticColor >> 16) & 0xFF,
          (staticColor >> 8) & 0xFF,
          staticColor & 0xFF);

  String html = R"rawliteral(
<!DOCTYPE html><html lang="en"><head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1">
<meta name="theme-color" content="#080d19">
<title>MyPixel LED Studio</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:Arial,Helvetica,sans-serif;background:#080d19;color:#f4f7ff;min-height:100vh}
header{padding:17px 5%;display:flex;justify-content:space-between;align-items:center;gap:12px;background:#0e1728;border-bottom:1px solid #26344d;position:sticky;top:0;z-index:5}
.logo{font-size:21px;font-weight:900;letter-spacing:1px}.logo b{color:#35d9ff}
.live{font-size:12px;color:#58e6a5;white-space:nowrap}
.wrap{width:92%;max-width:1150px;margin:25px auto}
.hero{padding:25px;border:1px solid #35496c;border-radius:20px;background:linear-gradient(115deg,#142c48,#27204f 65%,#142c48);position:relative;overflow:hidden;margin-bottom:20px}
.hero:after{content:'✦';position:absolute;right:35px;top:10px;font-size:100px;color:#7deaff;opacity:.13}
.hero h1{font-size:27px;margin-bottom:8px}.hero p{font-size:13px;color:#bdcce4;line-height:1.5}
.section{font-size:18px;margin:23px 0 13px}
.panel{background:#111b2c;border:1px solid #273650;border-radius:17px;padding:18px;margin-bottom:18px}
.info-grid{display:grid;grid-template-columns:repeat(3,1fr);gap:10px}
.info{background:#0b1423;border:1px solid #24334d;border-radius:12px;padding:13px;min-width:0}
.label{font-size:10px;color:#91a4c2;text-transform:uppercase;letter-spacing:1px;margin-bottom:7px}
.val{font-size:14px;font-weight:bold;overflow-wrap:anywhere}.green{color:#58e6a5}
.actions{display:grid;grid-template-columns:1fr 1fr;gap:10px;margin-top:13px}
button{font:inherit}.power,.offbtn{border:0;border-radius:11px;padding:14px;font-weight:bold;cursor:pointer;background:linear-gradient(100deg,#00bd82,#35e6a4);color:#061b15}
.offbtn{background:#293850;color:#fff}
.patterns{display:grid;grid-template-columns:repeat(4,1fr);gap:12px}
.pattern{border:1px solid #293954;background:#111b2c;color:#fff;border-radius:15px;padding:12px 8px;cursor:pointer;text-align:center;min-width:0;transition:.18s}
.pattern:active{transform:scale(.97)}.pattern.active{border:2px solid #35d9ff;background:#172c43;box-shadow:0 0 16px #35d9ff22}
.ico{height:53px;display:grid;place-items:center;font-size:30px;background:#091321;border-radius:10px;margin-bottom:9px}
.pname{font-size:12px;font-weight:bold;line-height:1.3}.pnum{font-size:10px;color:#8093b1;margin-top:5px}
.controls{display:grid;grid-template-columns:1fr 1fr;gap:20px}
.control-head{display:flex;justify-content:space-between;gap:8px;font-size:13px;margin-bottom:12px}
input[type=range]{width:100%;accent-color:#35d9ff}input[type=color]{width:100%;height:48px;border:1px solid #40516b;border-radius:10px;background:#0b1423;padding:4px}
.note{color:#8495b0;font-size:11px;margin-top:12px;line-height:1.5}.footer{text-align:center;color:#71829e;font-size:11px;padding:20px}
@media(max-width:720px){.patterns{grid-template-columns:repeat(3,1fr)}.info-grid{grid-template-columns:1fr 1fr}.info:last-child{grid-column:1/-1}}
@media(max-width:430px){header{padding:14px 4%}.logo{font-size:17px}.live{font-size:10px}.wrap{width:94%;margin:16px auto}.hero{padding:20px 17px}.hero h1{font-size:22px}.patterns{grid-template-columns:repeat(2,1fr);gap:9px}.pattern{padding:10px 7px}.ico{height:48px;font-size:27px}.panel{padding:14px}.controls{grid-template-columns:1fr;gap:18px}}
</style></head><body>
<header><div class="logo">MY<b>PIXEL</b> LED</div><div class="live">● DEVICE ONLINE</div></header>
<main class="wrap">
<section class="hero"><h1>✨ LED Control Studio</h1><p>Smart lighting control · Live effects · Color customization</p></section>
<h2 class="section">📡 Network & Device</h2>
<section class="panel">
 <div class="info-grid">
  <div class="info"><div class="label">Wi-Fi Network</div><div class="val" id="wifiName">__WIFI__</div></div>
  <div class="info"><div class="label">Connection</div><div class="val green">__WIFI_STATE__</div></div>
  <div class="info"><div class="label">IP Address</div><div class="val">__IP__</div></div>
 </div>
 <div class="actions"><button id="powerBtn" class="offbtn" onclick="togglePower()">⏻ POWER ON</button><button class="offbtn" onclick="location.reload()">↻ REFRESH STATUS</button></div>
</section>
<h2 class="section">🎆 Lighting Effects</h2>
<section class="patterns" id="patterns"></section>
<h2 class="section">🎨 Customize Lights</h2>
<section class="panel"><div class="controls">
 <div><div class="control-head"><span>☀ Brightness</span><b id="brightnessValue">__BRIGHTNESS__ / 255</b></div><input id="brightness" type="range" min="0" max="255" value="__BRIGHTNESS__" oninput="showBrightness(this.value)" onchange="setBrightness(this.value)"></div>
 <div><div class="control-head"><span>🌈 LED Color</span><b id="colorLabel">__COLOR__</b></div><input id="colorPicker" type="color" value="__COLOR__" onchange="setColor(this.value)"></div>
 </div><p class="note">Police and ambulance patterns are visual LED simulations only. They do not generate an audible siren.</p>
</section>
<div class="footer">MyPixel LED Controller · ESP32-C3 · 21 Addressable LEDs</div>
</main>
<script>
const effects=[
 ['🌙','Breathe'],['🎨','Static Color'],['💡','Blink'],['➡️','Chase'],
 ['✨','Sparkle'],['☄️','Comet'],['🪔','Diwali Jhalar'],['🔄','Color Change'],
 ['🪷','Deepak Glow'],['🌟','Random Sparkle'],['🌈','Color Pulse'],['🚓','Police Lights'],
 ['🚑','Ambulance'],['🌈','Rainbow Cycle'],['🎭','Theater Chase'],['🔥','Fire Effect']
];
let isOn=__POWER__, currentMode=__MODE__;
const grid=document.getElementById('patterns');
effects.forEach((e,i)=>{
 const b=document.createElement('button');b.className='pattern';b.innerHTML='<div class="ico">'+e[0]+'</div><div class="pname">'+e[1]+'</div><div class="pnum">EFFECT '+String(i+1).padStart(2,'0')+'</div>';
 b.onclick=()=>selectMode(i);b.id='effect'+i;grid.appendChild(b);
});
function command(obj){fetch('/set?'+new URLSearchParams(obj)).catch(e=>console.log(e));}
function selectMode(m){currentMode=m;command({mode:m});document.querySelectorAll('.pattern').forEach((p,i)=>p.classList.toggle('active',i===m));}
function paintPower(){let b=document.getElementById('powerBtn');b.textContent=isOn?'⏻ POWER OFF':'⏻ POWER ON';b.className=isOn?'power':'offbtn';}
function togglePower(){isOn=!isOn;command({on:isOn?1:0});paintPower();}
function showBrightness(v){document.getElementById('brightnessValue').textContent=v+' / 255';}
function setBrightness(v){command({brightness:v});}
function setColor(hex){document.getElementById('colorLabel').textContent=hex.toUpperCase();let r=parseInt(hex.slice(1,3),16),g=parseInt(hex.slice(3,5),16),b=parseInt(hex.slice(5,7),16);command({r,g,b});}
paintPower();selectMode(currentMode);
</script></body></html>
)rawliteral";

  html.replace("__WIFI__", wifiName);
  html.replace("__WIFI_STATE__", wifiState);
  html.replace("__IP__", ipAddress);
  html.replace("__POWER__", isOn ? "true" : "false");
  html.replace("__MODE__", String(constrain(currentMode, 0, 15)));
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
    } else {
      lastUpdate = 0;
    }
  }

  if (server.hasArg("mode")) {
    currentMode = constrain(server.arg("mode").toInt(), 0, 15);
    lastUpdate = 0;
  }

  if (server.hasArg("brightness")) {
    brightness = constrain(server.arg("brightness").toInt(), 0, 255);
    strip.setBrightness(brightness);
  }

  if (server.hasArg("r") && server.hasArg("g") && server.hasArg("b")) {
    uint8_t r = constrain(server.arg("r").toInt(), 0, 255);
    uint8_t g = constrain(server.arg("g").toInt(), 0, 255);
    uint8_t b = constrain(server.arg("b").toInt(), 0, 255);
    staticColor = strip.Color(r, g, b);
  }

  server.send(200, "text/plain", "OK");
}
