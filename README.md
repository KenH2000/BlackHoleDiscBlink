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

<div align="left">
      <a href="https://www.youtube.com/watch?v=HBt1XZJ4JSo">
         <img src="https://img.youtube.com/vi/HBt1XZJ4JSo/0.jpg" style="width:50%;">
      </a>
</div>
