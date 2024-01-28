# BSafeHelmet
## Project Description
This project introduces a state-of-the-art smart helmet designed to significantly enhance the safety of motorcycle and moped riders. Equipped with advanced sensors, the helmet is capable of detecting objects in the rider's blind spots, alerting them through LED indicators integrated into the helmet. This feature is pivotal in preventing potential collisions by increasing situational awareness. Additionally, the helmet boasts Bluetooth connectivity, seamlessly pairing with a specialized mobile app. This app plays a crucial role in safety, offering live GPS tracking that displays the rider's current location on a map, ensuring they are always traceable during their journey.

A standout feature of this smart helmet is its automatic emergency response system. In the unfortunate event of a crash or collision, the helmet's sensors detect the impact and communicate with the connected mobile app, triggering an automatic distress signal to emergency services. This signal includes the rider's precise GPS location, facilitating prompt response and assistance. The mobile app's interface is user-friendly, allowing riders to effortlessly access and manage the helmet's features, including blindspot monitoring and emergency contact settings. This project not only elevates the riding experience with its technological advancements but also significantly contributes to the safety and well-being of riders, potentially saving lives and offering peace of mind.

## Project Architecture

TIVA Launchpad (main programs and data parsing)
- Ultrasonic Sensors to detect vehicles and objects in blindspots
- Lights to turn on when blindspot trigger met
- Accelerometer to aid in detecting crashes
- Gyroscope to aid in detecting crashes
- GPIO interrupt on crash detection
  
Bluetooth Module (bridge communication between mobile app and helmet hardware)
- Send crash data to mobile phone to trigger emergency response
- Receive audio data from phone and play through helmet speakers

Mobile App (pairs to helmet via bluetooth)
- Allows user to connect phone audio to helmet speakers for music
- Tracks live location of user via GPS modules in phone
- If crash detected, calls emergency response teams for help using user location

## Completed Work

### Blind-Spot/Crash Detection
For the prototype, the persistent state of polling the ultra-sonic sensor for objects has been developed and integrated as a thread within the RTOS. The program is able to detect when an object is in range of the sensor, and display this info via a LED. Additionally, we have interfaced with an IMU to grab accelerometer and gyroscope data. The IMU data can be manipulated in such a way to determine when a crash has occurred and relay to the bluetooth interface. This functionality has been deployed within a thread in the RTOS.

### App
For the project prototype milestone, we focused on two things: updating the UI and implementing functional bluetooth connectivity. For the UI, we first sought to have the map display encompass the entire screen, except with a navigation bar at the bottom of the screen (positioned over the map). The navbar contains three buttons, each of which display to the user a different window depending on which was clicked. The bluetooth button and corresponding screen attempts to locate a bluetooth signal, though we have not yet succeeded in detecting an actual signal. The map page (which is displayed by default) allows users to scroll and zoom freely, and displays a POI marker at the location provided. The profile page has a text input box for users to input their phone number so that it may be used to call emergency services in the event of a crash. The bluetooth module has proven difficult to implement, and we have encountered many roadblocks in doing so. We initially attempted to utilize the PyBluez module, but experienced a number of crashes and bugs before pursuing a different strategy. The current bluetooth library we are implementing, Bleak, seems promising but often crashes when attempting to detect live bluetooth signals. Additional testing and bug fixing is needed to achieve a fully functional bluetooth connection, and will be a primary goal in the days to come.

### Bluetooth Communication
The HC05 bluetooth module can send/recieve information over a serial port on a PC. This will be insufficient for our project so we ordered a more sophisticated bluetooth communication module.

## Known Bugs
### Blind-Spot/Crash Detection
-Outside of verified range HC-SR04 receives inconsitent data, currently mitagated by waiting for a valid read. 

### App
- Difficulty reading crash signal data when sent by ESP device.

### Bluetooth communication
- The HC05 is meant for simple bluetooth over serial and cannot recieve reasonable audio quality over bluetooth, nor can it send regular data over bluetooth to remedy this an ESP32 will be used to do all the audio/bluetooth processing.
- HC05 does not have any working code in this github as of 11/5, and the code included in main3.c has an unknown bug (clock speed?). It does however work in Keil uVision with their packages and header files.


## Building the BSafe components
### OnBoard project
- To build the OnBoard project, you must first download the latest version of Code Composer Studio found here: https://www.ti.com/tool/download/CCSTUDIO/12.5.0.
- Next, clone the repository from GitHub to a local directory on your machine.
- Once the repository is cloned, open Code Composer Studio. When prompted to choose a workspace, choose the cloned "BSafeHelmet/OnBoard" directory.
- Lastly, in CCS, click on "Import project", and choose the cloned "BSafeHelmet/OnBoard/UltraSonic" directory.
- You will now be able to build the project in the CCS IDE.
- Note: if you receive include errors upon first build, right click the project name ("UltraSonic") in the Project Explorer tab and click on "Properties". Then navigate to Build -> Arm Properties -> Include Options. Delete the current path to the SW-TM4C-2.2.0.295 SDK and replace with your exact path to the SDK. The build should now work with no errors.

### BSafeBluetooth ESP32 Build Instructions
-Detailed walkthrough located in /ESP32_PROJECTS/ESP-SETUP.md

### BSafe Mobile App Build Instructions
- Acquire an Android 5+ mobile device.
- Download the BSafe_App.aia file to your computer.
- Open up MIT app inventor and create an account (if necessary).
- From the Projects tab, select 'import project (.aia) from my computer'.
- Open that project and download the Android App (.apk) from the Build tab.
- Copy the .apk file onto your android device and run the app (enable all permissions!).
