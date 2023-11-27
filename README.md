# BSafeHelmet

## Completed Work

### Blind-Spot/Crash Detection
For the Pre-Alpha build, the persistent state of polling the ultra-sonic sensor for objects has been developed. The program is able to detect when an object is in range of the sensor. Additionally, we have interfaced with an IMU to grab accelerometer and gyroscope data. This exact accelerometer will need to be tested in the near future to determine if it has high enough quality for the finished product. Lastly, we have a RTOS to run the future program on. All we need to do is add proper threads.

### App
For the project prototype milestone, we focused on two things: updating the UI and implementing functional bluetooth connectivity. For the UI, we first sought to have the map display encompass the entire screen, except with a navigation bar at the bottom of the screen (positioned over the map). The navbar contains three buttons, each of which display to the user a different window depending on which was clicked. The bluetooth button and corresponding screen attempts to locate a bluetooth signal, though we have not yet succeeded in detecting an actual signal. The map page (which is displayed by default) allows users to scroll and zoom freely, and displays a POI marker at the location provided. The profile page has a text input box for users to input their phone number so that it may be used to call emergency services in the event of a crash.

### Bluetooth Communication
The HC05 bluetooth module can send/recieve information over a serial port on a PC. This will be insufficient for our project so we ordered a more sophisticated bluetooth communication module.


## Project Architecture

TIVA Launchpad (main programs and data parsing)
- Ultrasonic Sensors to detect vehicles and objects in blindspots
- Lights to turn on when blindspot trigger met
- Accelerometer to aid in detecting crashes
- Gyroscope to aid in detecting crashes
  
Bluetooth Module (bridge communication between mobile app and helmet hardware)
- Send crash data to mobile phone to trigger emergency response
- Receive audio data from phone and play through helmet speakers

Mobile App (pairs to helmet via bluetooth)
- Allows user to connect phone audio to helmet speakers for music
- Tracks live location of user via GPS modules in phone
- If crash detected, calls emergency response teams for help using user location


## Known Bugs
### Blind-Spot/Crash Detection
- Currently the math for how far away the object is has not been developed properly. It repeatedly says "Distance: 0" regardless of how far away the detected object is. We will work on fixing that for the next milestone.
- Crash Detection has not been fully tested yet, false negatives/positives exist

### App
- GPS does not scrape real-time location. Uses placeholder coordinates for now.
- Map borders are not properly scaling to the size of the phone screen.
- Navbar placement is not static, and is displayed in awkward locations for the bluetooth and profile pages.
- Button icons not displaying properly, using text as placeholder in the meantime
- The bluetooth library (Bleak) is causing errors/crashes when attempting to detect nearby bluetooth signals.

### Bluetooth communication
- The HC05 is meant for simple bluetooth over serial and cannot recieve reasonable audio quality over bluetooth, nor can it send regular data over bluetooth to remedy this an ESP32 will be used to do all the audio/bluetooth processing.
- HC05 does not have any working code in this github as of 11/5, and the code included in main3.c has an unknown bug (clock speed?). It does however work in Keil uVision with their packages and header files. 
