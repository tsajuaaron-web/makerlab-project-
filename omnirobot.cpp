#include <WebServer.h>
#include <WebSocketsServer.h>

// ---#include <WiFi.h> Wi-Fi Credentials ---
const char* ssid = "OmniRobot_WS";
const char* password = "password123";

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// --- Pin Definitions ---
const int M1_RPWM = 25, M1_LPWM = 26, M1_REN = 33, M1_LEN = 32;
const int M2_RPWM = 27, M2_LPWM = 14, M2_REN = 12, M2_LEN = 13;
const int M3_RPWM = 18, M3_LPWM = 19, M3_REN = 5,  M3_LEN = 17;

// --- Kinematics & Motor Control ---
void setMotor(int rpwm, int lpwm, int speed) {
  int s = constrain(speed, -255, 255);
  if (s > 0) {
    analogWrite(rpwm, s);
    analogWrite(lpwm, 0);
  } else {
    analogWrite(rpwm, 0);
    analogWrite(lpwm, -s);
  }
}
void moveRobot(int x, int y, int turn) {
  // Turn is scaled by 0.4 for "Slow Rotation"
  float slowTurn = turn * 0.4; 

  int m1Speed = x + slowTurn;
  int m2Speed = (-0.5 * x) - (0.866 * y) + slowTurn;
  int m3Speed = (-0.5 * x) + (0.866 * y) + slowTurn;
setMotor(M1_RPWM, M1_LPWM, m1Speed);
  setMotor(M2_RPWM, M2_LPWM, m2Speed);
  setMotor(M3_RPWM, M3_LPWM, m3Speed);
}

// --- WebSocket Handler ---
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    int x, y, t;
    // Format expected: "x,y,t"
    if (sscanf((char*)payload, "%d,%d,%d", &x, &y, &t) == 3) {
      moveRobot(x, y, t);
    }
  }
}
// --- Web Interface ---
void handleRoot() {
  String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no
  html += "<style>body{background:#222; color:white; text-align:center; font-family:sans-serif; touch-action:none;}";
  html += ".btn{width:70px; height:70px; margin:5px; font-size:20px; border-radius:10px; border:none; background:#444; color:white;}";
  html += ".btn:active{background:#00ff00; color:black;} .rot{background:#55acee;} .stop{background:#ff4b2b; width:100%}</style></head><body>";
   html += "<h1>Omni WS Control</h1>";
   html += "<table>";
html += "<tr><td></td><td><button class='btn' onmousedown='send(0,200,0)' onmouseup='send(0,0,0)' ontouchstart='send(0,200,0)' ontouchend='send(0,0,0)'>&uarr;</button></td><td></td></tr>";
  html += "<tr><td><button class='btn' onmousedown='send(-200,0,0)' onmouseup='send(0,0,0)' ontouchstart='send(-200,0,0)' ontouchend='send(0,0,0)'>&larr;</button></td>";
  html += "<td><button class='btn rot' onmousedown='send(0,0,-150)' onmouseup='send(0,0,0)' ontouchstart='send(0,0,-150)' ontouchend='send(0,0,0)'>&#8634;</button></td>";
  html += "<td><button class='btn' onmousedown='send(200,0,0)' onmouseup='send(0,0,0)' ontouchstart='send(200,0,0)' ontouchend='send(0,0,0)'>&rarr;</button></td></tr>";
  html += "<tr><td><button class='btn rot' onmousedown='send(0,0,150)' onmouseup='send(0,0,0)' ontouchstart='send(0,0,150)' ontouchend='send(0,0,0)'>&#8635;</button></td>";
  html += "<td><button class='btn' onmousedown='send(0,-200,0)' onmouseup='send(0,0,0)' ontouchstart='send(0,-200,0)' ontouchend='send(0,0,0)'>&darr;</button></td><td></td></tr>";
  html += "</table><br><button class='btn stop' onclick='send(0,0,0)'>STOP</button>";
