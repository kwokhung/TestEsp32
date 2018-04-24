//#include "SbrXXX.h"
//#include "SbrXXX01.h"
//#include "SbrXXX02.h"
//#include "SbrXXX03.h"
//#include "SbrXXX04.h"
//#include "SbrXXX05.h"
//#include "SbrXXX06.h"
//#include "SbrYYY.h"
//#include "SbrZZZ.h"
//#include "SbrMpu.h"
//#include "SbrMotor.h"
//#include "SbrControl.h"
//#include "SbrPid.h"
//#include "SbrConfig.h"

void setup()
{
  Serial.begin(115200);

  //SbrXXX *sbrXXX = SbrXXX::getSingleTon("SBR - XXX");
  //SbrXXX01 *sbrXXX01 = SbrXXX01::getSingleTon("SBR - XXX01");
  //SbrXXX02 *sbrXXX02 = SbrXXX02::getSingleTon("SBR - XXX02");
  //SbrXXX03 *sbrXXX03 = SbrXXX03::getSingleTon("SBR - XXX03");
  //SbrXXX04 *sbrXXX04 = SbrXXX04::getSingleTon("SBR - XXX04");
  //SbrXXX05 *sbrXXX05 = SbrXXX05::getSingleTon("SBR - XXX05");
  //SbrXXX06 *sbrXXX06 = SbrXXX06::getSingleTon("SBR - XXX06");
  //SbrYYY *sbrYYY = SbrYYY::getSingleTon("SBR - YYY");
  //SbrZZZ *sbrZZZ = SbrZZZ::getSingleTon("SBR - ZZZ");
  //SbrMpu *sbrMpu = SbrMpu::getSingleTon("SBR - MPU");
  //SbrMotor *sbrMotor = SbrMotor::getSingleTon("SBR - Motor");
  //SbrControl *sbrControl = SbrControl::getSingleTon("SBR - Control");
  //SbrPid *sbrPid = SbrPid::getSingleTon("SBR - PID");
  //SbrConfig *sbrConfig = SbrConfig::getSingleTon("SBR - Config");

  //SbrZZZ::sbrXXX = sbrXXX;

  //SbrPid::sbrMpu = sbrMpu;
  //SbrPid::sbrMotor = sbrMotor;

  //SbrConfig::sbrMotor = sbrMotor;
  //SbrConfig::sbrPid = sbrPid;

  //xTaskCreate(SbrXXX::startUp, sbrXXX->name.c_str(), 10000, sbrXXX, 1, &sbrXXX->task);
  //xTaskCreate(SbrXXX01::startUp, sbrXXX01->name.c_str(), 10000, sbrXXX01, 1, &sbrXXX01->task);
  //xTaskCreate(SbrXXX02::startUp, sbrXXX02->name.c_str(), 10000, sbrXXX02, 1, &sbrXXX02->task);
  //xTaskCreate(SbrXXX03::startUp, sbrXXX03->name.c_str(), 10000, sbrXXX03, 1, &sbrXXX03->task);
  //xTaskCreate(SbrXXX04::startUp, sbrXXX04->name.c_str(), 10000, sbrXXX04, 1, &sbrXXX04->task);
  //xTaskCreate(SbrXXX05::startUp, sbrXXX05->name.c_str(), 10000, sbrXXX05, 1, &sbrXXX05->task);
  //xTaskCreate(SbrXXX06::startUp, sbrXXX06->name.c_str(), 10000, sbrXXX06, 1, &sbrXXX06->task);
  //xTaskCreate(SbrYYY::startUp, sbrYYY->name.c_str(), 10000, sbrYYY, 1, &sbrYYY->task);
  //xTaskCreate(SbrZZZ::startUp, sbrZZZ->name.c_str(), 10000, sbrZZZ, 1, &sbrZZZ->task);
  //xTaskCreate(SbrMpu::startUp, sbrMpu->name.c_str(), 10000, sbrMpu, 1, &sbrMpu->task);
  //xTaskCreate(SbrMotor::startUp, sbrMotor->name.c_str(), 10000, sbrMotor, 1, &sbrMotor->task);
  //xTaskCreate(SbrControl::startUp, sbrControl->name.c_str(), 10000, sbrControl, 2, &sbrControl->task);
  //xTaskCreate(SbrPid::startUp, sbrPid->name.c_str(), 10000, sbrPid, 2, &sbrPid->task);
  //xTaskCreate(SbrConfig::startUp, sbrConfig->name.c_str(), 10000, sbrConfig, 2, &sbrConfig->task);
}

void loop()
{
}
