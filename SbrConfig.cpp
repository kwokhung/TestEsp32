#include <esp_task_wdt.h>

#include "SbrConfig.h"

void SbrConfig::setup()
{
    Serial.println("SbrConfig::setup");

    Serial.println("\nConnecting to Wifi");
    WiFi.begin("MASON-IT", "22182830");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.print("\nWiFi connected with IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", SbrConfig::displayInfo);

    server.on("/update", []() {
        String var = server.argName(0);
        String action = server.arg(0);

        Serial.println("Handle " + action + " to " + var);

        if (var == "Pos")
        {
            sbrMotor->currentPos = sbrMotor->currentPos + (action == "inc" ? 1000 : -1000);
        }
        else if (var == "Kp")
        {
            sbrPid->Kp = sbrPid->Kp + (action == "inc" ? sbrPid->BASE_Kp * 0.1 : -sbrPid->BASE_Kp * 0.1);
        }
        else if (var == "Ki")
        {
            sbrPid->Ki = sbrPid->Ki + (action == "inc" ? sbrPid->BASE_Ki * 0.1 : -sbrPid->BASE_Ki * 0.1);
        }
        else if (var == "Kd")
        {
            sbrPid->Kd = sbrPid->Kd + (action == "inc" ? sbrPid->BASE_Kd * 0.1 : -sbrPid->BASE_Kd * 0.1);
        }
        else if (var == "Sp")
        {
            sbrPid->angleSetpoint = sbrPid->angleSetpoint + (action == "inc" ? 0.1 : -0.1);
        }
        else
        {
            Serial.println("UNKNOWN var " + var);
        }

        displayInfo();
    });

    server.onNotFound([]() {
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
    });

    Serial.println("HTTP server started");
    server.begin();
}

void SbrConfig::loop()
{
    Serial.println("SbrConfig::loop");

    server.handleClient();

    //sleepAWhile(1);
    sleepAWhile(1000);
}

void SbrConfig::displayInfo()
{
    Serial.println("Handle Display Info...");

    String message = "<h1>Self balancing robot\n</h1><h2> ";

    message += "<p>Position=" + String(sbrMotor->currentPos) + "  <a href=\"update?Pos=inc\"><button>Pos++</button></a><a href=\"update?Pos=dec\"><button>Pos--</button></a></p>";
    message += "<p>Kp=" + String(sbrPid->Kp) + "  <a href=\"update?Kp=inc\"><button>Kp++</button></a><a href=\"update?Kp=dec\"><button>Kp--</button></a></p>";
    message += "<p>Ki=" + String(sbrPid->Ki) + "  <a href=\"update?Ki=inc\"><button>Ki++</button></a><a href=\"update?Ki=dec\"><button>Ki--</button></a></p>";
    message += "<p>Kd=" + String(sbrPid->Kd) + "  <a href=\"update?Kd=inc\"><button>Kd++</button></a><a href=\"update?Kd=dec\"><button>Kd--</button></a></p>";
    message += "<p>SP=" + String(sbrPid->angleSetpoint) + "  <a href=\"update?Sp=inc\"><button>SP++</button></a><a href=\"update?Sp=dec\"><button>SP--</button></a></p></h2>";

    server.send(200, "text/html", message);
}

ESP32WebServer &SbrConfig::server = *new ESP32WebServer(80);
SbrMotor *SbrConfig::sbrMotor;
SbrPid *SbrConfig::sbrPid;
