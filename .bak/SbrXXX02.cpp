#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <esp_log.h>

#include "SbrXXX02.h"

uint8_t SbrXXX02::readMPU6050(uint8_t reg)
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(MPU6050_ADDR, 1 /*length*/);

    return Wire.read();
}

void SbrXXX02::writeMPU6050(uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}

void SbrXXX02::readRawData()
{                                         //Subroutine for reading the raw gyro and accelerometer data
    Wire.beginTransmission(MPU6050_ADDR); //Start communicating with the MPU-6050
    Wire.write(0x3B);                     //Send the requested starting register
    Wire.endTransmission();               //End the transmission
    Wire.requestFrom(MPU6050_ADDR, 14);   //Request 14 bytes from the MPU-6050

    while (Wire.available() < 14)
    {
    }

    acc_x = Wire.read() << 8 | Wire.read();       //Add the low and high byte to the acc_x variable
    acc_y = Wire.read() << 8 | Wire.read();       //Add the low and high byte to the acc_y variable
    acc_z = Wire.read() << 8 | Wire.read();       //Add the low and high byte to the acc_z variable
    temperature = Wire.read() << 8 | Wire.read(); //Add the low and high byte to the temperature variable
    gyro_x = Wire.read() << 8 | Wire.read();      //Add the low and high byte to the gyro_x variable
    gyro_y = Wire.read() << 8 | Wire.read();      //Add the low and high byte to the gyro_y variable
    gyro_z = Wire.read() << 8 | Wire.read();      //Add the low and high byte to the gyro_z variable
}

void SbrXXX02::calcRotation()
{
}

void SbrXXX02::setup()
{
    Serial.println("SbrXXX02::setup");

    queueSize = 10;
    queue = xQueueCreate(queueSize, sizeof(int));

    Wire.begin(SDAPIN, SCLPIN);

    writeMPU6050(MPU6050_PWR_MGMT_1, 0x00);

    while (readMPU6050(MPU6050_WHO_AM_I) != MPU6050_ADDR)
    {
        Serial.println("\nWHO_AM_I error.");

        sleepAWhile(100);
    }

    writeMPU6050(MPU6050_GYRO_CONFIG, 0x08);
    writeMPU6050(MPU6050_ACCEL_CONFIG, 0x10);

    Serial.println("Calibration");

    for (int i = 0; i < 2000; i++)
    {
        if (i % 125 == 0)
        {
            Serial.print(".");
        }

        readRawData();

        delayMicroseconds(3); //Delay 3us to simulate the 250Hz program loop
    }

    gyro_x_cal /= 2000; //Divide the gyro_x_cal variable by 2000 to get the avarage offset
    gyro_y_cal /= 2000; //Divide the gyro_y_cal variable by 2000 to get the avarage offset
    gyro_z_cal /= 2000; //Divide the gyro_z_cal variable by 2000 to get the avarage offset

    loop_timer = micros();
}

void SbrXXX02::loop()
{
    Serial.println("SbrXXX02::loop");

    readRawData();

    gyro_x -= gyro_x_cal; //Subtract the offset calibration value from the raw gyro_x value
    gyro_y -= gyro_y_cal; //Subtract the offset calibration value from the raw gyro_y value
    gyro_z -= gyro_z_cal; //Subtract the offset calibration value from the raw gyro_z value

    //Gyro angle calculations
    //0.0000611 = 1 / (250Hz / 65.5)
    angle_pitch += gyro_x * 0.0000611; //Calculate the traveled pitch angle and add this to the angle_pitch variable
    angle_roll += gyro_y * 0.0000611;  //Calculate the traveled roll angle and add this to the angle_roll variable

    //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
    angle_pitch += angle_roll * sin(gyro_z * 0.000001066); //If the IMU has yawed transfer the roll angle to the pitch angel
    angle_roll -= angle_pitch * sin(gyro_z * 0.000001066); //If the IMU has yawed transfer the pitch angle to the roll angel

    //Accelerometer angle calculations
    acc_total_vector = sqrt((acc_x * acc_x) + (acc_y * acc_y) + (acc_z * acc_z)); //Calculate the total accelerometer vector
    //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
    angle_pitch_acc = asin((float)acc_y / acc_total_vector) * 57.296; //Calculate the pitch angle
    angle_roll_acc = asin((float)acc_x / acc_total_vector) * -57.296; //Calculate the roll angle

    //Place the MPU-6050 spirit level and note the values in the following two lines for calibration
    angle_pitch_acc -= 0.0; //Accelerometer calibration value for pitch
    angle_roll_acc -= 0.0;  //Accelerometer calibration value for roll

    if (set_gyro_angles)
    {                                                                  //If the IMU is already started
        angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004; //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
        angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;    //Correct the drift of the gyro roll angle with the accelerometer roll angle
    }
    else
    {                                  //At first start
        angle_pitch = angle_pitch_acc; //Set the gyro pitch angle equal to the accelerometer pitch angle
        angle_roll = angle_roll_acc;   //Set the gyro roll angle equal to the accelerometer roll angle
        set_gyro_angles = true;        //Set the IMU started flag
    }

    //To dampen the pitch and roll angles a complementary filter is used
    angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1; //Take 90% of the output pitch value and add 10% of the raw pitch value
    angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1;    //Take 90% of the output roll value and add 10% of the raw roll value

    //write_LCD(); //Write the roll and pitch values to the LCD display
    Serial.print("angle_pitch : ");
    Serial.println(angle_pitch_output);
    Serial.print("angle_roll : ");
    Serial.println(angle_roll_output);

    while (micros() - loop_timer < 4000)
    {
        esp_task_wdt_reset(); //Wait until the loop_timer reaches 4000us (250Hz) before starting the next loop
    }

    loop_timer = micros();
    //sleepAWhile(1000);
}
