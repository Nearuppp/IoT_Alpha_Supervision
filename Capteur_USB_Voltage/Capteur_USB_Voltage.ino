void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  float Vin = ((float)analogRead(A0)/1024)*5;
  Serial.println(Vin);
  delay(1000);
}
