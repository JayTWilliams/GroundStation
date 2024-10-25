# Documentation timeline
## Week of 10/23/2024
- EncoderV2 may actually work, edit the code to alternate between new messages to verify this.
  - It does work lul. Verified this by editing the code to alternate between two messages. The reason I didn't see this earlier was because the same message was overlapping so it was hard to tell if anything was changing. The only thing I am uncomfortable with is that instead of transmitting nothing, it transmits a stream of 0s in between transmissions. This wastes a lot of power and bandwidth, so it would be better if nothing were sent. This is more of a reason why using CW would be easier.
- I asked Hausman if he has resources on models for atmospheric attenuation, and he said he would try to look for them in his textbooks and send me what he finds.
- I looked more at the hardware side these past weeks, and thought about them from a microwaves perspective. From my microwaves class, Hausman said that we should start thinking about "distributed" circuit models when our circuits are larger than one-tenth of a quarter wavelength. Using the dielectric constant for the FR-4 material commonly used in PCBs, we get one-fortieth of a wavelength being roughly 9 millimeters. Researching parts at this frequency gives parts with dimensions longer than this, necessitating a microwave design. This also comes with increased PCB cost, as microwave circuits need to be very precisely machined so that characteristic impedances of transmission lines stay as expected.
- Looking at datasheets for parts also showed operating temperature ranges smaller than what would be expected in LEO. To work with this, we would likely need some kind of thermal control for the electronics that is also completely thermally isolated from the experiment.

## Week of 10/2/2024
- It may be possible to set up a framework for another student to properly pass text data to GnuRadio without having to know anything about radios. What I can do is create a dummy flowgraph in GnuRadio, with the file source block and a QT GUI Time Sink, that will merely show what is being read in binary form. To show that it works, it could loop through a few different messages so that we can see if it is alternating. If it is the same message over and over again, it may be hard to determine if anything is changing.
- Another potential reason I thought of is that I may have the 0 and 1 flipped in the 2FSK modulator. Obviously, the arduino wouldn't pick anything up if this is the case. I also noticed that my deviation frequency was half of what it should be, because the receiver code for the arduino gives fsk deviation as 47.6kHz, but analyzing the spectrum shows a deviation of exactly twice that. Even more specifically, we see a spike near the carrier, and another spike at a frquency twice the deviation lower than the carrier.
- I wonder how I could experimentally determine which bit corresponds to which peak. Similarly, I don't think I understand how the GnuRadio block "Frequency Xlating FIR Filter" works. I put in numbers that made sense and fiddled with it a bit, and eventually it worked. The reason I used the block is because most FSK demodulators I found online used this block as an important piece, along with quadrature demod.

## Week of 9/18/2024
- A presentation was put together going over the ground station developments over the summer, and an analysis of the potential reasons that the link is not working.
- The first step was to borrow a friend's handheld radio, to see if the SDR was transmitting. It was transmitting, but not where I expected it to. It only showed peaks where I expected the center frequency to be, and not the actual signal. The signal sits roughly at 2.5MHz at baseband, so if I were to upconvert it by setting the center frequency to 433.92MHz, I would expect to see my signal at 433.92MHz + 2.5MHz = 436.42MHz. When listening in these bands, I couldn't hear anything. This could be the result of two things. One being that somehow the SDR already knows to adjust the center frequency so that it sits in the middle of the signal's bandwidth, but I find this unlikely because from an engineering perspective this comes with many problems. The other issue could be that for some reason, the hardware we purchased specifically to expand the radio's bandwidth is not working as intended/advertised, and is cutting off our signal.
![missingSignals](https://github.com/user-attachments/assets/203e966f-699e-419e-b08a-5677fc347b92)
- Another issue lies in the way that data is passed to GnuRadio. What I want is to pass it a text file, and have it periodically loop it with a one-second pause in between. GnuRadio automatically loops the file, with no pause, creating an unending transmission that the arduino may not like. I tried writing a C program that would feed GnuRadio this text file with a one second delay, but it produces weird results. One program only sends the file once, and another program doesn't do anything, until you stop the program and then it sends a bunch of repetitions really fast.

## Summer Updates (June-August)
- Ham Radio licenses were obtained over the summer, so now I have a call sign and can transmit. However as far as testing within the building goes, it is a Faraday cage and signals have a very hard time getting out.
### Attempt at Transmission
- Two GnuRadio flowgraphs were developed over the summer, one being a simulation of transmitting and receiving, and another being an actual transmission using the SDR.
- The goal of the simulation was to modulate and demodulate a "signal" using the same parameters that the sucessful receiver used, assuming that our arduino will hear it in the same way. The simulation works perfectly, however the actual transmission yields no results from the arduino. This is extremely hard to troubleshoot, because the arduino feels like a black box that we think expects a certain input, but is giving no results. When designing the receiver with the SDR, there was so much feedback to understand what we were doing wrong.
- Fundamentally, we expect that the arduino wants to hear a 2FSK-modulated signal, with a certain code word at the beginning.

## Week of 5/3/2024
- Decoder V3 had a bug that prevented proper clearing of a buffer, so the memset() function was adjusted to use a fixed buffer length instead of the sizeof function
- The S parameter graph given in the antenna VNA test report is incorrect, the graph presents S11 in 10\*log10 instead of the correct 20\*log10

## Week of 4/19/2024
### Real-Time Bit Processing
- The final product is visible in the BitProcessing/FinalDecoder directory, as decoderV3.c
- In summary:
  - decoderV1 does not attempt to synchronize with the sync word that the transmitter gives, so it is a 1/8 chance that the message is readable.
  - decoderV2 is able to sync with the sync word, using its special sequence, and then printing the resulting characters, however the length of the message is hardcoded instead of read off the transmission, awkward for sending messages of different length.
  - decoderV3 is able to read the message length byte sent directly after the sync word, and then read that many bytes of data to print a message of any length.
- The decoder uses a state-machine-type architecture:
  - State 0: program is waiting for a bit transition, upon which the program will transition to state 1.
  - State 1: program is filling a buffer bit by bit, while scanning it to see if thre register contains the desired sync word, then transitioning to state 2. If no sync word is found within an arbitrary amount of bits, then it is reported as noise and the state transitions back to 0.
  - State 2: program is filling a register with the next 8 bits, which are packed to get the length of the incoming data, then transitioning to state 3.
  - State 3: program gets the expected length of the data, and fills up another buffer with bits until the counter reaches the expected length, upon which the bits are packed into bytes and printed to the console. Then the state transitions to 0, continuing the cycle.
- An image of the final GNURadio block diagram and terminal result is visible in the same folder.
- Many memory issues needed to be dealt with that I still don't fully understand, but it isn't worth discussion as it is not important to the project.
- This will be the demo for the VIP Showcase.

### Interfacing with GNURadio
- Since there seems to be a lack of built-in processes for data handling in GNURadio, and associated documentation, some C programming was necessary to process bits.
- I learned about a FIFO file, aka a named pipe, which is useable on UNIX systems for inter-process communications, perfect for this application.
- The goal is to gather the extracted bits from GNURadio, using a file sink block that writes into a FIFO file to be read by a C program.
- So far, I have GNURadio set to pack the 8 bits into a byte (regardless of phase), and write the bytes to the FIFO file.
- The C program then treats these values as unsigned chars, and prints them to the terminal, occasionally getting the desired "Hello World" message when phases line up.
- For this specific application, an approach may be to detect when the sync word begins, set up a fixed-length buffer to store the incoming bits (including the sync word), and then remove the sync word to extract the data.
- The issue with this approach is that sometimes the first few bits of the sync word get lost due to the symbol sync block in GNURadio taking some time to lock, potentially causing more phase issues.
- It may be worth having the symbol sync and raw bits side by side, so I could trigger off of the raw bits and then used the synced data for analysis.

### Third NOAA reception test (4/14/2024)
- Once again, only static.
- There was some strange interference at the band of interest, noticeable on GQRX, causing strange rippling in the image.
- I am unsure of the source of this interference, but hopefully it is gone in the next test.
- This test was performed much later in the day when the terrace was closed, so it was indoors, harming the signal, if any was even left.

## Week of 4/12/2024
### Second NOAA reception test
- This test was slightly more successful, as sync lines and other artifacts are more clearly visible, however no detail of the Earth is visible.
- The bandwidth of the digital filter was adjusted during capture from 2\*110 kHz to 2\*40 kHz, which could be responsible for the large rectangles.
- Friday was a very foggy day, likely interfering with the quality of the signal.
- Unfortunately, the terrace guards were 20 minutes late, causing me to miss about 70% of the satellite's pass.
- The resulting image can be found in the NOAA images folder.
- Future passes:
  - NOAA 15: Sunday April 14th, 7:24PM (terrace closed but it is a VERY good pass, is worth attempting indoors from 8th floor window)
  - NOAA 18: Wednesday April 17th 11:00AM
  - NOAA 18: Friday April 19th, 10:36AM
- It is worth noting that the terrace has no power outlets. A long extension cord must be brought to power the radio.

## Week of 4/5/2024
### First NOAA reception test
- On Friday morning, the antenna was brought up to the 8th floor, but unfortunately an earthquake struck, rendering the terrace out of service due to safety concern, so a nearby room with a large window was chosen instead.
- NOAA FM being a low power VHF signal, it was not able to get through the glass, let alone the metal plating on the roof of the school. Thus, the 15 minute wait resulted in a pure static image. There were some scan lines on the left side, showing hope for proper reception.
- The chosen satellite for the experiment was the NOAA 18. The other satellites NOAA 15 and 19 are perfectly fine too, but NOAA 18 had the best pass at the time.
- It is extremely difficult to find a pass that fits into schedule, that is happening while the terrace is open. Proper direction is also important, as south to east is most preferred.
- The next best one looks to be NOAA 18, April 12th 10:29 AM.
- Unfortunately, the best (orange) passes are late at night, when the terrace is closed.
- These predications can be found on the N2YO website. Search for the satellite and click the link for 10-day predictions.

### Arduino Reception
- Better reception was acheived following the revelations from earlier, however still not consistent.
- The way to resolve this now is to decimate and interpolate appropriately, and construct a decision algorithm from the samples per symbol. This is a pain to do in real time, let alone in post using MATLAB.
- Since the CC1011 has an SMA connection, perhaps with some careful power consideration, it could be directly connected to the ground station to receive digital signals, functioning as a mini-SDR, but obviously for a fixed purpose.
- The Tx in the code can be done purely in bits, a helpful abstraction that can be used to send things other than text, like pictures, useful for the experiment. It could also be that the pictures are pre-defined in memory, reducing the complexity.

## 4/2/2024
### Playing with Arduino UNO
- Joseph Chico and I experimented with the Arduino Tx/Rx system, with little success, but some progress.
- A GnuRadio program was made to store a file containing raw binary information, which was then processed in MATLAB Assuming my MATLAB code is correct, there are issues with the GnuRadio program. Some issues include the wrong sampling rate and/or the center frequeny Xlating block. Quadrature demod block likely no longer has the correct gain. Multiplying by -1 should be verified if it actually flips bits.
- However when just making sure packets were still being sent using GQRX, the transmission seemed to periodically fade in and out in proportion to their transmission rate. This was also present in GnuRadio when plotting the FFT from just the source data, but after the WBFM block, this fading in and out disappeared.
- More on the Frequency Xlating block, it makes more sense now to set the center frequency to half of the frequency deviation, as on GQRX it looked like a LSB signal. Upon downconversion, the LSB part of the 2FSK signal will appear higher than the center frequency, explaining the bit flip when using quadrature demod!!
- Other than these revelations, noise is still a significant issue which appears to be interfering with bits.
- It is also worth trying the tiny antenna that came with the arduino kit with the SDR.

## Week of 3/29/2024
### Ham Radio Licenses
- Teams post was made to gather people interested in taking the exam and getting certified.
- This is an absolute must for the future of this club.
- Three books were purchased to study from over the summer.

### still waiting for terrace to open
- In this time, this github was created for more focused timelines.

### new parts
- Some new parts were ordered due to the failure of the original LNA.
- We decided to try again with the other LNA, as it has a DC input which is much easier to power with the bias tee that we already have.
- The bias tee is a purely passive device, so it is less sensitive to heat and should be more resilient to soldering.
- An N-type connector cable was also purchased so that the LNA does not have to sit directly on the antenna and can be held safely in a foam-padded box.
- The datasheet for the LNA is in the datasheets folder, named LNA2.pdf

## Week of 3/22/2024
### Antenna built
- The antenna was finished over Spring break, following the newly aquired lab space in 708
- The specs for the antenna can be found in the Fall23 folder, in the BasicSpecSheet.pdf file.
- The datasheet promised a very wide frequency range, without supporting graphs, so it was necessary to test with the VNA to find a working frequency range and ideal places with small scattering parameters.
- The results can be found in the Spring24 folder, titled VNAantennatest.pdf

### NOAA and other CubeSats reception plans
- In order to receive the delicate signals from satellites, roof or terrace access is required.
- For this reason we must wait until after April 1st to take our equipment up to the terrace.
- In the mean time, some discussion has been done on testing our own transmissions with very small power signals from an arduino.

## Semester start - 3/15/2024:
### First few weeks of the semester were spent attempting to interface with the SDR:
- Involved troubleshooting which can be found in the SDRsetupdocs.pdf file, in Spring24.
- Sources can be found in that doc, leading to Ettus's manual.
- Main issue was that the laptop needs a dedicated ethernet port, or else complications will arise.
- For this reason, along with OS preferences, a laptop for the ground station was set up with Fedora.

### To test reception, we started with broadband FM:
- details in FMProgress.pdf file, in Spring24 folder
- gqrx and GNURadio were used, which requires the UHD dependency to interface with the N200 SDR
- dependency is a massive pain to install on windows, another reason to just have a dedicated laptop with Linux

### RF Devices
- The first LNA, a minicircuits one, was bought due to the desired one being cancelled.
- Datasheet can be found in the Datasheets folder, under the name LNA.pdf.
- This device required soldering to connect power and ground, instead of the planned bias tee connection.
- Application notes by minicircuits can be found in the Datasheets folder, under the name turretterminalpins.pdf.
- After soldering, it is possible that the device is broken as when connected to a power supply with 5V + 150mA DC, the supply starts to complain.
- Next time, it may be wiser to ask a PCB lab employee if they can solder for me.
- The bandpass filter is still in working order, it was tested with the VNA and showed the same result as described on the datasheet. It is less sensitive because it is a passive device.

### I/Q data storage and NOAA reception
- Important for NOAA image reception, incoming I/Q data needs to be stored so that it can be losslessly converted to a .wav file
- Details in FMProgress.pdf file in the Spring24 documentation folder.
- Even though NOAA uses FM, converting I/Q data is important as simply "recording" the audio can lead to undesired quantization effects
- On the topic of quantization, there seems to be an issue with the current program as GNURadio keeps telling me that buffer overflows are occurring, sometimes causing the baseband audio to "skip"
- I am unsure how much this will impact the resulting image.
- There exist open-source NOAA signal decoders, so little effort is required on our part in the decoding process.
