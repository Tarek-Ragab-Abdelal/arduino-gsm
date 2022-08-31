#include <SoftwareSerial.h>
SoftwareSerial GSM_SERIAL (10, 11);

// GSM credentials
String apn = "internet.vodafone.net";                                // APN
String apn_u = "internet";                                           // APN-Username
String apn_p = "internet";
//###############################################################################################//
String gsm_send_serial(String command, unsigned long wait = 3000);
String gsm_send_serial(String command, unsigned long wait)
{
  String gsm_response = "";
  int trials = 0;
send:
  trials++;
  Serial.println("Send ->: " + command);
  GSM_SERIAL.println(command);
  long wtimer = millis();
  while (wtimer + wait > millis())
  {
    while (GSM_SERIAL.available())
    {
      gsm_response += (char)GSM_SERIAL.read();
    }
  }
  if ((trials <= 5) && (command == "AT+HTTPREAD") && (gsm_response.length() < 10 || gsm_response.indexOf("ERROR") >= 0))
    goto send;
  Serial.println(gsm_response);
  return gsm_response;
}

//###############################################################################################//

void initizalize_gsm()
{
  gsm_send_serial("AT+SAPBR=3,1,Contype,GPRS");
  gsm_send_serial("AT+SAPBR=3,1,APN," + apn);
  if (apn_u != "")
  {
    gsm_send_serial("AT+SAPBR=3,1,USER," + apn_u);
  }
  if (apn_p != "")
  {
    gsm_send_serial("AT+SAPBR=3,1,PWD," + apn_p);
  }
}

//###############################################################################################//

void reset_gsm(int pin)
{
  digitalWrite(pin, LOW);
  delay(200);
  digitalWrite(pin, HIGH);
  delay(15000);
}

//###############################################################################################//

String gsm_http_post(String url, String content_type, String postdata)
{
  int size = postdata.length();
  gsm_send_serial("AT+SAPBR=1,1");
  gsm_send_serial("AT+SAPBR=2,1");
  gsm_send_serial("AT+HTTPINIT");
  gsm_send_serial("AT+HTTPPARA=CID,1");
  gsm_send_serial("AT+HTTPPARA=URL," + url);
  gsm_send_serial("AT+HTTPPARA=CONTENT," + content_type);
  gsm_send_serial("AT+HTTPDATA=" + String(size) + ",10000");
  gsm_send_serial(postdata);
  gsm_send_serial("AT+HTTPACTION=1");
  String response = gsm_send_serial("AT+HTTPREAD", 10000);
  gsm_send_serial("WAIT=6");
  gsm_send_serial("AT+HTTPTERM");
  gsm_send_serial("AT+SAPBR=0,1");
  return response;
}

//###############################################################################################//

String gsm_http_get(String url)
{
  gsm_send_serial("AT+SAPBR=1,1");
  gsm_send_serial("AT+SAPBR=2,1");
  gsm_send_serial("AT+HTTPINIT");
  gsm_send_serial("AT+HTTPPARA=CID,1");
  gsm_send_serial("AT+HTTPPARA=URL," + url);
  gsm_send_serial("AT+HTTPACTION=0");
  String response = gsm_send_serial("AT+HTTPREAD");
  gsm_send_serial("AT+HTTPTERM");
  gsm_send_serial("AT+SAPBR=0,1");
  return response;
}
//###############################################################################################//
