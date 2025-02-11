#define led 15
int sensor;


void setup() {
  // put your setup code here, to run once:

pinMode(led,OUTPUT);
   Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  sensor = analogRead(34);

  Serial.println(sensor);
  delay(1000);
  if(sensor>=100)
  {
    digitalWrite(led,HIGH);
    delay(100);
  }
  else
  {
    digitalWrite(led,LOW);
  delay(100);
  }

}
