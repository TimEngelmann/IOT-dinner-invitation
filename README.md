# IOT-dinner-invitation
Want to invite your neighbor for dinner but you don't like texting? Well then just give them of those boxes and keep one for you. Press one of the buttons to send out a dinner invite. Your neighbor will receive it through their box: a voice makes an announcment and it will show on the screen. Your neighbor then has some time to accept or decline your invite and you will of course be notified about their decision.

## How it works?
Both microcontrollers are connected to wifi (doesn't need to be the same). Every 20 seconds they make an https request to check a single variable (communication/invite) in a realtime firebase database. If its 0 there is no invite, if its 1 the neighbor posted an invite. From there it becomes a state automat (see sketch). 

<img src="https://user-images.githubusercontent.com/46136690/168020001-de1004d2-4c09-46d7-a12f-3b544f1fbca9.png" width="400">


## Circuit Design
<img src="https://user-images.githubusercontent.com/46136690/168016286-f7d75826-7e0b-4afb-bcca-fa6a89efdcaa.png" width="600">
<img src="https://user-images.githubusercontent.com/46136690/168016467-19ecfbb7-0044-4da5-8e48-8840ea85013f.png" width="600">

## Installation

- Install Arduino **IDE**: [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
- Follow the following steps out of the handbook to install the **board** library (ESP32-Platform)
    [ESP_Arduino_Installation.pdf](https://github.com/TimEngelmann/food_call/files/7895384/ESP_Arduino_Installation.pdf)
- Install the **mp3 player** library
    - Go to t*ools > library-manager*
    - Install the DFRobotDFPlayerMini library\
      <img width="735" alt="Bildschirmfoto 2022-01-18 um 23 56 25" src="https://user-images.githubusercontent.com/46136690/150093757-cd534909-c3a0-4c68-b674-23c8cf644b69.png">
    - If needed, more info here: [https://www.youtube.com/watch?v=kq2RLz65_w0](https://www.youtube.com/watch?v=kq2RLz65_w0)
- Install the **display** library
    - Install SSD1306 library\
      <img width="735" alt="Bildschirmfoto 2022-01-19 um 00 06 16" src="https://user-images.githubusercontent.com/46136690/150093803-a1bc5a09-6e63-46cb-bf23-458fa0f1d07c.png">
    - Install GFX library\
      <img width="736" alt="Bildschirmfoto 2022-01-19 um 00 06 48" src="https://user-images.githubusercontent.com/46136690/150093867-6726aec8-0eea-45ea-91ba-e47d983c77de.png">
    - If needed, more info here (and also cool other functions of display): [https://randomnerdtutorials.com/esp32-ssd1306-oled-display-arduino-ide/](https://randomnerdtutorials.com/esp32-ssd1306-oled-display-arduino-ide/)
- Enable **wifi** connection
    - Set WIFI credentials by changing code:\
        `const char* ssid = "type Wifi name here";`\
        `const char* password =  "type password here";`
    - Setup firebase and connect it to microcontroller by pasting url:\
        `String url = "paste firebase url here";`
    - I believe no other libraries are needed. If needed, more info here: [https://medium.com/firebase-developers/using-firebase-to-control-your-arduino-project-over-the-web-ba94569d172c](https://medium.com/firebase-developers/using-firebase-to-control-your-arduino-project-over-the-web-ba94569d172c)
- Restart Arduino IDE

## Load code to board

- Connect board via USB to PC
- Make sure that right port is selected under *Tools > Port > {port name}*
- Click on the arrow next to the check mark (upper corner right) to load the code to the board
    - If it doesn’t compile make sure all libraries are installed properly
    - If it doesn’t connect to the board, press the *boot* button on the ESP32 while the code is uploading, this should help
- Once code is uploaded fully you can disconnect the board if you want

## Debugging

- Install Serial Tools in the MacAppStore: [https://apps.apple.com/de/app/serialtools/id611021963?mt=12](https://apps.apple.com/de/app/serialtools/id611021963?mt=12)
- Select the right port and set the bitrate to 115200
- Press connect after you have uploaded the code
- Press reset on the ESP32 board if you want the board to run the code from the top
- In the console you will see if the board connected to the WIFI, if the MP3 player works, ...
