// Analog pin.
int sensorPin = 5;
// Temperature variables.
int centigrade = 0;
int fahrenheit = 0;


void setup()
{
  // Start serial communication.
  Serial.begin(9600);
  
}

void loop()
{
  // Converts reading to temperature in centigrade.
  centigrade = ( 5.0 * analogRead(sensorPin) * 100.0) / 1024.0;
  // Converts to temperature in fahrenheit.
  fahrenheit = (centigrade * 9)/ 5 + 32;

  // Sends to serial port.
  Serial.print(analogRead(sensorPin),DEC);
  Serial.print(" reading, ");
  Serial.print(centigrade,DEC);
  Serial.print(" degree celsius, ");
  Serial.print(fahrenheit,DEC);
  Serial.println(" degree fahrenheit.");

  delay(1000);
}
