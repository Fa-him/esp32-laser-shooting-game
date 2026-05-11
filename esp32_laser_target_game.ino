#include <WiFi.h>

const char* ssid = "wifi_name";
const char* password = "wifi_pass";

WiFiServer server(80);

// Pins
const int ledPins[5] = {13, 12, 14, 27, 26};
const int ldrPins[5] = {39, 33, 32, 35, 34};
const int buzzerPin = 4;

// Game
int score = 0;
int activeTarget = -1;
bool hitDone = false;
bool waitingToStart = true;
bool gameStarted = false;
bool gameOver = false;

int threshold[5] = {2600, 2000, 2000, 2000, 2000};

unsigned long startTime = 0;
unsigned long gameDuration = 120000;

unsigned long targetStartTime = 0;
unsigned long targetMaxTime = 3000;

void allOff() {
  for (int i = 0; i < 5; i++) digitalWrite(ledPins[i], LOW);
}

void beep(int t) {
  digitalWrite(buzzerPin, HIGH);
  delay(t);
  digitalWrite(buzzerPin, LOW);
}

void resetToStartMode() {
  score = 0;
  hitDone = false;
  waitingToStart = true;
  gameStarted = false;
  gameOver = false;

  allOff();

  activeTarget = 2; // target 3
  digitalWrite(ledPins[activeTarget], HIGH);

  Serial.println("Reset complete. Hit Target 3 to start.");
}

void chooseTarget() {
  int newT;
  do {
    newT = random(0, 5);
  } while (newT == activeTarget);

  allOff();
  activeTarget = newT;
  hitDone = false;
  targetStartTime = millis();

  digitalWrite(ledPins[activeTarget], HIGH);
}

void startGame() {
  waitingToStart = false;
  gameStarted = true;
  gameOver = false;
  score = 0;

  allOff();

  for (int i = 3; i > 0; i--) {
    beep(200);
    delay(600);
  }

  beep(400);

  startTime = millis();
  chooseTarget();
}

void endGame() {
  gameOver = true;
  gameStarted = false;
  allOff();

  for (int i = 0; i < 3; i++) {
    beep(200);
    delay(200);
  }
}

void handleWeb() {
  WiFiClient client = server.available();
  if (!client) return;

  String req = client.readStringUntil('\r');
  client.flush();

  // Restart endpoint
  if (req.indexOf("/restart") >= 0) {
    resetToStartMode();

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println();
    client.println("Restarted");
    client.stop();
    return;
  }

  int remaining = 120;

  if (gameStarted) {
    unsigned long elapsed = millis() - startTime;
    remaining = max(0, (int)((gameDuration - elapsed) / 1000));
  }

  // Data endpoint
  if (req.indexOf("/data") >= 0) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println();

    client.print("{\"score\":");
    client.print(score);
    client.print(",\"time\":");
    client.print(remaining);
    client.print(",\"gameOver\":");
    client.print(gameOver ? "true" : "false");
    client.print(",\"waiting\":");
    client.print(waitingToStart ? "true" : "false");
    client.println("}");

    client.stop();
    return;
  }

  // Main page
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();

  client.println(R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body{
margin:0;
height:100vh;
display:flex;
justify-content:center;
align-items:center;
background:linear-gradient(135deg,#0f172a,#7c3aed,#ef4444);
color:white;
font-family:Arial;
text-align:center;
overflow:hidden;
}

.card{
padding:40px;
border-radius:25px;
background:rgba(0,0,0,0.4);
box-shadow:0 20px 60px rgba(0,0,0,0.6);
}

h1{font-size:60px;}

.status{
font-size:28px;
color:#facc15;
margin-bottom:20px;
font-weight:bold;
}

.value{
font-size:80px;
margin:20px;
transition:0.2s;
}

.flash{
transform:scale(1.2);
color:#22c55e;
}

button{
font-size:24px;
padding:16px 35px;
border:none;
border-radius:15px;
background:#22c55e;
color:white;
font-weight:bold;
cursor:pointer;
box-shadow:0 0 25px rgba(34,197,94,0.7);
margin-top:20px;
}

button:hover{
background:#16a34a;
transform:scale(1.05);
}

#gameover{
position:fixed;
top:0;
left:0;
width:100%;
height:100%;
background:rgba(0,0,0,0.95);
display:none;
justify-content:center;
align-items:center;
flex-direction:column;
animation:fade 1s ease forwards;
}

@keyframes fade{
from{opacity:0;}
to{opacity:1;}
}

#gameover h1{
font-size:85px;
color:#ef4444;
animation:pop 1s infinite alternate;
}

@keyframes pop{
from{transform:scale(1);}
to{transform:scale(1.2);}
}
</style>
</head>

<body>

<div class="card">
<h1>LASER GAME</h1>

<div id="status" class="status">Hit Target 3 to Start</div>

<div>
<div>Score</div>
<div id="score" class="value">0</div>
</div>

<div>
<div>Time</div>
<div id="time" class="value">120</div>
</div>

<button onclick="restartGame()">Restart</button>
</div>

<div id="gameover">
<h1>GAME OVER</h1>
<h2 id="finalScore"></h2>
<button onclick="restartGame()">Restart Game</button>
</div>

<script>
let lastScore = 0;

function restartGame(){
fetch('/restart')
.then(() => {
  document.getElementById("gameover").style.display="none";
  document.getElementById("score").innerText = "0";
  document.getElementById("time").innerText = "120";
  document.getElementById("status").innerText = "Hit Target 3 to Start";
});
}

setInterval(()=>{
fetch('/data')
.then(r=>r.json())
.then(data=>{
let scoreEl = document.getElementById('score');
let timeEl = document.getElementById('time');
let statusEl = document.getElementById('status');

scoreEl.innerText = data.score;
timeEl.innerText = data.time;

if(data.waiting){
statusEl.innerText = "Hit Target 3 to Start";
} else if(data.gameOver){
statusEl.innerText = "Game Over";
} else {
statusEl.innerText = "Game Running";
}

if(data.score != lastScore){
scoreEl.classList.add("flash");
setTimeout(()=>scoreEl.classList.remove("flash"),200);
lastScore = data.score;
}

if(data.gameOver){
document.getElementById("gameover").style.display="flex";
document.getElementById("finalScore").innerText = "Final Score: " + data.score;
}
});
},200);
</script>

</body>
</html>
)rawliteral");

  client.stop();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("Open: http://");
  Serial.println(WiFi.localIP());

  server.begin();
  randomSeed(micros());

  resetToStartMode();
}

void loop() {
  handleWeb();

  if (waitingToStart) {
    if (analogRead(ldrPins[2]) > threshold[2]) {
      beep(100);
      startGame();
    }
    return;
  }

  if (gameOver) return;

  if (gameStarted) {
    unsigned long elapsed = millis() - startTime;

    if (elapsed >= gameDuration) {
      endGame();
      return;
    }

    if (millis() - targetStartTime > targetMaxTime) {
      chooseTarget();
    }

    int val = analogRead(ldrPins[activeTarget]);

    if (val > threshold[activeTarget] && !hitDone) {
      hitDone = true;
      score++;
      beep(100);
      chooseTarget();
    }
  }
}