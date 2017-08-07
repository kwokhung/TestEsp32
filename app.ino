// ESP32 Touch Test
// Just test touch pin - Touch0 is T0 which is on GPIO 4.

void setup()
{
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Test");
}

void loop()
{
  Serial.println(T0);
  Serial.println(T1);
  Serial.println(T2);
  Serial.println(T3);
  Serial.println(touchRead(T0));  // get value using T0
  Serial.println(touchRead(T1));  // get value using T1
  Serial.println(touchRead(T2));  // get value using T2
  Serial.println(touchRead(T3));  // get value using T3
  delay(1000);
}
