int photocellPin = 1;
//define photocellsh=2, read the value of voltage.
int ledPin = 13;
//define ledPin12 is the output port of led’s level.
int val = 0;

//define original of val.
void setup() {
  Serial.begin(9600);      // open the serial port at 9600 bps:    
  Serial.print("Setup");
  pinMode(ledPin, OUTPUT); //set ledPin output
}

void loop() {
  val = analogRead(photocellPin);
  //get the value from sensor, 1024 = 100%/ 5v.
  // witht he proto board, it's beyond 320 = 100%, and darker is higher
  // Serial.println(val);
  if(val >= 180){
    digitalWrite(ledPin, HIGH); //when the value of val is less than 512(2.5V), light up led lamp
    Serial.print(val);
    Serial.print(" light on\n");
  } else {
    digitalWrite(ledPin, LOW);
  }
}
