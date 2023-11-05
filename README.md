# BSafeHelmet

## Blind-Spot/Crash Detection
For the Pre-Alpha build, the persistent state of polling the ultra-sonic sensor for objects has been developed. The program is able to detect when an object is in range of the sensor. Currently the math for how far away the object is has not been developed properly, we will work on that for the next milestone. Additionally, we have interfaced with an IMU to grab accelerometer and gyroscope data. This exact accelerometer will need to be tested in the near future to determine if it has high enough quality for the finished product. Lastly, we have a RTOS to run the future program on. All we need to do is add proper threads.

## App
### Completed Work
After researching different methods of app building, we decided to build our app using the Beeware framework, and first completed some tutorial work to familiarize ourselves with how it operates. We then created a simple UI design for our app homepage to visualize what the app design will look like. We were able to display a real map at a specified GPS location, in addition to a mock up of a nav bar and a button to connect to a helmet via bluetooth. We are also in the process of implementing bluetooth into the app, though it does not yet successfully detect any devices emitting a bluetooth signal.

### Project Architecture


### Known Bugs


## Bluetooth communication
The HC05 bluetooth module can send/recieve information over a serial port on a PC.
The HC05 is meant for simple bluetooth over serial and cannot recieve reasonable audio quality over bluetooth, nor can it send regular data over bluetooth.
