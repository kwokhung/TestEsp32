#include <IPAddress.h>
#include <SSD1306.h>

#include "OLed.h"

OLed::OLed(int address, int sdaPin, int sclPin)
    : display(new SSD1306(address, sdaPin, sclPin)),
      qrcode(new QRcode(display))
{
}

void OLed::setup()
{
    display->init();
    display->clear();
    display->display();

    //qrcode.debug();
    qrcode->init();
}

void OLed::hello()
{
    display->clear();
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->drawString(64, 28, "hello");
    display->display();
}

void OLed::connecting(String ssid, String password, int percentage)
{
    display->clear();
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->drawString(64, 8, ssid);
    display->drawString(64, 28, password);
    display->drawProgressBar(10, 48, 108, 12, percentage);
    display->display();
}

void OLed::beAccessPoint(String mySsid, String myPassword, IPAddress *myIP)
{
    /*String connectInfo = mySsid + "/" + myPassword + "/" + myIP->toString();

    qrcode->create(connectInfo);*/
    display->clear();
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->drawString(64, 8, "SSID: " + mySsid);
    display->drawString(64, 28, "Password: " + myPassword);
    display->drawString(64, 48, "IP: " + myIP->toString());
    display->display();
}

void OLed::connected(String ssid, String localIP)
{
    String connectInfo = ssid + "/" + localIP;

    qrcode->create(connectInfo);
    /*display->clear();
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->drawString(64, 8, F("Connected"));
    display->drawString(64, 28, ssid);
    display->drawString(64, 48, localIP);
    display->display();*/
}

void OLed::reset(String newSsid, String newPassword)
{
    display->clear();
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->drawString(64, 8, "SSID: " + newSsid);
    display->drawString(64, 28, "Password: " + newPassword);
    display->drawString(64, 48, "Reset");
    display->display();
}
