#include <time.h>
#include <sys/time.h>

#include "SbrMpu.h"
#include "SbrMotor.h"
#include "SbrControl.h"
#include "SbrPid.h"
#include "SbrConfig.h"

SbrMpu *sbrMpu = SbrMpu::getSingleTon("SBR - MPU");
SbrMotor *sbrMotor = SbrMotor::getSingleTon("SBR - Motor");
SbrControl *sbrControl = SbrControl::getSingleTon("SBR - Control");
SbrPid *sbrPid = SbrPid::getSingleTon("SBR - PID");
SbrConfig *sbrConfig = SbrConfig::getSingleTon("SBR - Config");

#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif

uint8_t temp_farenheit;
float temp_celsius;
char strftime_buf[64];
time_t now = 0;
struct tm timeinfo;
char buf[256];

void setup()
{
  Serial.begin(115200);
  tzset();

  SbrPid::sbrMpu = sbrMpu;
  SbrPid::sbrMotor = sbrMotor;

  SbrConfig::sbrMotor = sbrMotor;
  SbrConfig::sbrPid = sbrPid;

  xTaskCreate(SbrMpu::startUp, sbrMpu->name.c_str(), 10000, sbrMpu, 1, &sbrMpu->task);
  xTaskCreate(SbrMotor::startUp, sbrMotor->name.c_str(), 10000, sbrMotor, 1, &sbrMotor->task);
  xTaskCreate(SbrControl::startUp, sbrControl->name.c_str(), 10000, sbrControl, 2, &sbrControl->task);
  xTaskCreate(SbrPid::startUp, sbrPid->name.c_str(), 10000, sbrPid, 2, &sbrPid->task);
  xTaskCreate(SbrConfig::startUp, sbrConfig->name.c_str(), 10000, sbrConfig, 1, &sbrConfig->task);
}

void loop()
{
  localtime_r(&now, &timeinfo);
  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
  sprintf(buf, "scan start %02d:%02d:%02d ", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  Serial.print(buf);

  temp_farenheit = temprature_sens_read();
  temp_celsius = (temp_farenheit - 32) / 1.8;
  Serial.print("Temp onBoard ");
  Serial.print(temp_farenheit);
  Serial.print("°F ");
  Serial.print(temp_celsius);
  Serial.println("°C");
  delay(60000);
  now++;
}
