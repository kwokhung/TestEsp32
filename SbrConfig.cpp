#include "SbrConfig.h"

SbrConfig::SbrConfig(std::string name)
    : name(name)
{
    task = NULL;
}

void SbrConfig::setup()
{
    Serial.println("SbrConfig::setup");

    setup_wifi();
}

void SbrConfig::setup_wifi()
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

    server.onNotFound(SbrConfig::handleNotFound);
    server.on("/", SbrConfig::displayInfo);
    server.on("/update", SbrConfig::updateVars);

    server.begin();
    Serial.println("HTTP server started");

    //xTaskCreatePinnedToCore(wifiLoop, "wifiLoop", 4096, NULL, 1, NULL, xPortGetCoreID());
}

void SbrConfig::startUp(void *parameter)
{
    SbrConfig *sbrConfig = (SbrConfig *)parameter;

    sbrConfig->setup();

    Serial.println("Starting thread dealing with Wifi/HTTP client requests...");

    while (true)
    {
        // deals with the Wifi clients and responds, calls the callbacks, etc. ...
        server.handleClient();

        delay(1);
    }

    vTaskDelete(NULL);
}

void SbrConfig::displayInfo()
{
    Serial.println("Handle Display Info...");

    String message = "<h1>Self balancing robot\n</h1><h2> ";

    message += "<p>Position=" + String(sbrMotor->currentPos) + "  <a href=\"update?Pos=inc\"><button>Pos++</button></a><a href=\"update?Pos=dec\"><button>Pos--</button></a></p>";
    //message += "<p>Kp=" + String(Kp) + "  <a href=\"update?Kp=inc\"><button>Kp++</button></a><a href=\"update?Kp=dec\"><button>Kp--</button></a></p>";
    //message += "<p>Ki=" + String(Ki) + "  <a href=\"update?Ki=inc\"><button>Ki++</button></a><a href=\"update?Ki=dec\"><button>Ki--</button></a></p>";
    //message += "<p>Kd=" + String(Kd) + "  <a href=\"update?Kd=inc\"><button>Kd++</button></a><a href=\"update?Kd=dec\"><button>Kd--</button></a></p>";
    //message += "<p>SP=" + String(angleSetpoint) + "  <a href=\"update?Sp=inc\"><button>SP++</button></a><a href=\"update?Sp=dec\"><button>SP--</button></a></p></h2>";

    server.send(200, "text/html", message);
}

void SbrConfig::handleNotFound()
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

void SbrConfig::updateVars()
{
    // we expect one and only 1 var which is the name of what we need to change, and the value is "inc" or "dec"
    String var = server.argName(0);
    String action = server.arg(0);

    Serial.println("Handle " + action + " to " + var);

    if (var == "Pos")
    {
        sbrMotor->currentPos = sbrMotor->currentPos + (action == "inc" ? 1000 : -1000);
    }
    /*else if (var == "Kp")
    {
        Kp = Kp + (action == "inc" ? BASE_Kp * 0.1 : -BASE_Kp * 0.1);
    }
    else if (var == "Ki")
    {
        Ki = Ki + (action == "inc" ? BASE_Ki * 0.1 : -BASE_Ki * 0.1);
    }
    else if (var == "Kd")
    {
        Kd = Kd + (action == "inc" ? BASE_Kd * 0.1 : -BASE_Kd * 0.1);
    }
    else if (var == "Sp")
    {
        angleSetpoint = angleSetpoint + (action == "inc" ? 0.1 : -0.1);
    }
    else
    {
        Serial.println("UNKNOWN var " + var);
    }
    */

    displayInfo();
}

ESP32WebServer &SbrConfig::server = *new ESP32WebServer(80);
SbrMotor *SbrConfig::sbrMotor;