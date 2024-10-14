#include <SoftwareSerial.h>
#include <TinyGPS++.h>

const int buttonPin = A1;
int buttonState = 0;
SoftwareSerial gpsSerial(10, 11);  // RX, TX for NEO-6M GPS module
SoftwareSerial gsmSerial(7, 8);    // RX, TX for SIM900A GSM module
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  gsmSerial.begin(9600);
  pinMode(buttonPin, INPUT);
}

void loop() {

  buttonState = analogRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.print("\nCollision\n");
    Serial.print("\nLongitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("\nLatitude: ");
    Serial.println(gps.location.lat(), 6);
    // Button is pressed, get GPS data and send via GSM
    String gpsData = getGPSData();
    sendGPSData(gpsData);
  }

  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      // Process GPS data (optional)
    }
  }
}

String getGPSData() {
  String gpsData ="!--Car Accident Detected, Take Action--!" "\nAccident Location: " + String(gps.location.lat(), 6) + ", " + String(gps.location.lng(), 6) + "\n---Driver Details---\nName: Aman Kundal\n\Family Contact No.: +91XXXXXXXXXX";
  return gpsData;
}
void sendGPSData(String data) {
  gsmSerial.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+919021136061\""); // Replace with recipient's phone number
  delay(1000);
  gsmSerial.println(data);
  delay(1000);
  gsmSerial.write(26); // Send Ctrl+Z to indicate the end of SMS
  delay(1000);
}