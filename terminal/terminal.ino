unsigned char buffer[512];
unsigned int bufferIndex = 0;

unsigned char delimiter = 0x0D;
bool packetDetected = 0;

void processPacket() {
  unsigned int index = 0;

  while(index < bufferIndex){
    Serial.print((char)buffer[index]);
    delay(10);
    index++;
  }

  bufferIndex = 0;
}

void processIncomingSerial() {
  unsigned char incomingByte;

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    buffer[bufferIndex] = incomingByte;

    if (incomingByte == delimiter)
      packetDetected = 1;

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