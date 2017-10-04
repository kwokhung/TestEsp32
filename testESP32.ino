#include "App.h"

App app;

void setup()
{
    app.setup();
}

void loop()
{
    app.getMqtt()->loop();
}
