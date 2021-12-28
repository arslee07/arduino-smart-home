#include "DHT.h"

DHT dht(12, DHT11);
String msg;

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void setup() {
  pinMode(7, OUTPUT);
  pinMode(13, OUTPUT);
  dht.begin();
  Serial.begin(9600);
}


void loop() {
  delay(100);
  readSerialPort();

  if (msg != "") {
    String type = getValue(msg, ' ', 0);
    if (type == "led") {
      String command = getValue(msg, ' ', 1);
      if (command == "on") {
        digitalWrite(getValue(msg, ' ', 2).toInt(), HIGH);
        Serial.print("200");
      } else if (command == "off") {
        digitalWrite(getValue(msg, ' ', 2).toInt(), LOW);
        Serial.print("200");
      } else if (command == "switch") {
        int pin = getValue(msg, ' ', 2).toInt();
        digitalWrite(pin, !digitalRead(pin));
        Serial.print("200");
      } else if (command == "state") {
        String query = digitalRead(getValue(msg, ' ', 2).toInt()) == HIGH ? "1" : "0";
        Serial.print("200|" + query);
      } else Serial.print("404");
    } else if (type == "sensor") {
      String query = getValue(msg, ' ', 1);
      if (query == "humidity") {
        float h = dht.readHumidity();
        if (isnan(h)) {
          Serial.print("500");
        } else {
          Serial.print("200|" + String(h));
        }
      } else if (query == "temperature") {
        float t = dht.readTemperature();
        if (isnan(t)) {
          Serial.print("500");
        } else {
          Serial.print("200|" + String(t));
        }
      } else Serial.print("404");
    } else Serial.print("404");
  }
}


void readSerialPort() {
  msg = "";
  if (Serial.available()) {
    delay(10);
    while (Serial.available() > 0) {
      msg += (char)Serial.read();
    }
    Serial.flush();
  }
}
