# Arduino-Terminal
Basic terminal example implemented on an Arduino. Get and set various registers through any terminal program such as RealTerm or TerraTerm. Baud rate is set to 115,200.

Adds incoming data over the USB serial port to a buffer and watches for a carriage return to determine when to process the information. 

- **ver**: prints the firmware version to the console
- **settingA**: First settings register. Accepts one argument between 0 and 10. 
- **settingB**: Second settings register. Accepts one argument between 0 and 10. 
- **TaskA**: First task
- **TaskB**: Second task

## Settings
No argugument being sent is a GET command. Which means that the Arduino will respond to the terminal with whatever value is currently in memory for that setting. 

One argument for the setting will overwrite the current value and print one of the responses below. 
- **NR**: Not Recognized
- **A**: Accepted
- **E**: Error

## Tasks
Sending the **TASK** command will toggle to that task and process whatever code you have in the **processTasks()** function. By toggling, I mean if the current state is **IDLE** and you send **TaskA** then it will begin processing **TaskA**. If the current state is **TaskA** and you send **TaskA** then it will return to **IDLE**. You can switch directly from **TaskA** to **TaskB** without returning to **IDLE**.

Typically, this state machine will be controlled by some timer so it'll update every 10ms or 100ms but to keep this example simple it's simply called in the main loop. 