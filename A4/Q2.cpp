#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "AT8";
const char* password = "ATW$@8888";

WebServer server(80);

// LED pins
const int led1Pin = 26;
const int led2Pin = 27;

// LED states
bool led1State = false;
bool led2State = false;

// HTML page with LED control and surprise images
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Fun LED Control</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    :root {
      --bg-color: #121212;
      --card-color: #1e1e1e;
      --text-color: #e0e0e0;
      --primary: #bb86fc;
      --primary-dark: #3700b3;
      --on-color: #4caf50;
      --off-color: #f44336;
    }
    
    body {
      font-family:Arial, sans-serif;
      background-color: var(--bg-color);
      color: var(--text-color);
      margin: 0;
      padding: 20px;
      display: flex;
      flex-direction: column;
      align-items: center;
      min-height: 100vh;
      text-align: center;
      overflow : hidden;
    }
    
    h1 {
      color: var(--primary);
      margin-bottom: 10px;
      text-shadow: 0 0 10px var(--primary);
      animation: glow 2s infinite alternate;
    }
    
    @keyframes glow {
      from { text-shadow: 0 0 5px var(--primary); }
      to { text-shadow: 0 0 15px var(--primary); }
    }
    
    .container {
      display: flex;
      flex-wrap: wrap;
      justify-content: center;
      gap: 20px;
      width: 100%;
      max-width: 800px;
    }
    
    .card {
      background-color: var(--card-color);
      border-radius: 15px;
      padding: 20px;
      width: 200px;
      text-align: center;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.3);
      transition: all 0.3s ease;
      position: relative;
      overflow: hidden;
    }
    
    .card:hover {
      transform: translateY(-5px);
      box-shadow: 0 8px 16px rgba(0, 0, 0, 0.4);
    }
    
    .led-icon {
      width: 80px;
      height: 80px;
      margin: 0 auto 15px;
      border-radius: 50%;
      background-color: #333;
      display: flex;
      align-items: center;
      justify-content: center;
      font-size: 40px;
      transition: all 0.3s ease;
      box-shadow: inset 0 0 10px rgba(0,0,0,0.5);
    }
    
    .led-on .led-icon {
      background-color: var(--on-color);
      box-shadow: 0 0 20px var(--on-color);
      animation: pulse 1s infinite;
    }
    
    @keyframes pulse {
      0% { box-shadow: 0 0 10px var(--on-color); }
      50% { box-shadow: 0 0 20px var(--on-color); }
      100% { box-shadow: 0 0 10px var(--on-color); }
    }
    
    .btn {
      border: none;
      border-radius: 25px;
      padding: 12px 24px;
      font-size: 16px;
      cursor: pointer;
      transition: all 0.3s ease;
      width: 100%;
      font-weight: bold;
      margin-top: 10px;
      position: relative;
      overflow: hidden;
    }
    
    .btn:before {
      content: "";
      position: absolute;
      top: 0;
      left: -100%;
      width: 100%;
      height: 100%;
      background: linear-gradient(90deg, transparent, rgba(255,255,255,0.2), transparent);
      transition: 0.5s;
    }
    
    .btn:hover:before {
      left: 100%;
    }
    
    .btn-on {
      background-color: var(--on-color);
      color: white;
    }
    
    .btn-off {
      background-color: var(--off-color);
      color: white;
    }
    
    .surprise {
      position: fixed;
      top: 50%;
      left: 50%;
      transform: translate(-50%, -50%) scale(0);
      max-width: 80%;
      max-height: 80%;
      border-radius: 10px;
      box-shadow: 0 0 0 1000px rgba(0,0,0,0.8);
      z-index: 100;
      transition: transform 0.5s ease;
    }
    
    .surprise.show {
      transform: translate(-50%, -50%) scale(1);
    }
    
    .close-btn {
      position: absolute;
      top: 10px;
      right: 10px;
      background: #f44336;
      color: white;
      border: none;
      border-radius: 50%;
      width: 30px;
      height: 30px;
      font-weight: bold;
      cursor: pointer;
      z-index: 101;
    }
    
    footer {
      margin-top: 30px;
      color: #666;
      font-size: 12px;
    }
  </style>
</head>
<body>
  <h1>Fun LED Controller</h1>
  <p>Turn on the LEDs for a surprise!</p>
  
  <div class="container">
    <div class="card %LED1CLASS%">
      <h2>LED 1</h2>
      <div class="led-icon">💡</div>
      <form action="/led1">
        <button class="btn %LED1BTNCLASS%">%LED1BTNTEXT%</button>
      </form>
    </div>
    
    <div class="card %LED2CLASS%">
      <h2>LED 2</h2>
      <div class="led-icon">💡</div>
      <form action="/led2">
        <button class="btn %LED2BTNCLASS%">%LED2BTNTEXT%</button>
      </form>
    </div>
  </div>
  
  <footer>
    IoT SW & HW Engineering - Summer 2025 | Problem 2 | Made with Levi
  </footer>
  
  <script>
    // Show surprise when LED is turned on
    function checkSurprise() {
      %LED1SHOW%
      %LED2SHOW%
    }
    
    function hideSurprise() {
      document.querySelectorAll('.surprise').forEach(img => {
        img.classList.remove('show');
      });
    }
    
    // Check on page load
    window.onload = checkSurprise;
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  String html = htmlPage;
  
  // LED 1 state
  bool led1State = digitalRead(led1Pin);
  html.replace("%LED1CLASS%", led1State ? "led-on" : "");
  html.replace("%LED1BTNCLASS%", led1State ? "btn-off" : "btn-on");
  html.replace("%LED1BTNTEXT%", led1State ? "TURN OFF" : "TURN ON");
  html.replace("%LED1SHOW%", led1State ? 
    "document.getElementById('surprise1').classList.add('show');" : "");
  
  // LED 2 state
  bool led2State = digitalRead(led2Pin);
  html.replace("%LED2CLASS%", led2State ? "led-on" : "");
  html.replace("%LED2BTNCLASS%", led2State ? "btn-off" : "btn-on");
  html.replace("%LED2BTNTEXT%", led2State ? "TURN OFF" : "TURN ON");
  html.replace("%LED2SHOW%", led2State ? 
    "document.getElementById('surprise2').classList.add('show');" : "");
  
  server.send(200, "text/html", html);
}

void handleLED1() {
  digitalWrite(led1Pin, !digitalRead(led1Pin));
  handleRoot();
}

void handleLED2() {
  digitalWrite(led2Pin, !digitalRead(led2Pin));
  handleRoot();
}

void setup() {
  Serial.begin(115200);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);

  WiFi.begin(ssid, password,0, NULL, true);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/led1", handleLED1);
  server.on("/led2", handleLED2);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}