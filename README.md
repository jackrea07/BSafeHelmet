# BSafeHelmet
## Link to Repo
https://github.com/jackrea07/BSafeHelmet/tree/main

Journal log is in the github repo!

PCB Repo: https://github.com/evanchang43/BSafeHelmetPCB
## Link to Video Demos
App: https://drive.google.com/file/d/1RnGf_oHPC9VafAtWeLwomjTPM84jExlQ/view?usp=sharing

Helmet/Hardware: https://youtu.be/3T6ek3lzZdU

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
  
ESP Module (bridge communication between mobile app and helmet hardware)
- Send crash data to mobile phone to trigger emergency response
- Receive audio data from phone and play through helmet speakers
- Controls quality of life features like battery monitoring. 

Mobile App (pairs to helmet via bluetooth)
- Allows user to connect phone audio to helmet speakers for music
- Tracks live location of user via GPS modules in phone
- If crash detected, calls emergency response teams for help using user location

## Known Bugs
### Blind-Spot/Crash Detection
- Inconsisent ultrasonic readings at exact specified threshold. 

### App/Bluetooth
- Crash response does not trigger properly upon reception of crash signal from ESP device and must be activated manually. May need to send BT notification signal to app from the ESP.

### Misc
- Speaker setup is unideal and needs an amplifier. Volume control is not implemented yet, but the ESP recieves volume information.
  
## Work Completed for Beta Build
### Helmet/Hardware
- Re-evaluated sensor feasability. Discovered HC-SR04 timing needs to be at a precision level we were unable to maintain when moving the system into an RTOS
- Researched and ordered LV-MaxSonar-EZ 1010 sensor. This sensor has two modes of communication, RS232 and analog voltage.
- Attempted to implement UART communication with LV-MaxSonar-EZ 1010 sensor, discovered that RS232 communication protocol is inversed on the sensor.
- Integrated level shifter with LV-MaxSonar-EZ 1010 sensor.
- After implementing the level shifter, discovered UART consistently receives a page break interrupt, which results in the message failing.
- Implemented ADC to read and convert the analog voltage output of the LV-MaxSonar-EZ 1010 sensor.
- Determined sensors trigger consistent false positives, designed algorithm to require multiple consecutive "hit" signals before illuminating blind spot LED.
- Researched motorcyle crash data. Scaled data down to 15mph crash and created g-force threshold for crash detection.
- Finallized breadboard design and layout for use in creating PCB.
- Attempted to implement designs for ESP32, Tiva Launchpad, voltage regulator,  LV-MaxSonar-EZ, and PCM audio devices in Altium. Learned that Altium does not link footprints and symbols.
- Implemented designs for ESP32, Tiva Launchpad, voltage regulator,  LV-MaxSonar-EZ, and PCM audio devices in Altium.
- ESP audio outputs to an external I2S DAC. This chip doesn't provide audio amplification but we can use 3.5mm earbuds to hear with reasonable quality.
- Tested amperage through the circuit with a digital multimeter, the hardware setup as of 2/23 drew approx. 270mA on average. Note that our power providing chip can handle at most 500mA and we have not connected amplified speakers yet.
### Bluetooth/Software
- Implemented improvements in the Bluetooth connection logic to stabilize the link between the BLE device and the phone.
- Refined data handling processes to ensure accurate reception and processing of data sent by the ESP 32.
- Performed extensive testing to map out and evaluate all user interface paths related to emergency contact settings.
- Modified the app's logic to default to dialing 911 if no user-specified emergency contact number is saved, ensuring there is always a number to call in case of a crash.
- Discovered through performance testing that the notifier would remain active indefinitely without user action after a crash signal, halting the emergency response process.
- Introduced a countdown mechanism that activates upon receiving a crash signal. Configured the app to automatically place an emergency call to the predefined number (or 911 if no number is set) if no user input is detected by the end of the countdown.
- Bluetooth audio implemented on the ESP32 using an A2DP sink, can connect/use the ESP as you would any bluetooth speaker.

## Building the BSafe components
### OnBoard project
- To build the OnBoard project, you must first download the latest version of Code Composer Studio found here: https://www.ti.com/tool/download/CCSTUDIO/12.5.0.
- Next, clone the repository from GitHub to a local directory on your machine.
- Once the repository is cloned, open Code Composer Studio. When prompted to choose a workspace, choose the cloned "BSafeHelmet/OnBoard" directory.
- Lastly, in CCS, click on "Import project", and choose the cloned "BSafeHelmet/OnBoard/UltraSonic" directory.
- You will now be able to build the project in the CCS IDE.
- Note: if you receive include errors upon first build, right click the project name ("UltraSonic") in the Project Explorer tab and click on "Properties". Then navigate to Build -> Arm Properties -> Include Options. Delete the current path to the SW-TM4C-2.2.0.295 SDK and replace with your exact path to the SDK. The build should now work with no errors.

### BSafeBluetooth ESP32 Build Instructions
- Detailed walkthrough located in /ESP32_PROJECTS/ESP-SETUP.md

### BSafe Mobile App Build Instructions
- Acquire an Android 5+ mobile device.
- Download the BSafe_App.aia file to your computer.
- Open up MIT app inventor and create an account (if necessary).
- From the Projects tab, select 'import project (.aia) from my computer'.
- Open that project and download the Android App (.apk) from the Build tab.
- Copy the .apk file onto your android device and run the app (enable all permissions!).
