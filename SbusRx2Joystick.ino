// SbusRx2Joystick
// 2020/04/20
//
// Add board manager
// https://raw.githubusercontent.com/sparkfun/Arduino_Boards/master/IDE_Board_Manager/package_sparkfun_index.json

// https://github.com/MHeironimus/ArduinoJoystickLibrary
#include <Joystick.h>

// bolderflight S.BUS Library
// https://github.com/bolderflight/SBUS
#include <SBUS.h>

const uint16_t Threshold = 1024 + 256;
const int BUTTON_COUNT = 8;

SBUS sbus(Serial1);

Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID, // hidReportId
  JOYSTICK_TYPE_JOYSTICK,     // joystickType
  BUTTON_COUNT,               // buttonCount
  0,                          // hatSwitchCount
  true,                       // includeXAxis
  true,                       // includeYAxis
  false,                      // includeZAxis
  false,                      // includeRxAxis
  false,                      // includeRyAxis
  false,                      // includeRzAxis
  true,                       // includeRudder
  true,                       // includeThrottle
  false,                      // includeAccelerator
  false,                      // includeBrake
  false);                     // includeSteering

// S.BUS frame data: channel, fail safe, and lost frames data
uint16_t channels[16];
bool failSafe;
bool lostFrame;

void setup() {
  sbus.begin();

  Joystick.setXAxisRange(224, 1824);
  Joystick.setYAxisRange(224, 1824);
  Joystick.setThrottleRange(224, 1824);
  Joystick.setRudderRange(224, 1824);
  Joystick.begin(false);

  // 動作確認のためのシリアル通信
//  Serial.begin(9600);
}

void loop() {
  if(sbus.read(&channels[0], &failSafe, &lostFrame)){
    // スティックのアナログ値をシリアルコンソールに表示して確認
    // 11bit (0 ~ 2047)の値が取得できます。
    // max, min ,center値をメモします。
//    Serial.println(channels[0]);
    
    Joystick.setXAxis(channels[0]);
    Joystick.setYAxis(channels[1]);
    Joystick.setThrottle(channels[2]);
    Joystick.setRudder(channels[3]);

    for (int i = 0; i < BUTTON_COUNT; i++) {
      Joystick.setButton(i, (uint8_t)(channels[i+4] > Threshold));
    }

    Joystick.sendState();
  }
}
