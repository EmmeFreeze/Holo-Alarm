/*
HC-06 Module Configurator
This Arduino Code Configures the Baudrate, name and PIN to a HC-06 Bluetooth module on Serial port 0
By: Lucas Teske
*/

void setup() { 
 //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.write("AT+PIN0000");   //<- SCRIVI IL COMANDO QUI
  delay(500);
 
} 


void loop() { 

} 
