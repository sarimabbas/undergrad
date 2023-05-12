## Concept

Our original conception for the AKW takeover project was to create an interactive art installation, in which users and would be invited to interact with various modules, each containing unique sensors. The sensor data would inform visual and audio output in Processing and SuperCollider, which would then be projected onto a display and have sound output out of desktop speakers. Our design was also “elements” based, and we each focused on a different element (fire, earth, air, and water) for our visual design. Unfortunately, our full concept was not able to be fully realized, but I’ll talk about what we did accomplish below.

## Mesh Network

For our Mesh Network, we used painlessMesh, an Arduino library that handles the back end of the mesh network. This worked quite well, and we were all able to connect to the network and send messages. In order to get the messages into Processing and SuperCollider, we used a python script that read the incoming messages from a JSON file, and then converted them to OSC messages to send out.

To use the network on the Raspberry Pi, which we used to host Processing and SuperCollider, we had to use an additional dependency called painlessMeshBoost. This had a similar setup to painlessMesh and similarly used a python script to format the messages.

## Sensor Module

For my sensor module, I focused on the water element. I wanted to do some sonification in SuperCollider, and I wanted to utilise microphones to do amplitude detection. I ended up using three DFRobot Analog Sound Sensors, each mounted inside a wooden box. They were glued to the inside of the box on 3 different sides of the box, and I drilled a hole in each side to expose the microphone. The sensors are wired to the ESP32 and send analog output. Lastly, everything is powered by a 9 volt battery connected to a protoboard. Check out the image below for a look at the box.

![alt text](https://github.com/alexichristakis/akw-takeover/blob/master/mesh/sam-water-input/Pictures%20and%20Video/IMG_6302.JPG)

## SuperCollider Realization

In my SuperCollider realization, I wanted to continue with the elements theme of the installation. I used three sound files, one of fire crackling, an earthquake, and ocean waves. My idea is that each of the sounds would be associated with one side of the box, and making sounds on one side of the box would cause its associated sound file to become dominant. I ended up using a SynthDef to blend all three sound files together, with the amplitudes determined by which sound sensor reads the highest values. Originally, I took readings from all three sensors, normalized them, and simply applied those values to the amplitude. This worked OK, but I found that it was a bit hard to distinguish which side was associated with which sound file, because the readings were often close together. Instead, I ran a check to see with sensor picked up the loudest reading, and set this sound’s amplitude to 0.7 while the others were set to 0.15. This made it much more clear which side was which. I was happy that it worked very well and you could consistently switch between the dominant sound samples by blowing or snapping on the various sides of the box. Additionally, I'm using other data from the mesh network (from my other groupmates) to control low pass and high pass filters on the sound. 

There were a number of difficulties I ran into with the sensors. One was that it was difficult tog calibrate and get consistent readings of the sensors. For example, if I snapped on the left side of the box, the center sensor might actually get a higher reading. I was able to reduce this problem a bit by adjusting the potentiometers on the sensors, however it still persisted. I also found that blowing directly into the sensor was much more consistent than snapping near it. Another solution was to cover the other two sensors with my fingers, which reduced their readings. Through a combination of these techniques, I was able to mostly smooth out the data to a degree where it worked for my application.

Another issue I ran into was false readings from the sensors. Sometimes, even if there was no discernable sound, the sensors would give me false readings, which were quite variable and unpredictable. This problem was heightened when I used the 9 volt battery to power the sensors as opposed to powering them from 5 volts out of the ESP. To solve this problem, I used a threshold to check the readings from the sensors. If they were below the threshold, it's likely that they were false readings and they wouldn’t have any effect. While this might have caused some quieter sound to go undetected, it was actually very effective at filtering out the noise.

## Challenges and Troubleshooting

Due to some last minute issues with our mesh network and Processing, we were unfortunately not able to get our installation fully running. Many of the sensors were working individually, but we were unable to get them all sending data to the same processing script. Fortunately, I was able to get my sensor module working individually, and you can check out a video of it below.

## Circuit Diagram

![alt text](https://github.com/alexichristakis/akw-takeover/blob/master/mesh/sam-water-input/Pictures%20and%20Video/circuit.png)

## Parts List

-   ESP32
-   3 DFRobot Analog Sound Sensors v2
-   Protoboard
-   Wires
-   9 volt Battery
-   Wooden enclosure

