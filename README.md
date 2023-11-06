# BSafeHelmet

## Completed Work

### Blind-Spot/Crash Detection
For the Pre-Alpha build, the persistent state of polling the ultra-sonic sensor for objects has been developed. The program is able to detect when an object is in range of the sensor. Additionally, we have interfaced with an IMU to grab accelerometer and gyroscope data. This exact accelerometer will need to be tested in the near future to determine if it has high enough quality for the finished product. Lastly, we have a RTOS to run the future program on. All we need to do is add proper threads.

### App
After researching different methods of app building, we decided to build our app using the Beeware framework, and first completed some tutorial work to familiarize ourselves with how it operates. We then created a simple UI design for our app homepage to visualize what the app design will look like. We were able to display a real map at a specified GPS location, in addition to a mock up of a nav bar and a button to connect to a helmet via bluetooth. We are also in the process of implementing bluetooth into the app, though it does not yet successfully detect any devices emitting a bluetooth signal.

### Bluetooth Communication
The HC05 bluetooth module can send/recieve information over a serial port on a PC.


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
- Map has small watermark tab at the bottom and zoom buttons that shouldn't be there.
- The navbar is not displaying the title as intended (the navigation button should appear to the left side of the title, and the title should be centered)
- The bluetooth module (pybluez) is producing errors when attempting to import and implement it into the app.
- There is not yet a way to reliably move between the bluetooth connection and home pages without crashing the program.

### Bluetooth communication
- The HC05 is meant for simple bluetooth over serial and cannot recieve reasonable audio quality over bluetooth, nor can it send regular data over bluetooth to remedy this an ESP32 will be used to do all the audio/bluetooth processing.
- HC05 does not have any working code in this github as of 11/5, and the code included in main3.c has an unknown bug (clock speed?). 
