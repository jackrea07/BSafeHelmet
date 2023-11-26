# Guide to setting up ESP-IDF environment & simple usage
## NOTES:
Windows 10 procedure
Up to date VScode is a prerequisite
This require a sizeable chunk of disk space, but you can delete the installed SDK later if needed
**this is what worked for me.
## ESP IDF basic SETUP
1: In VScode, install the extension "Espressif IDF".
<div align="center"> <img src="images/step1.JPG" width = "1000" /> </div> 
2: Open the extension on the left side (you may need to dig for it),
the first opening will run the IDF Setup (this might take a bit).

3: Click Express
<div align="center"> <img src="images/step2-3.JPG" width = "1000" /> </div> 
4: Change the ESP-IDF version to the latest release (v5.1.2).
I used the default directories but if you don't, make sure to write them down 
for reference in the environment variable setup.

5: Click install (this will take a awhile to setup the sdk, maybe 10+ minutes).
<div align="center"> <img src="images/step4-5.JPG" width = "1000" /> </div> 

## Enviornment variable setup 
Use these instead of using relative paths in each project.
6: Search up "advanced system settings" in  your start menu and open "view advanced system settings"
Advanced tab->Environment Variables...
<div align="center"> <img src="images/step6.JPG" width = "1000" /> </div> 
7: Add a variable by clicking "New" in the system variables tab
Variable Name: IDF_PATH
Variable value: C:\Users\<.User>\esp\esp-idf
(replace <.User> with your local username directory)
<div align="center"> <img src="images/step 7.JPG" width = "500" /> </div> 
<div align="center"> <img src="images/step7.JPG" width = "500" /> </div> 
8: Add another variable in the same way:
Variable Name: IDF_TOOLS_PATH
Variable value: C:\Users\<.User>\.espressif
## Basic install finished

## Uses and information:
### Command pallet
Interact with the VSCode Espressif IDF extension using predefined commands in the command pallet
ctrl+shift+p to open the command pallet or
click search in the top middle 
Espressif commands begin with "ESP-IDF:"

### Useful workflow with commands
### 1
#### ctrl+shift+p
Open command pallet and type:
#### ESP IDF: Show Examples Project
press enter to indicate the working directory
Opens a menu of example projects, there are many to choose from
select 

#### ESP IDF: Set Espressif device target 
After



