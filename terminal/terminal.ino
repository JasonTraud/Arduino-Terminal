char buffer[512];                 // Buffer variables
unsigned int bufferIndex = 0;     // 

unsigned char delimiter = 0x0D;   // Carriage return for our delimiter
char* commandDelimiter = " ,=";   // Breaking up commands, accept a space, a comma, or an equals sign
unsigned char whiteSpace = 0x20;  // White space
bool packetDetected = 0;          // Flag for detecting a packet

String firmwareVersion = "v1.2a"; // Hard coded firmware version

// Our commands
String command_firmwareVersion = "ver";
String command_settingsA = "settingA";
String command_settingsB = "settingB";
String command_taskA = "taskA";
String command_taskB = "taskB";

// Our responses
String response_notRecognized = "NR";
String response_accepted = "A";
String response_error = "E";

// Default settings
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

  // If there isn't an argument, then we're getting the value
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

  // If there isn't an argument, then we're getting the value
  else {
    Serial.println(String(settingB));
  }
}

void taskA_handler() {

  // Toggle
  if (task != TASK_A)
    task = TASK_A;
  else 
    task = IDLE; 
  
  // Debug print
  Serial.println(stateNames[task]);
}

void taskB_handler() {

  // Toggle
  if (task != TASK_B)
    task = TASK_B;
  else 
    task = IDLE; 
  
  // Debug print
  Serial.println(stateNames[task]);
}

void processPacket() {  
  String command, arg1, arg2;

  command = strtok(buffer,commandDelimiter);  // Retrieve our command.
  arg1 = strtok(NULL,commandDelimiter);       // Add more strtok commands 
  arg2 = strtok(NULL,commandDelimiter);       // if more arguments are needed

  // Look for our commands
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
  
  bufferIndex = 0; // Reset our buffer
}

// Add incoming serial to our buffer, 
// raise a flag when the EOL character is found
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

// Our state machine to process our tasks. 
// Typically this would be called based on some timer.
void processTasks() {
  switch(task){
    case TASK_A:
      // perform taskA funcitons     
      // Serial.println("TaskA");
      // delay(100);
      return;
    case TASK_B:
      // perform taskB funcitons      
      // Serial.println("TaskB");
      // delay(100);
      return;
    default:
      // default state should be idle
      return;
  }
}

// Setup our hardware
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
  processTasks();
}