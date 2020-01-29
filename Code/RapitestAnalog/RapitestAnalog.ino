void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  float A = analogRead(A0);
  A = map(A,0,10,0,1023);
  A = A / 306.9;
  Serial.println(A);
  delay(100);
  // put your main code here, to run repeatedly:

}
