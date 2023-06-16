# Table of content
  * [preface](#preface)
  * [car design](#car-design)
  * [components, prices and links](#components-prices-and-links)
  * [schematics](#schematics)
  * [demo videos](#demo-videos)
  * [android app](#android-app)
  * [Important Notes](#Important-Notes)

# preface

<details>
<summary> click to view...</summary>

our problem statment was to create a car that follows a line without drifting out of the line and traverse a maze to find a specific path from end to end, the car have a small user interface via buttons, can play song and produce various patterns on strip of neopixel leds while traversing the maze or running on the line tracker, all without affecting the main functionality of the car, this wouldn't have been possible without the help of freeRTOS and espressif-idf.


### An autonomous car that can follow a line, solve maze and find it's path out. 
![our lovely car](./temp%20files%20for%20images%20and%20videos/car.jpg)

### shape of line tracker that was given to us to train on 
![line tracker1](./temp%20files%20for%20images%20and%20videos/line%20tracker1.jpg)

### shape of line tracker that was given on compitition day 
![line tracker2](./temp%20files%20for%20images%20and%20videos/line%20tracker2.jpg)

### shape of maze that was given to us and used on competition day
![maze](./temp%20files%20for%20images%20and%20videos/maze.jpg)



</details>


# car design

<details>
<summary> click to view...</summary>

- we used [freeCAD](https://www.freecad.org/) to design our car body, the design files can be found in [3D car design files/freeCAD-project files](./3D%20car%20design%20files/freeCAD-project%20files/) and you can open it using freeCAD, at first we designed the car to be of 3 floors and designed a holder for the sensor but after printing a prototype of the design, it was found that files called `third floor.FCStd` and `first_floor.FCStd` are only the useful file and all of the rest of design files were not used for the sake of this project.

- to print the 3d design, you need to generate STL files from your design (was done by freeCAD also), so the generated stl files are found in [3D car design files/stl files for 3 printing](./3D%20car%20design%20files/stl%20files%20for%203%20printing/) where `1st - floor.stl` and `3rd - floor.stl` are only files that we printed where each files is printed alone and you shall send the `.stl` files to the 3D printing service company not the `.FCStd` files.

- we made a custom design not a readily printed one as we needed a custmized shape, surface area and customized space for PCB and 2 motors, we choosed a circular shape as it seems to be a nice modern shape and it was inspired by [Pololu car](https://www.youtube.com/watch?v=mJV-KDqHgDQ)


## image of the final designed
![final car body](./3D%20car%20design%20files/Screenshot%20of%20the%20design%20.png)


</details>

# components, prices and links


<details>
<summary> click to view...</summary>

you can find the list of components with prices in the directory called [prices & components used](./prices%20%26%20components%20used/) in the `prices and components list.pdf` or `prices and components list.xlsx` but it's not mentioned where we purchased these components and the prices doesn't include custom costs nor the shipping prices.



**if you have time, we recommend you to purchase all of the components from ali express as it's cheaper in prices** 

| component | no.of items | unit price | link | extra details|
| :---      | :----:      |  :----:    | :--- | :---         |
| 5 Channel TCRT5000 IR ![5 Channel TCRT5000 IR](./temp%20files%20for%20images%20and%20videos/5%20Channel%20TCRT5000%20IR%20.png) | 2 | 50 EGP | https://www.aliexpress.com/item/1005004984933041.html?spm=a2g0o.order_list.order_list_main.25.21ef1802DzQBHj | used to detect line, intersections, path, etc...| ![5 Channel TCRT5000 IR](./temp%20files%20for%20images%20and%20videos/5%20Channel%20TCRT5000%20IR%20.png) |
| 3D printing (Car Chassis) ![3D printing (Car Chassis)](./temp%20files%20for%20images%20and%20videos/Screenshot%20of%20the%20design%20.png) | 1 | 460 EGP | https://wa.me/+201228206436 and https://wa.me/+201203414479 | this is the car body, there are plenty of companies, individuals whom provide 3d printing services, but these 2 numbers are for students who provide 3d printing services for a good quality with cheap prices | ![3D printing (Car Chassis)](./temp%20files%20for%20images%20and%20videos/Screenshot%20of%20the%20design%20.png)
| wires ![wires](./temp%20files%20for%20images%20and%20videos/jumper%20wires.png) | 40 | 1 EGP | https://store.fut-electronics.com/search?type=product&q=jumper+wire | was used to connect compoents with each other, future electronics provides jumper wires with many different lengths ( we used 30 cm and 20 cm wire) | ![wires](./temp%20files%20for%20images%20and%20videos/jumper%20wires.png)
| voltage regulator (L7805) ![voltage regulator (L7805)](./temp%20files%20for%20images%20and%20videos/L7805.png) | 1 | 4.5 EGP | https://free-electronic.com/product/l7805cv/ | supplied esp32 with the power supply of 5V, we had to insulate the power source of esp32 from other components as speakers made small fluctuations in the power source which made esp32 to reset several times | ![voltage regulator (L7805)](./temp%20files%20for%20images%20and%20videos/L7805.png)|
| PCB Bread Board Shape FR2 (9X15 cm2) ![PCB Bread Board Shape FR2 (9X15 cm2)](./temp%20files%20for%20images%20and%20videos/PCB.png) | 2 | 15 EGP | https://free-electronic.com/product/pcb-bread-board-shape-fr2-9x15-cm2-horizontal/ | we used a prototype pcb which is more reliable than solderless board | ![PCB Bread Board Shape FR2 (9X15 cm2)](./temp%20files%20for%20images%20and%20videos/PCB.png) |
| pin header ![pin header](./temp%20files%20for%20images%20and%20videos/pin%20header.png) | 8 | 4 EGP | https://free-electronic.com/?s=pin+header&product_cat=0&post_type=product | needed pin headers to be soldered onto prototype pcb (we mainly used female pin header and rarely male pin headers) | ![pin header](./temp%20files%20for%20images%20and%20videos/pin%20header.png) |
| Robot Car Wheel 85mm ![Robot Car Wheel 85mm](./temp%20files%20for%20images%20and%20videos/robot%20wheel.png) | 2 | 85 EGP |  https://free-electronic.com/product/robot-car-wheel-85mm/ | this was the perfect wheel for our car body the provided needed height from the ground | ![Robot Car Wheel 85mm](./temp%20files%20for%20images%20and%20videos/robot%20wheel.png) | 
| Metal Coupler 4mm ![Metal Coupler 4mm](./temp%20files%20for%20images%20and%20videos/coupler.png) | 2 | 35 EGP | https://free-electronic.com/product/metal-coupler-4mm-short-coupling-for-robot-smart-car-wheel/ |  was used to connect the motors to the wheels | ![Metal Coupler 4mm](./temp%20files%20for%20images%20and%20videos/coupler.png) |
| Metal Caster Wheel for Robot (20mm) ![ Metal Caster Wheel for Robot (20mm)](./temp%20files%20for%20images%20and%20videos/caster%20wheel.png) | 2 | 35 EGP | https://free-electronic.com/product/metal-caster-wheel-for-robot-20mm/ | since car has only 2 motors so we needed 2 caster wheel to support car body from beneath to prevent it from swinging | ![ Metal Caster Wheel for Robot (20mm)](./temp%20files%20for%20images%20and%20videos/caster%20wheel.png) |
| NeoPixel Led stick ![NeoPixel Led stick](./temp%20files%20for%20images%20and%20videos/NeoPixel%20Led%20stick.png) | 1 | 200 EGP | https://www.aliexpress.com/i/4000203985307.html | I bought this component before a year from ram-electronics but it seems now it's not available on ram any more but it's available on ali express | ![NeoPixel Led stick](./temp%20files%20for%20images%20and%20videos/NeoPixel%20Led%20stick.png)
| DC Gear Motor (8.8KG ) 250RPM ![DC Gear Motor (8.8KG ) 250RPM](./temp%20files%20for%20images%20and%20videos/gear%20motor.png)  | 2 | 325 EGP | https://free-electronic.com/product/dc-gear-motor-8-8kg/ | criteria of selection of our gear motor is that it has high reduction ration with high torque and high rpm but note that rpm is inversely proportional to reduction ratio so our motor had torque of 8.8 KG with rpm = 250 RPM and reduction ration 25:1 but it worked perfectly | ![DC Gear Motor (8.8KG ) 250RPM](./temp%20files%20for%20images%20and%20videos/gear%20motor.png)
| Motor driver DRV8871 ![Motor driver DRV8871](./temp%20files%20for%20images%20and%20videos/drv8871.png) | 2 | 130 EGP | https://www.aliexpress.com/item/1005001590487986.html?spm=a2g0o.order_list.order_list_main.40.21ef1802DzQBHj | motor driver small chip where one chip controls one motor so we needed 2, this is better than L298 as it's smaller, can pass higher current | ![Motor driver DRV8871](./temp%20files%20for%20images%20and%20videos/drv8871.png) |
|  Screw Mounting Bracket ![Screw Mounting Bracket](./temp%20files%20for%20images%20and%20videos/motor%20holder.png) | 2 | 50 EGP | https://www.aliexpress.com/item/32856826638.html?spm=a2g0o.order_list.order_list_main.89.21ef1802DzQBHj | used to hold the motor onto the car body | ![Screw Mounting Bracket](./temp%20files%20for%20images%20and%20videos/motor%20holder.png) |
| ESP32 Development Board ![ESP32 Development Board](./temp%20files%20for%20images%20and%20videos/esp32.png) | 1 | 150 EGP | https://www.aliexpress.com/item/1005001621773806.html?spm=a2g0o.order_list.order_list_main.95.21ef1802DzQBHj | this was our main MCU and we recommend using it over arduino as it has many useful features like its espressif-idf that comes with freeRTOS, dual core, high speed MCU (240 MHZ), integrated wifi and integrated bluetooth on chip| ![ESP32 Development Board](./temp%20files%20for%20images%20and%20videos/esp32.png) |
| DFPlayer Mini MP3 ![DFPlayer Mini MP3](./temp%20files%20for%20images%20and%20videos/dfplayer.png) | 1 | 50 EGP | https://www.aliexpress.com/item/1005004994130177.html?spm=a2g0o.order_list.order_list_main.70.21ef1802DzQBHj | a chip that plays mp3 audios without need of continous interaction with MCU, you just give it mp3 file number using MCU and it will play it for you | ![DFPlayer Mini MP3](./temp%20files%20for%20images%20and%20videos/dfplayer.png) |
| Speaker 4 Ohm 3 Watt ![Speaker 4 Ohm 3 Watt](./temp%20files%20for%20images%20and%20videos/speaker.png) | 2 | 55 EGP | https://www.aliexpress.com/item/1005005020984275.html?spm=a2g0o.order_list.order_list_main.15.21ef1802DzQBHj | speakers used to output sound and music while traversing | ![Speaker 4 Ohm 3 Watt](./temp%20files%20for%20images%20and%20videos/speaker.png) |
| I/O Expander Breakout - SX1509 ![I/O Expander Breakout - SX1509](./temp%20files%20for%20images%20and%20videos/sx1509.png) | 1 | 85 EGP | https://store.fut-electronics.com/products/copy-of-bi-directional-voltage-level-translator-shifter-txb0104?_pos=1&_sid=6a669f7ad&_ss=r | since the amount of pins on esp32 wouldn't be enough for all components, we used a port expander that communicates with esp32 through I2C that will increase the number of pins available | ![I/O Expander Breakout - SX1509](./temp%20files%20for%20images%20and%20videos/sx1509.png) |
| battery holder ( 3 x 18650 ) ![battery holder ( 3 x 18650 )](./temp%20files%20for%20images%20and%20videos/battery%20holder.png) | 1 | 15 EGP | https://free-electronic.com/product/battery-holder-3-x-18650/ | holder for lithium batteries as our car runs on 3 lithium batteries | ![battery holder ( 3 x 18650 )](./temp%20files%20for%20images%20and%20videos/battery%20holder.png) |
| Rechargeable Li-ion Battery18650 ![Rechargeable Li-ion Battery18650](./temp%20files%20for%20images%20and%20videos/battery.png) | 3 | 50 EGP | https://free-electronic.com/product/rechargeable-li-ion-battery18650-3-7v-1200mah/ | used as power source for the car | ![Rechargeable Li-ion Battery18650](./temp%20files%20for%20images%20and%20videos/battery.png) | 
| electrolytic capacitor (25V - 60 micro farad) ![electrolytic capacitor](./temp%20files%20for%20images%20and%20videos/capacitor.png) | 1 | 3 EGP | https://ram-e-shop.com/product/c-47u250v/ | was used to be connected between 3.3v and GND that helps in power source stability | ![electrolytic capacitor](./temp%20files%20for%20images%20and%20videos/capacitor.png) |
| switches  ![switch](./temp%20files%20for%20images%20and%20videos/swithc.png) | 2 | 1.5 EGP | https://free-electronic.com/product/boat-rocker-switch-on-off-6a250vac-kcd1/ | one used to switch on/off the battery from the system and second one to switch on/off esp32 from the system | ![switch](./temp%20files%20for%20images%20and%20videos/swithc.png) |
| push buttons ![push button](./temp%20files%20for%20images%20and%20videos/push_buttoin.png) | 3 | 0.75 EGP | https://free-electronic.com/product/press-2pin-6x6/ | used as user interface , 1 for Line tracker (L) , 1 for maze trainer (M_t) , 1 for maze solver (M_s) | ![push button](./temp%20files%20for%20images%20and%20videos/push_buttoin.png) |
| LM2596HVS-ADJ DC-DC Step-Down Module (3A) ![DC-DC Step-Down Module](./temp%20files%20for%20images%20and%20videos/DC-DC%20bulk.png) | 1 | 60 EGP | https://free-electronic.com/product/lm2596hvs-adj-dc-dc-step-down-module-3a/ | used to convert power supply from batteries (around 11 volt) to 3.3V to supply all the system of the car except esp32 that's supplied by voltage regulator and we didn't use voltage regulator to supply the whole system as the whole system withdraw a high amount of current and the maximum that a voltage regulator can supply is about 1 A| ![DC-DC Step-Down Module](./temp%20files%20for%20images%20and%20videos/DC-DC%20bulk.png)|
| external EEPROM - 32 Kilo-byte (24C256) ![EEPROM](./temp%20files%20for%20images%20and%20videos/eeprom.png) | 1 | 25 EGP | https://free-electronic.com/product/24c256/ | was used to store the optimized path after traversing the maze as Esp32 doesn't have interal EEPROM so we bought external one to store the optimized path on it so that if the system shutdown (car was turned off), we don't have to train again as the path is already stored | ![EEPROM](./temp%20files%20for%20images%20and%20videos/eeprom.png) |
| IC SOCKET Base 8 PIN DIP for eeprom ![IC socket](./temp%20files%20for%20images%20and%20videos/socket.png) | 1 | 0.5 EGP | https://free-electronic.com/product/ic-socket-base-8-pin-dip/ | we didn't solder EEPROM IC onto the pcb board directly but instead, we solder the IC socket and the eeprom is inserted to the socket so any time, you can change the eeprom | ![IC socket](./temp%20files%20for%20images%20and%20videos/socket.png) |
| PAM8403 5V Power Audio Amplifier Board ![amplifier](./temp%20files%20for%20images%20and%20videos/amplifier.png) | 1 | 35 EGP | https://free-electronic.com/product/pam8403-5v-power-audio-amplifier-board/ | since the DFPlayer can output weak analog signal, the this amplifier module is used to amplify the signal before bypassing it to the speakers and the knob on it is used to control the volume | ![amplifier](./temp%20files%20for%20images%20and%20videos/amplifier.png) | 

</details>

# schematics

<details>
<summary> click to view...</summary>

- we made the schematics using [KICAD](https://www.kicad.org/) (one of the most free useful tools in PCB designs and schematics) where the KICAD project files can be found in the directory called [schematics files/KICAD project files](./schematics%20files/KICAD%20project%20files/) and the schematic itself is exported as pdf where it can be found in [schematics files/shematics view.pdf](./schematics%20files/shematics%20view.pdf)

![schematics](./temp%20files%20for%20images%20and%20videos/schematic.png)

</details>

# demo videos



<details>
<summary> click to view...</summary>

## Line tracker 
### (click on image to view the video)
[![IMAGE ALT TEXT](http://img.youtube.com/vi/FVEoGQgsD1o/0.jpg)](http://www.youtube.com/watch?v=FVEoGQgsD1o "Line tracker on competition day")

## Maze trainer 
### (click on image to view the video)
[![IMAGE ALT TEXT](http://img.youtube.com/vi/uL8tybA_7e4/0.jpg)](http://www.youtube.com/watch?v=uL8tybA_7e4 "Maze trainer on competition day")

## Maze solver 
### (click on image to view the video)
[![IMAGE ALT TEXT](http://img.youtube.com/vi/FgmfNq2BWTQ/0.jpg)](http://www.youtube.com/watch?v=FgmfNq2BWTQ "Maze solver on competition day")

## Miscellaneous Videos
### (click on image to view the video)
---

### competition day line tracker with sound
[![IMAGE ALT TEXT](http://img.youtube.com/vi/pODliE_6rCc/0.jpg)](http://www.youtube.com/watch?v=pODliE_6rCc "Line tracker on competition day")

### random expriment 1 on Line tracker
[![IMAGE ALT TEXT](http://img.youtube.com/vi/ryqJlh_6-NY/0.jpg)](http://www.youtube.com/watch?v=ryqJlh_6-NY "Line tracker 1")

### random expriment 2 on Line tracker
[![IMAGE ALT TEXT](http://img.youtube.com/vi/IrJWMPCKxJE/0.jpg)](http://www.youtube.com/watch?v=IrJWMPCKxJE "Line tracker 2")

### competition day maze trainer with sound
[![IMAGE ALT TEXT](http://img.youtube.com/vi/vTmSzqmyeRo/0.jpg)](http://www.youtube.com/watch?v=vTmSzqmyeRo "maze trainer on competition day")

### random expriment 1 maze trainer 
[![IMAGE ALT TEXT](http://img.youtube.com/vi/PMsLQ8f5rEo/0.jpg)](http://www.youtube.com/watch?v=PMsLQ8f5rEo "random maze trainer")

### random expriment 2 maze trainer 
[![IMAGE ALT TEXT](http://img.youtube.com/vi/YR6AI_VsmS8/0.jpg)](http://www.youtube.com/watch?v=YR6AI_VsmS8 "random maze trainer")

### random expriment 3 maze trainer 
[![IMAGE ALT TEXT](http://img.youtube.com/vi/V4b4zh7_W2g/0.jpg)](http://www.youtube.com/watch?v=V4b4zh7_W2g "random maze trainer")

### random expriment 4 maze trainer 
[![IMAGE ALT TEXT](http://img.youtube.com/vi/Yapoz3KJym8/0.jpg)](http://www.youtube.com/watch?v=Yapoz3KJym8 "random maze trainer")

### competition day maze solver with sound
[![IMAGE ALT TEXT](http://img.youtube.com/vi/Ea6kgzUnRns/0.jpg)](http://www.youtube.com/watch?v=Ea6kgzUnRns "maze solver on competition day")

### random expriment maze solver 
[![IMAGE ALT TEXT](http://img.youtube.com/vi/8YGnpCDjReU/0.jpg)](http://www.youtube.com/watch?v=8YGnpCDjReU "random maze solver")

### random expriment 1 maze solver & maze trainer
[![IMAGE ALT TEXT](http://img.youtube.com/vi/6LpIFKPfciE/0.jpg)](http://www.youtube.com/watch?v=6LpIFKPfciE "random maze")

### random expriment 2 maze solver & maze trainer
[![IMAGE ALT TEXT](http://img.youtube.com/vi/ktwz5GvXBHs/0.jpg)](http://www.youtube.com/watch?v=ktwz5GvXBHs "random maze")


</details>

# android app

<details>
<summary> click to view...</summary>

- the app can be found on [google play sore](https://play.google.com/store/apps/details?id=com.carcotech.maze_solver) 

- whereas the code can be found in the directory called [android code](./android%20code/)

- we used android app only in the line tracker as the mobile application sends "H" via bluetooth to the car to tell it that it's running on a straight so move faster or send "L" via bluetooth to the car to tell it that it's not running on a straight so move slower 

![app](./temp%20files%20for%20images%20and%20videos/app.gif)

</details>

# Important Notes

<details>
<summary> click to view...</summary>

1. we used freeRTOS that comes with espressif-idf.
2. code can be found in [ESP32 code/Carcotech/](./ESP32%20code/Carcotech/) where the project is made using [platformIO](https://platformio.org/) (platformIO is just vscode extension that can be downloaded), we recommend using platformIO over arduinoIDE as you can manage code more professionaly on vscode than on arduino IDE.
3. to open the project, open platformIO and click open project where the project working directory is `ESP32 code/Carcotech/`, also note that we used alot of libraries where some of them aren't included in the project files but they are present in platformIO repo, so once you open the project using platformIO, platformIO will download these libraries from its repo.
4. if you couldn't understand the code, [here](https://youtu.be/uo7UUR3-JVw) is a video for explanation of the code but note that the video is in arabic.
5. in the directory called [datasheet of some components](./datasheet%20of%20some%20components/), as the name implies, you can find some of the datasheets and reference manual of the components that we used .
6. name of libraries that are not included in this project files are found in the file called [platformIO.ini](./ESP32%20code/Carcotech/platformio.ini) where the libraries are 
    -   dfrobot/DFRobotDFPlayerMini @ 1.0.5
    -   https://github.com/RobTillaart/I2C_EEPROM
    -   https://github.com/adafruit/Adafruit_NeoPixel 


</details>