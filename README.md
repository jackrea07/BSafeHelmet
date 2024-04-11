# BSafeHelmet
## Link to Repo
repo: https://github.com/evanchang43/BSafeHelmetPCB

## Link to Journal log
https://docs.google.com/spreadsheets/d/1Mm9VA6NvYEt4OLsk81zikazPE4Yr7_iFd0ALAwPqtKs/edit#gid=786079556

## Link to PCB repo and parts list
repo: https://github.com/evanchang43/BSafeHelmetPCB
parts list: https://docs.google.com/spreadsheets/d/1Ij_14RiddeNyJEjv02iHXSV4uPIW0RRCsKHnJEb6k7Q/edit?usp=sharing
## Link to Video Demos
App: https://drive.google.com/file/d/1fuTFZnjEL3Cb6rX_PFb6AShLxgxvSsbe/view?usp=sharing

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
- One buggy ultrasonic sensor, have determined it is not a software issue. Have ordered a new sensor, it is currently on its way.

### App/Bluetooth
- None at the moment!

### Printed Circuit Board
- The board was manufactured with multiple vias missing. The absence of these vias contributed to the lack of functionality in the audio module. 
- The TIVA, while drawing the same amount of current as was allowed on the breadboard, heats in place after 5-10 minutes. The current diagnosis for this issue is that the current is not being managed correctly elsewhere on the board. 
- Some (but not all) TIVA pins fail randomly. The current diagnosis for this issue is that the heating of the Tiva causes these pins to fail and behave unpredictably. As a result, while the crash detection algorithm is correctly identifying crashes during debugging, the signal pins used to communicate the crash are not working effectively. 
- One side of the helmet’s blind-spot detection does not work because of a faulty sensor. A replacement sensor has been ordered. 

## Work Completed for Production Release
### Helmet/Hardware
- Conducted extensive false-positive and false-negative testing on crash detection algorithm.
- Developed, ordered, and assembled printed circuit board.
- Conducted thourough testing to establish root cause of failure on PCB.
### Bluetooth/Software
- General UI changes and quality of life improvements.
- Added product description and brand logo to profile page.
- Fixed bug where bluetooth connection would disrupt if user switched pages prior to connection completing.
- Cleaned up bluetooth device display to only show BSafe devices.
- Added functionality to send text messages to both 911 and a user-specified emergency contact with the user's name and GPS location.

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
