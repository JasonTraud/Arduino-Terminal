char buffer[512];                 // Buffer variables
unsigned int bufferIndex = 0;     // 

unsigned char delimiter = 0x0D;   // Carriage return for our delimiter
char* commandDelimiter = " ,=";   // Breaking up commands, accept a space, a comma, or an equals sign
unsigned char whiteSpace = 0x20;  // White space
bool packetDetected = 0;          // Flag for detecting a packet

String firmwareVersion = "v1.1a"; // Hard coded firmware version

// Our list of commands
String command_firmwareVersion = "ver";
String command_settingsA = "settingA";
String command_settingsB = "settingB";
String command_taskA = "taskA";
String command_taskB = "taskB";

// Our list of responses
String response_notRecognized = "NR";
String response_accepted = "A";
String response_error = "E";

unsigned int settingA = 5;
unsigned int settingB = 5;

// Our list of tasks
enum Tasks { IDLE, TASK_A, TASK_B } task;
#define IDNAME(name) #name
const char* stateNames[] = {IDNAME(IDLE), IDNAME(TASK_A), IDNAME(TASK_B)};

void settingA_handler(String arg1, String arg2) {
  unsigned int temp_settingA = settingA;
  
  // If there's an argument, attempt to set it
  if (arg1 != NULL) {
    temp_settingA = atoi(arg1.c_str());
    
    if ((temp_settingA > 0) &&  (temp_settingA <= 10)){
      settingA = temp_settingA;
      Serial.println(response_accepted);
    } else {
      Serial.println(response_error);
    }    
  }

  // If there isn't an argument, then we're getting
  else {
    Serial.println(String(settingA));
  }
}

void settingB_handler(String arg1, String arg2) {
  unsigned int temp_settingB = settingB;
  
  // If there's an argument, attempt to set it
  if (arg1 != NULL) {
    temp_settingB = atoi(arg1.c_str());
    
    if ((temp_settingB > 0) &&  (temp_settingB <= 10)){
      settingB = temp_settingB;
      Serial.println(response_accepted);
    } else {
      Serial.println(response_error);
    }    
  }

  // If there isn't an argument, then we're getting
  else {
    Serial.println(String(settingB));
  }
}

void taskA_handler() {

  Serial.println("TaskA");
  Serial.println(stateNames[task]);  

  if (task == IDLE)
    task = TASK_A;
  else 
    task = IDLE; 
  
  Serial.println(stateNames[task]);
}

void taskB_handler() {
  Serial.println("TaskB");
  Serial.println(stateNames[task]);  

  if (task == IDLE)
    task = TASK_B;
  else 
    task = IDLE; 
  
  Serial.println(stateNames[task]);
}

void processPacket() {

  // Retrieve our command
  String command, arg1, arg2;

  command = strtok(buffer,commandDelimiter);
  arg1 = strtok(NULL,commandDelimiter);
  arg2 = strtok(NULL,commandDelimiter);

  // Look for our stock commands
  if (command.equalsIgnoreCase(command_firmwareVersion)) {
    Serial.println(firmwareVersion);
  } 
  
  else if (command.equalsIgnoreCase(command_settingsA)) {
    settingA_handler(arg1, arg2);
  } 
  
  else if (command.equalsIgnoreCase(command_settingsB)) {
    settingB_handler(arg1, arg2);
  } 
  
  else if (command.equalsIgnoreCase(command_taskA)) {
    taskA_handler();
  } 
  
  else if (command.equalsIgnoreCase(command_taskB)) {
    taskB_handler();
  }

  // Print command not recognized response
  else {
    Serial.println(response_notRecognized);
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
  task = IDLE;  
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