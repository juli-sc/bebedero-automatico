int trigPin =22; 
int echoPin = 23;
int tiempo=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
tiempo= pulseIn (echoPin, HIGH);
delay(25);
digitalWrite(trigPin, LOW);
Serial.println(tiempo);
}
