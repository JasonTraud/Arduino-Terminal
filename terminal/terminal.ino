unsigned char buffer[512];        // Buffer variables
unsigned int bufferIndex = 0;     // 

unsigned char delimiter = 0x0D;   // Carriage return for our delimiter
char* commandDelimiter = " ,=";   // Breaking up commands, accept a space, a comma, or an equals sign
unsigned char whiteSpace = 0x20;  // White space
bool packetDetected = 0;          // Flag for detecting a packet

String firmwareVersion = "v1.0a"; // Hard coded firmware version

// Our list of commands
String command_firmwareVersion = "ver";

void processPacket() {

  // Retrieve our command
  String token;
  token = strtok(buffer,commandDelimiter);

  // Look for our stock commands
  if (token.equalsIgnoreCase(command_firmwareVersion)) {
    Serial.println(firmwareVersion);
  }

  // Print "NR" for Not Recognized if the commands are not found
  else {
    Serial.println("NR");
  }

  // Reset our buffer
  bufferIndex = 0;
}

void processIncomingSerial() {
  unsigned char incomingByte;

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    buffer[bufferIndex] = incomingByte;

    if (incomingByte == delimiter) {
      packetDetected = 1;
      buffer[bufferIndex] = 0x00;
    }
      

    bufferIndex ++;
  }
}

// Setup Hardware
void setup() {
  Serial.begin(115200);
}

// Main loop
void loop(){  
  processIncomingSerial();
  if(packetDetected) {
    packetDetected = 0;
    processPacket();
  }
}