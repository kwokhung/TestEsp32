//#include "SbrXXX.h"
//#include "SbrXXX01.h"
//#include "SbrXXX02.h"
//#include "SbrXXX03.h"
//#include "SbrXXX04.h"
//#include "SbrXXX05.h"
//#include "SbrXXX06.h"
//#include "SbrXXX07.hpp"
//#include "SbrXXX08.hpp"
//#include "SbrXXX09.hpp"
//#include "SbrXXX10.hpp"
//#include "SbrXXX11.hpp"
//#include "SbrXXX12.hpp"
//#include "SbrXXX13.hpp"
//#include "SbrXXX14.hpp"
#include "SbrXXX15.hpp"
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
  //SbrXXX07 *sbrXXX07 = SbrXXX07::getSingleTon("SBR - XXX07");
  //SbrXXX08 *sbrXXX08 = SbrXXX08::getSingleTon("SBR - XXX08");
  //SbrXXX09 *sbrXXX09 = SbrXXX09::getSingleTon("SBR - XXX09");
  //SbrXXX10 *sbrXXX10 = SbrXXX10::getSingleTon("SBR - XXX10");
  //SbrXXX11 *sbrXXX11 = SbrXXX11::getSingleTon("SBR - XXX11");
  //SbrXXX12 *sbrXXX12 = SbrXXX12::getSingleTon("SBR - XXX12");
  //SbrXXX13 *sbrXXX13 = SbrXXX13::getSingleTon("SBR - XXX13");
  //SbrXXX14 *sbrXXX14 = SbrXXX14::getSingleTon("SBR - XXX14");
  SbrXXX15 *sbrXXX15 = SbrXXX15::getSingleTon("SBR - XXX15");
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

  //SbrXXX10::sbrXXX11 = sbrXXX11;

  //xTaskCreate(SbrXXX::startUp, sbrXXX->name.c_str(), 10000, sbrXXX, 1, &sbrXXX->task);
  //xTaskCreate(SbrXXX01::startUp, sbrXXX01->name.c_str(), 10000, sbrXXX01, 1, &sbrXXX01->task);
  //xTaskCreate(SbrXXX02::startUp, sbrXXX02->name.c_str(), 10000, sbrXXX02, 1, &sbrXXX02->task);
  //xTaskCreate(SbrXXX03::startUp, sbrXXX03->name.c_str(), 10000, sbrXXX03, 1, &sbrXXX03->task);
  //xTaskCreate(SbrXXX04::startUp, sbrXXX04->name.c_str(), 10000, sbrXXX04, 1, &sbrXXX04->task);
  //xTaskCreate(SbrXXX05::startUp, sbrXXX05->name.c_str(), 10000, sbrXXX05, 1, &sbrXXX05->task);
  //xTaskCreate(SbrXXX06::startUp, sbrXXX06->name.c_str(), 10000, sbrXXX06, 1, &sbrXXX06->task);
  //xTaskCreate(SbrXXX07::startUp, sbrXXX07->name.c_str(), 10000, sbrXXX07, 1, &sbrXXX07->task);
  //xTaskCreate(SbrXXX08::startUp, sbrXXX08->name.c_str(), 10000, sbrXXX08, 1, &sbrXXX08->task);
  //xTaskCreate(SbrXXX09::startUp, sbrXXX09->name.c_str(), 10000, sbrXXX09, 1, &sbrXXX09->task);
  //xTaskCreate(SbrXXX10::startUp, sbrXXX10->name.c_str(), 10000, sbrXXX10, 1, &sbrXXX10->task);
  //xTaskCreate(SbrXXX11::startUp, sbrXXX11->name.c_str(), 10000, sbrXXX11, 2, &sbrXXX11->task);
  //xTaskCreate(SbrXXX12::startUp, sbrXXX12->name.c_str(), 10000, sbrXXX12, 2, &sbrXXX12->task);
  //xTaskCreate(SbrXXX13::startUp, sbrXXX13->name.c_str(), 10000, sbrXXX13, 2, &sbrXXX13->task);
  //xTaskCreate(SbrXXX14::startUp, sbrXXX14->name.c_str(), 10000, sbrXXX14, 2, &sbrXXX14->task);
  xTaskCreate(SbrXXX15::startUp, sbrXXX15->name.c_str(), 10000, sbrXXX15, 2, &sbrXXX15->task);
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
