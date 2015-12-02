/*  This file is part of HidnSeek.

 HidnSeek is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 HidnSeek is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with HidnSeek.  If not, see <http://www.gnu.org/licenses/>.*/

bool initSigFox() {
  serialString(PSTR("SigFox: "));
  long previousMillis = millis();
  while ((millis() - previousMillis) < 6000) {
    if (SigFox.begin() == 3) {
      Serial.println(millis() - previousMillis);
      Serial.println(SigFox.getID(), HEX);
      return true;
    }
    else delay(200);
  }
  Serial.println("Fail");
  return false;
}

void sendSigFox(byte msgType) {
  // isReady check removed in the library due to reset of millis during sleep time
  if (msgType > 0) {
    if (baromPresent && msgType == MSG_NO_MOTION) bmp180Measure(&p.lat, &p.lon);
    p.cpx &= ~(7 << 0);
    p.cpx |= (uint32_t) (  7 & msgType); // mode (2bits)
  }
  else {
    previous_lat = p.lat;
    previous_lon = p.lon;
  }
  digitalWrite(redLEDpin, HIGH);
  long previousMillis = millis();
  SigFox.send(&p, sizeof(p));
  while ((millis() - previousMillis) < 6000) delay(100);
  digitalWrite(redLEDpin, LOW);
  stepMsg(); // Init the message number per day and time usage counters
}
