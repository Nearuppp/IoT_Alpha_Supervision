#define debugSerial SerialUSB

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

int getvoltage(){
  // put your main code here, to run repeatedly:
    int sensorValue = analogRead(A0);
    float voltage = map(sensorValue, 0, 1023, 0, 5);
    int output = (voltage >= 5) ? 1 : 0; // if voltage >= 5V, output = 1, else output = 0
    SerialUSB.print("Est allume ?:");
    SerialUSB.println(output);

    return output;
}

void loop() {
  // put your main code here, to run repeatedly:
  getvoltage();
  delay(1000);
}
