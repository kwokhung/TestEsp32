#include <Wire.h>

#define PERIOD 4000         // loop period in micros
#define PRINT_PERIOD 100000 // print period in micros

#define MOT_R_ENB 32
#define MOT_R_STP 33
#define MOT_R_DIR 25
#define MOT_R_CHANNEL 1 // for the ledc library

#define MOT_L_ENB 26
#define MOT_L_STP 14
#define MOT_L_DIR 27
#define MOT_L_CHANNEL 2 // for the ledc library

#define MAX_SPEED 20000

uint32_t prevSpeedStart;
int16_t prevSpeed;
int32_t currentPos = 0;

void setup_motors()
{
  ledcAttachPin(MOT_L_STP, MOT_L_CHANNEL);
  ledcSetup(MOT_L_CHANNEL, 0, 10); // these will be updated later by the ledcWriteNote()
  ledcAttachPin(MOT_R_STP, MOT_R_CHANNEL);
  ledcSetup(MOT_R_CHANNEL, 0, 10); // these will be updated later by the ledcWriteNote()

  pinMode(MOT_L_ENB, OUTPUT);
  pinMode(MOT_L_DIR, OUTPUT);
  disableL(true);

  pinMode(MOT_R_ENB, OUTPUT);
  pinMode(MOT_R_DIR, OUTPUT);
  disableR(true);
}

void disableL(bool orEnable)
{
  digitalWrite(MOT_L_ENB, orEnable);
}

void disableR(bool orEnable)
{
  digitalWrite(MOT_R_ENB, orEnable);
}

void forwardL(bool orBack)
{
  digitalWrite(MOT_L_DIR, orBack);
}
void forwardR(bool orBack)
{
  digitalWrite(MOT_R_DIR, orBack);
}

void setSpeed(int16_t s, int16_t rotation)
{
  int16_t sL = s - rotation;
  int16_t sR = s + rotation;
  boolean backwardL = sL < 0;
  boolean backwardR = sR < 0;

  if (backwardL)
  {
    forwardL(false);
    sL = -sL;
  }
  else
  {
    forwardL(true);
  }

  if (backwardR)
  {
    forwardR(false);
    sR = -sR;
  }
  else
  {
    forwardR(true);
  }

  disableL(sL < MAX_SPEED / 100);
  disableR(sR < MAX_SPEED / 100);

  if (sL > MAX_SPEED)
    sL = MAX_SPEED;
  if (sR > MAX_SPEED)
    sR = MAX_SPEED;

  // keep track of the position (in steps forward or backward)
  currentPos += ((micros() - prevSpeedStart) / (float)1000000) * prevSpeed;
  prevSpeed = s;
  prevSpeedStart = micros();

  // set the new speed
  ledcWriteTone(MOT_L_CHANNEL, sL);
  ledcWriteTone(MOT_R_CHANNEL, sR);
}

static int MPU_ADDR = 0x69; //AD0 is HIGH

// MPU6050 specific
#define MPU6050_FS_SEL0 3
#define MPU6050_FS_SEL1 4
#define MPU6050_AFS_SEL0 3
#define MPU6050_AFS_SEL1 4

// Combined definitions for the FS_SEL values.eg.  ±250 degrees/second
#define MPU6050_FS_SEL_250 (0)
#define MPU6050_FS_SEL_500 (bit(MPU6050_FS_SEL0))
#define MPU6050_FS_SEL_1000 (bit(MPU6050_FS_SEL1))
#define MPU6050_FS_SEL_2000 (bit(MPU6050_FS_SEL1) | bit(MPU6050_FS_SEL0))

// Combined definitions for the AFS_SEL values
#define MPU6050_AFS_SEL_2G (0)
#define MPU6050_AFS_SEL_4G (bit(MPU6050_AFS_SEL0))
#define MPU6050_AFS_SEL_8G (bit(MPU6050_AFS_SEL1))
#define MPU6050_AFS_SEL_16G (bit(MPU6050_AFS_SEL1) | bit(MPU6050_AFS_SEL0))

// See page 12 & 13 of MPU-6050 datasheet for sensitivities config and corresponding output
#define GYRO_FULL_SCALE_RANGE MPU6050_FS_SEL_250
#define GYRO_SCALE_FACTOR 131 // LSB / (degs per seconds)
#define ACC_FULL_SCALE_RANGE MPU6050_AFS_SEL_4G
#define ACC_SCALE_FACTOR 8192 // LSB / g

static float GYRO_RAW_TO_DEGS = 1.0 / (1000000.0 / PERIOD) / GYRO_SCALE_FACTOR;

int16_t accX, accY, accZ;
int16_t gyroX, gyroY, gyroZ;
int16_t gyroX_calibration, gyroY_calibration, gyroZ_calibration;

void setup_mpu()
{
  Wire.begin();
  Wire.setClock(400000L);

  //By default the MPU-6050 sleeps. So we have to wake it up.
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); //We want to write to the PWR_MGMT_1 register (6B hex)
  Wire.write(0x00); //Set the register bits as 00000000 to activate the gyro
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1B); //We want to write to the GYRO_CONFIG register (1B hex)
  Wire.write(GYRO_FULL_SCALE_RANGE);
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1C); //We want to write to the ACCEL_CONFIG register (1A hex)
  Wire.write(ACC_FULL_SCALE_RANGE);
  Wire.endTransmission();
  //Set some filtering to improve the raw data.
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1A); //We want to write to the CONFIG register (1A hex)
  Wire.write(0x03); //Set the register bits as 00000011 (Set Digital Low Pass Filter to ~43Hz)
  Wire.endTransmission();

  calibrateGyro();
}

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

void getAcceleration(int16_t *x, int16_t *y, int16_t *z)
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission();
  Wire.requestFrom(MPU_ADDR, 6);
  *x = constr((((int16_t)Wire.read()) << 8) | Wire.read(), -ACC_SCALE_FACTOR, ACC_SCALE_FACTOR);
  *y = constr((((int16_t)Wire.read()) << 8) | Wire.read(), -ACC_SCALE_FACTOR, ACC_SCALE_FACTOR);
  *z = constr((((int16_t)Wire.read()) << 8) | Wire.read(), -ACC_SCALE_FACTOR, ACC_SCALE_FACTOR);
}

#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

void getRotation(int16_t *x, int16_t *y, int16_t *z)
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(GYRO_XOUT_H);
  Wire.endTransmission();
  Wire.requestFrom(MPU_ADDR, 6);
  *x = ((((int16_t)Wire.read()) << 8) | Wire.read()) - gyroX_calibration;
  *y = ((((int16_t)Wire.read()) << 8) | Wire.read()) - gyroY_calibration;
  *z = ((((int16_t)Wire.read()) << 8) | Wire.read()) - gyroZ_calibration;
}

void calibrateGyro()
{
  int32_t x, y, z;

  for (int i = 0; i < 500; i++)
  {
    getRotation(&gyroX, &gyroY, &gyroZ);
    x += gyroX;
    y += gyroY;
    z += gyroZ;

    delayMicroseconds(PERIOD); // simulate the main program loop time ??
  }

  gyroX_calibration = x / 500;
  gyroY_calibration = y / 500;
  gyroZ_calibration = z / 500;
}

// on ESP32 Arduino constrain doesn't work
int16_t constr(int16_t value, int16_t mini, int16_t maxi)
{
  if (value < mini)
    return mini;
  else if (value > maxi)
    return maxi;
  return value;
}

float constrf(float value, float mini, float maxi)
{
  if (value < mini)
    return mini;
  else if (value > maxi)
    return maxi;
  return value;
}

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

  setup_mpu();
  setup_motors();
  setup_serial_control();
  setup_wifi();

  loop_timer = micros() + PERIOD;
  print_timer = micros() + PRINT_PERIOD;
}

void loop()
{
  getAcceleration(&accX, &accY, &accZ);
  rollAcc = asin((float)accX / ACC_SCALE_FACTOR) * RAD_TO_DEG;
  pitchAcc = asin((float)accY / ACC_SCALE_FACTOR) * RAD_TO_DEG;

  getRotation(&gyroX, &gyroY, &gyroZ);
  // roll vs pitch depends on how the MPU is installed in the robot
  roll -= gyroY * GYRO_RAW_TO_DEGS;
  pitch += gyroX * GYRO_RAW_TO_DEGS;
  // sin() has to be applied on radians
  //  roll += pitch * sin((float)gyroZ * GYRO_RAW_TO_DEGS * DEG_TO_RAD);
  //  pitch -= roll * sin((float)gyroZ * GYRO_RAW_TO_DEGS * DEG_TO_RAD);

  roll = roll * 0.999 + rollAcc * 0.001;
  pitch = pitch * 0.999 + pitchAcc * 0.001;

  // apply PID algo

  // The selfBalanceAngleSetpoint variable is automatically changed to make sure that the robot stays balanced all the time.
  positionErr = constrf(currentPos / (float)1000, -MAX_CONTROL_OR_POSITION_ERR, MAX_CONTROL_OR_POSITION_ERR);
  serialControlErr = 0;
  if (isValidJoystickValue(joystickY))
  {
    serialControlErr = constrf((joystickY - 130) / (float)15, -MAX_CONTROL_OR_POSITION_ERR, MAX_CONTROL_OR_POSITION_ERR);
    // this control has to change slowly/gradually to avoid shaking the robot
    if (serialControlErr < prevSerialControlErr)
    {
      serialControlErr = max(serialControlErr, prevSerialControlErr - MAX_CONTROL_ERR_INCREMENT);
    }
    else
    {
      serialControlErr = min(serialControlErr, prevSerialControlErr + MAX_CONTROL_ERR_INCREMENT);
    }

    prevSerialControlErr = serialControlErr;
  }

  pidError = pitch - angleSetpoint - selfBalanceAngleSetpoint;
  // either no manual / serial control -> try to keep the position or follow manual control
  if (abs(serialControlErr) > MIN_CONTROL_ERR)
  {
    pidError += serialControlErr > 0 ? serialControlErr - MIN_CONTROL_ERR : serialControlErr + MIN_CONTROL_ERR;
    // re-init position so it doesn't try to go back when getting out of manual control mode
    currentPos = 0;
  }
  else
  {
    pidError += positionErr;
  }

  integralErr = constrf(integralErr + Ki * pidError, -MAX_PID_OUTPUT, MAX_PID_OUTPUT);
  errorDerivative = pidError - pidLastError;

  pidOutput = Kp * pidError + integralErr + Kd * errorDerivative;

  if (pidOutput < 5 && pidOutput > -5)
    pidOutput = 0; //Create a dead-band to stop the motors when the robot is balanced

  if (pitch > 30 || pitch < -30)
  { //If the robot tips over
    pidOutput = 0;
    integralErr = 0;
    selfBalanceAngleSetpoint = 0;
  }

  // store error for next loop
  pidLastError = pidError;

  int16_t rotation = 0;
  if (isValidJoystickValue(joystickX))
  {
    rotation = constrf((float)(joystickX - 130), -MAX_PID_OUTPUT, MAX_PID_OUTPUT) * (MAX_SPEED / MAX_PID_OUTPUT);
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

  setSpeed(constrf(pidOutput, -MAX_PID_OUTPUT, MAX_PID_OUTPUT) * (MAX_SPEED / MAX_PID_OUTPUT), rotation);

  // The angle calculations are tuned for a loop time of PERIOD milliseconds.
  // To make sure every loop is exactly that, a wait loop is created by setting the loop_timer
  if (loop_timer <= micros())
    Serial.println("ERROR loop too short !");
  while (loop_timer > micros())
    ;
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
  message += "<p>Position=" + String(currentPos) + "  <a href=\"update?Pos=inc\"><button>Pos++</button></a><a href=\"update?Pos=dec\"><button>Pos--</button></a></p>";
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
    currentPos = currentPos + (action == "inc" ? 1000 : -1000);
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
