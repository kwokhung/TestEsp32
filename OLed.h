#ifndef OLed_h
#define OLed_h

#include <IPAddress.h>
#include <OLEDDisplay.h>
#include <qrcode.h>

class OLed
{
public:
  OLed(int address, int sdaPin, int sclPin);

  void setup();
  void hello();
  void connecting(String ssid, String password, int percentage);
  void beAccessPoint(String mySsid, String myPassword, IPAddress *myIP);
  void connected(String ssid, String localIP);
  void reset(String newSsid, String newPassword);

private:
  OLEDDisplay *display;
  QRcode *qrcode;
};

#endif