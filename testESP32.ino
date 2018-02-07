#include "SbrMpu.h"
#include "SbrMotor.h"
#include "SbrControl.h"
#include "SbrPid.h"
#include "SbrConfig.h"

SbrMpu *sbrMpu = new SbrMpu("Self balancing Robot - MPU");
SbrMotor *sbrMotor = new SbrMotor("Self balancing Robot - Motor");
SbrControl *sbrControl = new SbrControl("Self balancing Robot - Control");
SbrPid *sbrPid = new SbrPid("Self balancing Robot - PID", sbrMpu, sbrMotor);
SbrConfig *sbrConfig = new SbrConfig("Self balancing Robot - Config", sbrMotor);

void setup()
{
  Serial.begin(500000);

  xTaskCreate(SbrMpu::startUp, "SBR - MPU", 10000, sbrMpu, 1, &sbrMpu->task);
  xTaskCreate(SbrMotor::startUp, "SBR - Motor", 10000, sbrMotor, 1, &sbrMotor->task);
  xTaskCreate(SbrControl::startUp, "SBR - Control", 10000, sbrControl, 2, &sbrControl->task);
  xTaskCreate(SbrPid::startUp, "SBR - PID", 10000, sbrPid, 2, &sbrPid->task);
  xTaskCreate(SbrConfig::startUp, "SBR - Config", 10000, sbrConfig, 1, &sbrConfig->task);
}

void loop()
{
}
