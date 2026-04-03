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
