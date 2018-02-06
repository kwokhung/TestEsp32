#include "SbrMpu.h"
#include "SbrMotor.h"

SbrMpu *sbrMpu = new SbrMpu("Self balancing Robot - MPU");
SbrMotor *sbrMotor = new SbrMotor("Self balancing Robot - Motor");

#define PRINT_PERIOD 100000 // print period in micros

#define MAX_PID_OUTPUT 500

float BASE_Kp = 100.0, BASE_Ki = 5.0, BASE_Kd = 130.0;
float Kp = BASE_Kp, Ki = BASE_Ki, Kd = BASE_Kd;
float angleSetpoint = 0, selfBalanceAngleSetpoint = 0;
float pidOutput, pidError, pidLastError, integralErr, positionErr, serialControlErr, prevSerialControlErr, errorDerivative;

float MAX_CONTROL_OR_POSITION_ERR = MAX_PID_OUTPUT / Kp;
float MAX_CONTROL_ERR_INCREMENT = MAX_CONTROL_OR_POSITION_ERR / 400;
#define MIN_CONTROL_ERR 1

// populated in the SerialControl part
uint8_t joystickX;
uint8_t joystickY;

uint32_t loop_timer;
uint32_t print_timer;

float roll, pitch, rollAcc, pitchAcc;
float speeed;

void setup()
{
  Serial.begin(500000);

  xTaskCreate(SbrMpu::startUp, "SBR - MPU", 10000, sbrMpu, 1, &sbrMpu->task);
  xTaskCreate(SbrMotor::startUp, "SBR - Motor", 10000, sbrMotor, 1, &sbrMotor->task);

  setup_serial_control();
  setup_wifi();

  loop_timer = micros() + PERIOD;
  print_timer = micros() + PRINT_PERIOD;
}

void loop()
{
  sbrMpu->getAcceleration(&sbrMpu->accX, &sbrMpu->accY, &sbrMpu->accZ);
  rollAcc = asin((float)sbrMpu->accX / ACC_SCALE_FACTOR) * RAD_TO_DEG;
  pitchAcc = asin((float)sbrMpu->accY / ACC_SCALE_FACTOR) * RAD_TO_DEG;

  sbrMpu->getRotation(&sbrMpu->gyroX, &sbrMpu->gyroY, &sbrMpu->gyroZ);
  // roll vs pitch depends on how the MPU is installed in the robot
  roll -= sbrMpu->gyroY * SbrMpu::GYRO_RAW_TO_DEGS;
  pitch += sbrMpu->gyroX * SbrMpu::GYRO_RAW_TO_DEGS;
  // sin() has to be applied on radians
  //  roll += pitch * sin((float)gyroZ * SbrMpu::GYRO_RAW_TO_DEGS * DEG_TO_RAD);
  //  pitch -= roll * sin((float)gyroZ * SbrMpu::GYRO_RAW_TO_DEGS * DEG_TO_RAD);

  roll = roll * 0.999 + rollAcc * 0.001;
  pitch = pitch * 0.999 + pitchAcc * 0.001;

  // apply PID algo

  // The selfBalanceAngleSetpoint variable is automatically changed to make sure that the robot stays balanced all the time.
  positionErr = SbrMpu::constrf(sbrMotor->currentPos / (float)1000, -MAX_CONTROL_OR_POSITION_ERR, MAX_CONTROL_OR_POSITION_ERR);
  serialControlErr = 0;

  if (isValidJoystickValue(joystickY))
  {
    serialControlErr = SbrMpu::constrf((joystickY - 130) / (float)15, -MAX_CONTROL_OR_POSITION_ERR, MAX_CONTROL_OR_POSITION_ERR);

    // this control has to change slowly/gradually to avoid shaking the robot
    if (serialControlErr < prevSerialControlErr)
    {
      serialControlErr = _max(serialControlErr, prevSerialControlErr - MAX_CONTROL_ERR_INCREMENT);
    }
    else
    {
      serialControlErr = _min(serialControlErr, prevSerialControlErr + MAX_CONTROL_ERR_INCREMENT);
    }

    prevSerialControlErr = serialControlErr;
  }

  pidError = pitch - angleSetpoint - selfBalanceAngleSetpoint;

  // either no manual / serial control -> try to keep the position or follow manual control
  if (abs(serialControlErr) > MIN_CONTROL_ERR)
  {
    pidError += serialControlErr > 0 ? serialControlErr - MIN_CONTROL_ERR : serialControlErr + MIN_CONTROL_ERR;
    // re-init position so it doesn't try to go back when getting out of manual control mode
    sbrMotor->currentPos = 0;
  }
  else
  {
    pidError += positionErr;
  }

  integralErr = SbrMpu::constrf(integralErr + Ki * pidError, -MAX_PID_OUTPUT, MAX_PID_OUTPUT);
  errorDerivative = pidError - pidLastError;

  pidOutput = Kp * pidError + integralErr + Kd * errorDerivative;

  if (pidOutput < 5 && pidOutput > -5)
  {
    pidOutput = 0; //Create a dead-band to stop the motors when the robot is balanced
  }

  if (pitch > 30 || pitch < -30)
  {
    //If the robot tips over
    pidOutput = 0;
    integralErr = 0;
    selfBalanceAngleSetpoint = 0;
  }

  // store error for next loop
  pidLastError = pidError;

  int16_t rotation = 0;

  if (isValidJoystickValue(joystickX))
  {
    rotation = SbrMpu::constrf((float)(joystickX - 130), -MAX_PID_OUTPUT, MAX_PID_OUTPUT) * (MAX_SPEED / MAX_PID_OUTPUT);
  }

  if (micros() >= print_timer)
  {
    Serial.print(positionErr);
    Serial.print("  -  ");
    Serial.print(rotation);
    Serial.print(" / ");
    Serial.println(serialControlErr);
    //Serial.print(pitch); Serial.print(" / ");Serial.print(errorDerivative);Serial.print(" - ");Serial.println(selfBalanceAngleSetpoint);
    //Serial.print(accX); Serial.print(" / ");Serial.print(accY); Serial.print(" / ");Serial.print(accZ); Serial.print(" / ");Serial.print(gyroX); Serial.print(" / ");Serial.print(gyroY);Serial.print(" / ");Serial.println(gyroZ);
    print_timer += PRINT_PERIOD;
  }

  //The self balancing point is adjusted when there is not forward or backwards movement from the transmitter. This way the robot will always find it's balancing point
  //  if(angleSetpoint == 0) {                                  //If the setpoint is zero degrees
  //    if(pidOutput < 0) selfBalanceAngleSetpoint -= 0.0015;   //Increase the self_balance_pid_setpoint if the robot is still moving forward
  //    if(pidOutput > 0) selfBalanceAngleSetpoint += 0.0015;   //Decrease the self_balance_pid_setpoint if the robot is still moving backward
  //  }

  sbrMotor->setSpeed(SbrMpu::constrf(pidOutput, -MAX_PID_OUTPUT, MAX_PID_OUTPUT) * (MAX_SPEED / MAX_PID_OUTPUT), rotation);

  // The angle calculations are tuned for a loop time of PERIOD milliseconds.
  // To make sure every loop is exactly that, a wait loop is created by setting the loop_timer
  if (loop_timer <= micros())
  {
    Serial.println("ERROR loop too short !");
  }

  while (loop_timer > micros())
  {
  }

  loop_timer += PERIOD;
}

HardwareSerial SerialControl(1);

uint8_t _prevChar;
boolean _readingMsg = false;
uint8_t _msg[6];
uint8_t _msgPos;
boolean _validData = false;

boolean startNewMsg(uint8_t c)
{
  boolean res = (_prevChar == 0) && (c == 255);
  _prevChar = c;
  return res;
}

void serialControlLoop(void *params)
{
  Serial.println("\nStarting thread dealing with Serial Control requests...");
  uint8_t currChar;

  while (true)
  {
    while (SerialControl.available())
    {
      currChar = SerialControl.read();

      if (startNewMsg(currChar))
      {
        _readingMsg = true;
        _msgPos = 0;
      }
      else if (_readingMsg)
      {
        if (_msgPos >= 6)
        {
          // data finished, last byte is the CRC
          uint8_t crc = 0;
          for (uint8_t i = 0; i < 6; i++)
            crc += _msg[i];

          if (crc == currChar)
          {
            joystickX = _msg[0];
            joystickY = _msg[1];
            _validData = true;
          }
          else
          {
            _validData = false;
            Serial.print("Wrong CRC: ");
            Serial.print(currChar);
            Serial.print(" Expected: ");
            Serial.println(crc);
          }

          _readingMsg = false;
        }
        else
        {
          // normal data, add it to the message
          _msg[_msgPos++] = currChar;
        }
      }
    }

    delay(1);
  }
}

void setup_serial_control()
{
  SerialControl.begin(9600, SERIAL_8N1, 17, 16);

  // deal with control requests in a separate thread, to avoid impacting the real time balancing
  // lower number means lower priority. 1 is just above tskIDLE_PRIORITY == 0  which is the lowest priority
  // use same core as rest of Arduino code as the other one is for system tasks
  xTaskCreatePinnedToCore(serialControlLoop, "serialControlLoop", 4096, NULL, 2, NULL, xPortGetCoreID());
}

boolean isValidJoystickValue(uint8_t joystick)
{
  return joystick > 20 && joystick < 230;
}

#include <WiFi.h>
#include <ESP32WebServer.h>

ESP32WebServer server(80);

void displayInfo()
{
  Serial.println("Handle Display Info...");
  String message = "<h1>Self balancing robot\n</h1><h2> ";
  message += "<p>Position=" + String(sbrMotor->currentPos) + "  <a href=\"update?Pos=inc\"><button>Pos++</button></a><a href=\"update?Pos=dec\"><button>Pos--</button></a></p>";
  message += "<p>Kp=" + String(Kp) + "  <a href=\"update?Kp=inc\"><button>Kp++</button></a><a href=\"update?Kp=dec\"><button>Kp--</button></a></p>";
  message += "<p>Ki=" + String(Ki) + "  <a href=\"update?Ki=inc\"><button>Ki++</button></a><a href=\"update?Ki=dec\"><button>Ki--</button></a></p>";
  message += "<p>Kd=" + String(Kd) + "  <a href=\"update?Kd=inc\"><button>Kd++</button></a><a href=\"update?Kd=dec\"><button>Kd--</button></a></p>";
  message += "<p>SP=" + String(angleSetpoint) + "  <a href=\"update?Sp=inc\"><button>SP++</button></a><a href=\"update?Sp=dec\"><button>SP--</button></a></p></h2>";
  server.send(200, "text/html", message);
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: " + server.uri();
  message += "\nMethod: " + (server.method() == HTTP_GET) ? " GET" : " POST";
  message += "\nArguments: " + server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void updateVars()
{
  // we expect one and only 1 var which is the name of what we need to change, and the value is "inc" or "dec"
  String var = server.argName(0);
  String action = server.arg(0);

  Serial.println("Handle " + action + " to " + var);

  if (var == "Pos")
    sbrMotor->currentPos = sbrMotor->currentPos + (action == "inc" ? 1000 : -1000);
  else if (var == "Kp")
    Kp = Kp + (action == "inc" ? BASE_Kp * 0.1 : -BASE_Kp * 0.1);
  else if (var == "Ki")
    Ki = Ki + (action == "inc" ? BASE_Ki * 0.1 : -BASE_Ki * 0.1);
  else if (var == "Kd")
    Kd = Kd + (action == "inc" ? BASE_Kd * 0.1 : -BASE_Kd * 0.1);
  else if (var == "Sp")
    angleSetpoint = angleSetpoint + (action == "inc" ? 0.1 : -0.1);
  else
    Serial.println("UNKNOWN var " + var);

  displayInfo();
}

void wifiLoop(void *params)
{
  Serial.println("Starting thread dealing with Wifi/HTTP client requests...");
  while (true)
  {
    // deals with the Wifi clients and responds, calls the callbacks, etc. ...
    server.handleClient();
    delay(1);
  }
}

void setup_wifi()
{
  Serial.println("\nConnecting to Wifi");
  WiFi.begin("xxx", "yyy");
  /*wait until ESP32 connect to WiFi*/
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi connected with IP address: ");
  Serial.println(WiFi.localIP());

  server.onNotFound(handleNotFound);
  server.on("/", displayInfo);
  server.on("/update", updateVars);

  server.begin();
  Serial.println("HTTP server started");

  // deal with WiFi/HTTP requests in a separate thread, to avoid impacting the real time balancing
  // lower number means lower priority. 1 is just above tskIDLE_PRIORITY == 0  which is the lowest priority
  // use same core as rest of Arduino code as the other one is for system tasks
  xTaskCreatePinnedToCore(wifiLoop, "wifiLoop", 4096, NULL, 1, NULL, xPortGetCoreID());
}
