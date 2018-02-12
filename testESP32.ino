#include "SbrXXX.h"
#include "SbrYYY.h"
#include "SbrMpu.h"
#include "SbrMotor.h"
#include "SbrControl.h"
#include "SbrPid.h"
#include "SbrConfig.h"

SbrXXX *sbrXXX = SbrXXX::getSingleTon("SBR - XXX");
SbrYYY *sbrYYY = SbrYYY::getSingleTon("SBR - YYY");
SbrMpu *sbrMpu = SbrMpu::getSingleTon("SBR - MPU");
SbrMotor *sbrMotor = SbrMotor::getSingleTon("SBR - Motor");
SbrControl *sbrControl = SbrControl::getSingleTon("SBR - Control");
SbrPid *sbrPid = SbrPid::getSingleTon("SBR - PID");
SbrConfig *sbrConfig = SbrConfig::getSingleTon("SBR - Config");

void setup()
{
  Serial.begin(115200);

  SbrPid::sbrMpu = sbrMpu;
  SbrPid::sbrMotor = sbrMotor;

  SbrConfig::sbrMotor = sbrMotor;
  SbrConfig::sbrPid = sbrPid;

  xTaskCreate(SbrXXX::startUp, sbrXXX->name.c_str(), 10000, sbrXXX, 1, &sbrXXX->task);
  xTaskCreate(SbrMpu::startUp, sbrMpu->name.c_str(), 10000, sbrMpu, 1, &sbrMpu->task);
  //xTaskCreate(SbrMotor::startUp, sbrMotor->name.c_str(), 10000, sbrMotor, 1, &sbrMotor->task);
  xTaskCreate(SbrControl::startUp, sbrControl->name.c_str(), 10000, sbrControl, 2, &sbrControl->task);
  xTaskCreate(SbrPid::startUp, sbrPid->name.c_str(), 10000, sbrPid, 2, &sbrPid->task);
  xTaskCreate(SbrConfig::startUp, sbrConfig->name.c_str(), 10000, sbrConfig, 1, &sbrConfig->task);
  xTaskCreate(SbrYYY::startUp, sbrYYY->name.c_str(), 10000, sbrYYY, 1, &sbrYYY->task);
}

void loop()
{
}
