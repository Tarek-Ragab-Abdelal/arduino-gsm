#include <ArduinoJson.h>
#include "gsm_module.h"

//###############################################################################################//
// Server Path
String statusPath = "http://46.101.238.90:1880/status";
// LED Current status
String current_status = "off";
//###############################################################################################//
String CreateReading()
{
  String reading = "";
  StaticJsonDocument<200> doc;
  doc["status"] = current_status;
  serializeJson(doc, reading);
  Serial.print("Data: ");
  Serial.println(reading);
  return reading;
}
//###############################################################################################//

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  GSM_SERIAL.begin(4800);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  delay(10000);
  while (GSM_SERIAL.available())
  {
    Serial.write(GSM_SERIAL.read());
  }
  delay(2000);
  Serial.println(" ------------------ CONFIG GPRS ------------------- ");
  initizalize_gsm();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(" ------------------ Posting Data ------------------ ");
  String body = CreateReading();
  String response = gsm_http_post(statusPath, "application/json", body);
  Serial.println("Response String is:");
  Serial.println(response);
  if (response.indexOf("on") >= 0) {
    Serial.println("Switching to ON");
    digitalWrite(13, HIGH);
    current_status = "on";
  }

  else if (response.indexOf("off") >= 0) {
    Serial.println("Switching to OFF");
    digitalWrite(13, LOW);
    current_status = "off";
  }
  else
    Serial.println("couldn't get the commmand");
  delay(1000);
}
