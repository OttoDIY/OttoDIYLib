// Define the pin to which the photoresistor is connected
const int photoresistorPin = A0;
int voltage_intensity = 0;
int light_perc = 0;
int sensorValue = 0;

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);
}

void loop() {
  // Read the analog value from the photoresistor
  sensorValue = 1023. - analogRead(photoresistorPin);
  voltage_intensity = (5.0/1023.) * sensorValue;
  light_perc = (voltage_intensity/5.0)*100.0; //converting values to percentages

  // Print the sensor value to the Serial Monitor
  Serial.print("Light Percentage: ");
  Serial.print(light_perc);
  Serial.println("%");

  // Wait for a short delay
  delay(500);
}
