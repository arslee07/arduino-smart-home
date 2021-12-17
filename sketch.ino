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
  Serial.begin(9600);
}

void loop() {
  readSerialPort();

  if (msg != "") {
    if (getValue(msg, ' ', 0) == "on") {
      digitalWrite(getValue(msg, ' ', 1).toInt(), HIGH);
      Serial.print("200");
    } else if (getValue(msg, ' ', 0) == "off") {
      digitalWrite(getValue(msg, ' ', 1).toInt(), LOW);
      Serial.print("200");
    } else if (getValue(msg, ' ', 0) == "switch") {
      int pin = getValue(msg, ' ', 1).toInt();
      digitalWrite(pin, !digitalRead(pin));
      Serial.print("200");
    } else {
      Serial.print("404");
    }
  }
  
  delay(100);
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
