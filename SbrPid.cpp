#include "SbrPid.h"

SbrPid::SbrPid(std::string name)
    : name(name)
{
    task = NULL;
}

void SbrPid::setup()
{
    Serial.println("SbrPid::setup");

    loop_timer = micros() + PERIOD;
    print_timer = micros() + PRINT_PERIOD;
}

void SbrPid::loop()
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

    if (SbrControl::isValidJoystickValue(SbrControl::joystickY))
    {
        serialControlErr = SbrMpu::constrf((SbrControl::joystickY - 130) / (float)15, -MAX_CONTROL_OR_POSITION_ERR, MAX_CONTROL_OR_POSITION_ERR);

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

    if (SbrControl::isValidJoystickValue(SbrControl::joystickX))
    {
        rotation = SbrMpu::constrf((float)(SbrControl::joystickX - 130), -MAX_PID_OUTPUT, MAX_PID_OUTPUT) * (MAX_SPEED / MAX_PID_OUTPUT);
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

void SbrPid::startUp(void *parameter)
{
    SbrPid *sbrPid = (SbrPid *)parameter;

    sbrPid->setup();

    Serial.println("Starting thread dealing with Wifi/HTTP client requests...");

    while (true)
    {
        sbrPid->loop();
    }

    vTaskDelete(NULL);
}

SbrMpu *SbrPid::sbrMpu;
SbrMotor *SbrPid::sbrMotor;
float SbrPid::BASE_Kp = 100.0, SbrPid::BASE_Ki = 5.0, SbrPid::BASE_Kd = 130.0;
float SbrPid::Kp = SbrPid::BASE_Kp, SbrPid::Ki = SbrPid::BASE_Ki, SbrPid::Kd = SbrPid::BASE_Kd;
float SbrPid::angleSetpoint = 0, SbrPid::selfBalanceAngleSetpoint = 0;
float SbrPid::pidOutput, SbrPid::pidError, SbrPid::pidLastError, SbrPid::integralErr, SbrPid::positionErr, SbrPid::serialControlErr, SbrPid::prevSerialControlErr, SbrPid::errorDerivative;
float SbrPid::MAX_CONTROL_OR_POSITION_ERR = MAX_PID_OUTPUT / SbrPid::Kp;
float SbrPid::MAX_CONTROL_ERR_INCREMENT = SbrPid::MAX_CONTROL_OR_POSITION_ERR / 400;
float SbrPid::roll, SbrPid::pitch, SbrPid::rollAcc, SbrPid::pitchAcc;
uint32_t SbrPid::loop_timer;
uint32_t SbrPid::print_timer;
