# BlackHoleDiscBlink
Arduino sketch to blink Black Hole disc LEDs.  

455 blinker bulbs are scarce and expensive now, so it makes sense to isolate the 11 disc lamps from the GI and control pinball LEDs with an Atmega328PU microprocessor.  An added bonus is that pinball LEDs give off almost no heat, so will not warp the disc.  You could use a combination of slow/fast blinking LEDs, but they won't look as good because they blink at about the same on/off times and 'sync up'.

Parts list:

<li>1 19-row minimum stripboard
<li>1 Atmega328PU processor 
<li>1 28-pin DIP socket
<li>4 1N4004 diodes
<li>1 7805 regulator
<li>1 1000uF (35v) capacitor
<li>1 10uF (35v) capacitor
<li>.100 headers, connector housings, & contacts
<li>hook up wire
<li>11 warm white 44-base pinball LEDs
      

![blackhole_blinkers](https://user-images.githubusercontent.com/60443687/163850766-168c6913-259c-4e9c-a849-e6f80e6093a9.png)

With an Arduino and a light sensor, I measured the on/off times of several real 455 bulbs to derive the min/max blink times.  I found that most 455 bulbs had on/off times between 100 and 1000 ms.  The sketch randomizes the blink on/off times to each LED about every 10 seconds.
     
The optional motor control board will turn the disc spin motor ON or OFF based on these settings selected using the SET BUTTON:
      
<li>1. ON     -- disc motor is always ON.  
<br>(The Indicator LED is ON)
<li>2. OFF    -- disc motor is ON in Game Play and OFF in Game Over.  
<br>(The indicator LED is OFF)
<li>3. TIMED  -- disc motor is ON in Game Play runs for 10 minutes after Game Over and then turns OFF until a new game is started.
<br>(The indicator LED flashes)

The last mode selected with the SET BUTTON is saved to EEPROM and applied on startup.

![blackhole_motor_ctl](https://user-images.githubusercontent.com/60443687/164757974-5926309e-764a-43fe-82e4-a0109dc9191e.png)

<h1>Game Modification for Blinker Control</h1>

Remove the circuit boards covering the 11 bulbs under the disc.  There are two factory wire braids that connect all 11 bulbs under the disc.  Both of these will need to be completely isolated from the game GI.  After isolation, bypass wires will need to be run to reconnect the remaining GI.  
      
On the isolated bulb holders, one terminal of each bulb holder will need to be completely removed from the factory wire, and a dedicated control line soldered to it.  The other terminals can stay connected on the isolated factory wire forming a common ground.  Gather all the wires and make a molex connector to mate with the corresponding pins on the blinker control board.  Make sure the ground line is connected to the ground pin on the blinker control board.  The arrangement of the control lines does not matter unless you are going to enable the attract routine in the controller.
      
Before connecting to the board, make sure the 11 lamps are completely isolated from the game GI.  Replace the bulbs with pinball LEDs.  The Atmega may be damaged if you try to use incandescent bulbs.
      
<h1>Game Modification for Motor Controller (optional)</h1>
The following modifacations are made to the game to control the motor.  Skip these steps if you do not want to use the motor controller.
      
![image](https://user-images.githubusercontent.com/60443687/214337892-f69e9837-966f-413f-ba51-6cb3903fee8e.png)

Locate the rear third wire from the right, on the A3J3 connector. Connect a 1 foot length of wire to it. This wire is the Control Line (white). 

![image](https://user-images.githubusercontent.com/60443687/214337994-4bc4554d-38d7-42a8-91ab-c1961053d1c5.png)

Locate the A10J10 2-pin connector for the motor. Remove the wires from the connector, and save the connector in your spare parts box. It will not be needed for this modification. 

![image](https://user-images.githubusercontent.com/60443687/214338481-dd0ec882-ce1d-42fc-b28e-0714e4a18500.png)

Use a 3-pin male Molex connector and configure the pins as follows:
1. +V Motor (to mate with 6V wire on female side).
2. -V Motor (to mate with Motor Ground on female side).
3. No pin (this does not connect to the female side).

Use a 3-pin female Molex connector and configure the pins as follows:
1. Combine the game's 6V wire (white/orange) with a 1 foot length of wire (red).
2. Pin 2 is a 1 foot length of wire (blue) for the Motor Ground.
3. Combine the game's ground wire (green/yellow) with a 1 foot length of wire (green). This pin will not connect to the male side. 

Gather the white (Ctl), red(6v), blue(M-), and green(Gnd) wires, and make a molex connector to mate with the corresponding pins on the motor controller board.

      
Youtube video of blinking LEDs under disc.      
      
<div align="left">
      <a href="https://www.youtube.com/watch?v=HBt1XZJ4JSo">
         <img src="https://img.youtube.com/vi/HBt1XZJ4JSo/0.jpg" style="width:50%;">
      </a>
</div>
