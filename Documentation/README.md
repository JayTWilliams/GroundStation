# Documentation timeline

## Week of 4/28/2025
- The PCB is finished!
- <img src="https://github.com/user-attachments/assets/427e6758-1a04-4f91-8c12-50eac7fedb95" width="500"></img>
- It doesn't work, but not all is lost! There are several problems I seem to have isolated so far:
  - The ground pin next to 3V3 on the bottom of the PCB doesn't work. this is because I forgot to label that net as ground in Altium. I can just tie a wire from that pin to some other via with a ground connection.
  - RF switch IC442 is non-functional. During building, it was placed incorrectly but later rotated into correct position. The solder job is a tad messy, and things could be shorting underneath. I bought more of the chip so its worth just using another one.
  - The power supplies produce no voltage. I know that they should work because I built the same circuit out of larger components, as is visible from previous documentation. I suspect then, that this isn't because of the schematic being wrong, but the control switches are not turning on as expected. I need to verify this, as well as check the datasheets for Q1 and Q2.
    - It could also just be that the voltage regulator expects around 12.5 volts at the input (for an 11 volt output), and with the gate voltage being 3.3 volts, the input voltage to the PCB should be at least 16 volts to overcome the threshold. The board was initially tested with 15 volts.
- The things that do work:
  - Filter FL1 and switch IC441 are functional. I know this by doing a continuity test on the signal line across the switch, and also plugging the PCB into a VNA and seeing the characteristic reflection response of the filter.

## Week of 4/21/2025
- The voltage regulator (LM317) to be used on both the ground station and RF sides of communication has been mounted on a test testup:
- <img src="https://github.com/user-attachments/assets/58a7b698-4e16-4fec-bac3-8daa7d68f305" width="500"></img>
  - It works as intended, with the potentiometer's purpose being to tune the output voltage.
  - The row of wire mounts on the left and right serve as power and ground connections, where the top-most connection is power, and the bottom-most connection is ground. The input is on the left side, and the output is on the right side.
  - Attaching various load resistances at the output do not change the output voltage, meaning that the device works as intended.
  - During testing, a typical breadboard 30-ohm resistor burned out immediately when 11 Volts was applied across (367mA).
- PCBs and all of its components have arrived, but I need to note several issues that need to be ironed out for prototype 2:
  - Power and RF circuitry should be on separate boards.
    - Having DC lines sitting below RF lines can cause coupling, and poorer RF power transfer.
    - Having power supplies sit directly opposite of amplifiers can lead to a LOT of heat dissipation in one area, likely leading to burnout if the amplifier is operated near saturation.
    - Separate boards will also allow for better layouts, and the current design requires an extra wire connecting two 3V3 vias. This leads to bad inductance.
    - Avoid double-sided designs as much as possible for RF boards.
    - For the first test, it may be worth building two boards, but splitting up the components such that one board is for power, and another board is for ground.
  - Transmission lines should be properly impedance matched. Using the greater than one-fourtieth of a wavelength rule of thumb to start thinking about distributed models, the circuit's transmission lines sit at around a one-twentieth of a wavelength in length. Because of this, the lines are worth simulating in ADS with the S-parameters of the associated devices. The high transmit power of the PHA-202+ will cause serious reflections if the lines are not properly matched.
  - All of the passive components need to be double-checked for tolerances regarding maximum voltage and current.
- The first prototype for the PCB is uploaded to the Spring25 folder. It contains the spectral plan, and all associated Altium files.
- <img src="https://github.com/user-attachments/assets/83711c78-dff6-48fb-a035-656d3cebe394" width="400"></img><img src="https://github.com/user-attachments/assets/24801060-b98e-452f-ae0d-44c63768961f" width="400"></img>



## Week of 3/31/2025
- Finished PCB schematic, need to double check that all parts are avaliable. A single inductor on the Tx amplifier will need to come from coil craft, but all other parts should be available from Mouser.
- Need to due the layout soon, but need to verify the PCB with someone more familiar with Altium to make sure that it is layout ready.
- Something to note, is that no RF switches were available from minicircuits that fit our specifications exactly, so the compromise is to use a switch with the understanding that if Tx is on when it shouldn't be, the switch will break.

## Week of 3/10/2025
- Performed a super basic power test with the CC1101 and SDR, the results of which are displayed below:
- ![tone_sequence](https://github.com/user-attachments/assets/1a2cd9af-2d1f-4b69-8e68-b15108b057da)
- ![tones(-30to-10)](https://github.com/user-attachments/assets/f72bcf1d-9bbc-4cc8-8fb9-0b19dfa551ae)
- Each peak is spaced 10 kHz apart, with an increasing power level with increasing frequency. There is an evident linear relationship, although let it be known that the power levels are not equally spaced.
- The first plot is the CC1101 cycling through all its power levels, and the second plot is individually running single-frequencies at various power levels.
- These two methods very clearly give different results.
  - One possibility could be that because of the rapid cycling over time, PSD is less concentrated in each peak.
  - Another possibility is that because the tones are not "clean" in the sense that they turn on and off quite rapidly, which could also contribute to a lower PSD.
    - This is supported by the fact that the individually transmitted peaks are higher than the cycled ones.



## Week of 3/6/2025
- Notes from the VIP presentation (See VIP030525)
  - The SPDT power switch on the high-level PCB block diagram should be split into separate SPST switches, with an active-low configuration. This is so that if a switch fails, it will fail to the "on" state and although the amplifier will continue to burn power, it will at least be functional. However, switches can also "latch-up"
    - Upon more research, transistors typically fail to short-circuits, meaning that a reflective switch (what will be used for the power switch) will fail by turning the FET/diode into a short circuit, effectively leaving the switch in a permanently "on" state.
  - Ground-level transmit BPF is looking necessary to suppress harmonics.
    - FCC regulation defines your occupied band as the one where 99% of your signal power lies. For a 20dBm carrier and a -20dBm harmonic output, spurious signals outside of our intended band are 0.01% of our total output power, comfortably satisfying FCC regulation and thus not requiring this BPF.
  - <img src="https://github.com/user-attachments/assets/d1f164ec-558e-495a-981b-1457a603b88e" width="500"></img>
- The Altium circuit schematic is almost finished, and all that is left is to add the power circuitry. I would like to perform more tests with the voltage regulators, because it is somewhat unclear how the different output voltages are selected.
- Some changes were made to the circuit:
    - Upon trying to connect the Tx and Rx lines to the single coax port, I realized that there needs to be a switch so that power is distributed correctly, and so the ports are matched. Thus, another of the same switch was added between the coax connection and the Rx and Tx lines. This prompted a discussion on if the output switch was necessary, and if we could get away with using a power divider. A quick conclusion was reached that dividing the power of the already weak incoming signal power is unacceptable.
    - The more I look at the circuit, the more it bothers me that there is an LNA being used in saturation to transmit a signal. It just seems improper and not what the LNA was designed for. Our use case still sits well below the absolute maximum ratings, but it seems like bad design. There is an amplifier (PHA-202+) that can get us up to a 30dBm (1W) output power, but it requires +11V VDD, meaning that there will be two seperate power supplies. This will also require a different RF switch to be used, as 1W of power flowing through the current one (HSWA2-30DR+) surpasses the absolute maximum rating.
 - I need to do noise floor research.
   - Brilliant idea:
   - ![image](https://github.com/user-attachments/assets/8de54f84-0ad6-4c67-8b47-e30c52519daa)
   - https://dsp.stackexchange.com/questions/83195/what-a-sdr-can-and-cannot-tell-about-a-received-signals-power-energy
   - Use a tranceiver (like a CC1101) to transmit two signals of different power levels. Then assuming the SDR receives linearly (it damn well should for an $1800 daughtercard) we can find this constant of proportionality. Using this constant, we can make a (very) rough estimate of the noise floor as seen by the SDR.



## Week of 2/25/2025
- I ordered some new SMA cables for the SDR, a voltage regulator to drive 5V and 200mA, and some throughhole USB mounts.
  - These have arrived, and I have replaced the cables in the SDR. I also attempted to solder the voltage regulator into a makeshift mount, but was ultimately unsuccessful and may have ruined one of the chips. Two more remain, but I would rather have a professional (Dino) do the makeshift mount, or design a PCB specifically for it.
  - Other PCB designs lay in the future for this semester, so if they can be ordered simultaneously it would be great.
- Began work on the spectral plan and component shopping.
  - Looking at affordable amplifiers for the ground station, it is not in the budget to get amplifiers with saturation points around 40 dBm. The ones we can afford can produce around 21 dBm, and the SDR can transmit at 20 dBm, so there is no point complicating the transmitter design on the ground.
  - There is already a decent reciever chain on the ground, consisting of an LNA and a BPF. This may need some additional amplification based on some rudamentary calculations.
  - Some Tx and Rx amplification will be necessary on the satellite's end, because no more Tx amplification can be done on the ground. I've found some cheap amplifiers and filters that should be easily implemented on a PCB.
- Because soldering the fancy voltage regulator was a flop, it would be worth trying to design a circuit using a cheaper voltage regulator.
  - Here is one that we have in the lab: https://www.ti.com/lit/ds/symlink/lm79.pdf?ts=1740476018703
  - The datasheet provides some example circuitry, with the example of interest is the current source in figure 4.
  - The design problem is to try to get a 5V fixed output with no more than 190mA of current. To test this, build the circuit on a breadboard and exchange a bunch of load resistors and measure the current through each one. Once the one that provides 150mA is found (should be an RL of around 33Ohms), take note and start testing lower and lower resistors to see how much current is drawn. See what happens if you try to draw more than 1.5 Amps out of it.
  - Deliverable: A circuit schematic, an actual circuit on a breadboard, and a plot of current over load resistor.
  - If the current source provides more than 200mA of current, it doesn't mean you failed. Just focus on generating the plot and getting a 5V fixed output.

## Week of 2/10/2025
- Tested the LNA with the VNA, and it works! It provides around 22dB of gain, which is lower than the advertised 24dB. This measurement was made using a power supply hooked to the VDD and GND pins of the LNA, set to 5V and 150mA. Some figures are below:
  - <img src="https://github.com/user-attachments/assets/8f0bf2a5-ab2f-4b57-afdf-ccedac4044af" width="500"></img>
  - The device plugged into the VNA with its VDD and GND pins connected to the power supply.
  - <img src="https://github.com/user-attachments/assets/422c94a4-0079-4d61-86ee-b7a14509ec04" width="500"></img>
  - The power supply setup, with the voltage and current shown.
  - <img src="https://github.com/user-attachments/assets/9a5804d3-05f4-440f-a6fb-7186c3a7f7c1" width="500"></img>
  - The results after saving the data and running it through MatLab to get some nice figures.
- There were some issues, however. While screwing in the VNA ports to the LNA, sometimes the power supply would short out and provide no voltage. This was concering, as the VNA is sensitive and shouldn't have any external power applied to it. Fiddling with the device and cables a bit to straighten everything out seemed to fix the issue. It may be worth redoing the soldering joints because it could be that there is a stray wire from VDD touching the chassis of the LNA.
  - Tore off the old wires. Going to solder new ones. Shorting on the chassis while connected to the VNA is unacceptable.
  - Re-did the soldering, no issues with shorting and LNA still works!
- Now that I have proved the LNA still works, I would like to build some circuitry that can use the 6V lead-acid battery to power the LNA. This would require a voltage drop and a current-limiter.
- The BPF at the output of the LNA should be sufficient for the Rx RF frontend. So it would be nice to have some kind of voltage regulation circuit, along with a battery and the RF components all screwed to a board and kept inside the protective case.
- The Jackery portable power bank can supply 5V through a USB port. Thinking about using this for powering the LNA by hooking it up to a USB port with throughhole connections on a perfboard with a voltage regulator. There will also be testing done without the USB port, so it makes sense to also have a SPDT switch to flip between the modes.
- Note to self: consider implementing the Rx part of link with an actual autocorrelation

## Week of 2/3/2025
- Tried running a completely vanilla build of the GnuRadio OOT module, and still got the same error. At this point it is probably not my fault.

## Week of 1/27/2025
- Added a convenient shell script so that connecting the laptop to the SDR is easier. On the laptop, this file can be found in Documents -> SDR utilities. Simply run it in terminal as:

  ``` sudo sh connectToSDR.sh ``` and enter in the password to the laptop.
  - Please allow a bit of time for the laptop to report a ping as missing. If the SDR fails to connect, some troubleshooting is required. Refer to the SDR setup docs, which can be found in the Spring '24 documentation folder.
- I tried following the GnuRadio guide on OOT modules exactly, creating the same one they did, but the same cmake error came up. This tells me that it is likely something went wrong with the installation of dependencies. Need to look into this further.

## Week of 12/11/2024
- Unfortunately I have been very late on delivering power budgets to Oscar, but a very rough estimate has been created using some theory I have picked up in my Microwaves course. The basic calculation involves Friis radio link formula, and some guessed parameters. These are the minimum received signal power (-90 dBm), and a middle-ground estimation of LEO orbit as 500 km. Using the SDR's maximum transmit power of 20 dBm, provided by the datasheet, and factoring out the 13 dBi antenna gain, we need a system gain of 16.18 dB. If this is all done on the transmitter's side, then we would need to transmit 4.15 Watts.
- However, because the antenna is also a lossy component, it will have some reflections when trying to give it an incident voltage. This is modeled by s-parameters.
  - Last semester we empirically obtained the s-parameters of the antenna over a bandwidth, but the attached cable was very long and contributed significantly to loss and inductance. Now that I will be the TA for EMT, I should have much more consistent access to the VNA and can make more tests.
  - Knowing how much power is reflected is important when transmitting with significant power, because reflected power can damage previous components. If this power makes it back to an amplifier or the SDR, the internal components can break. If reflections are bad, we can purchase a circulator, which is a heavy ferrite component that only lets signals through in one direction, by applying a DC magnetic bias. It is a three-port device that can redirect reflected power to a load resistor mounted on a heatsink.
- A presentation I made goes into more detail on this, and most of the information in it is sourced from Pozar's Microwaves chapter 14.

## Week of 11/21/2024
- I have settled on creating out-of-tree (OOT) modules in GnuRadio to accomplish proper data delivery. The idea is to copy the source code of the basic "file source" block, and simply add a parameter that allows it to periodically sleep and not transmit anything.
- This will allow us to switch between Tx and Rx properly, as well as be more efficient with power and bandwidth. A constrant stream of '0' means that we are transmitting a constant frequency for no reason, wasting power and clogging up the available spectrum for other users.
- GnuRadio provides a guide on how to do this, titled "Creating C++ OOT with gr-modtool" : https://wiki.gnuradio.org/index.php?title=Creating_C%2B%2B_OOT_with_gr-modtool
  - The first attempt at this has been unsuccessful, meaning that it failed when trying to compile it with CMake, as per the instructions. The error was very cryptic and did not give any markers specific to the project that may have caused it. The error is given below:
```
cmake: /builddir/build/BUILD/libuv-v1.47.0/src/unix/process.c:972: uv_spawn: Assertion `!(options->flags & ~(UV_PROCESS_DETACHED | UV_PROCESS_SETGID | UV_PROCESS_SETUID | UV_PROCESS_WINDOWS_HIDE | UV_PROCESS_WINDOWS_HIDE_CONSOLE | UV_PROCESS_WINDOWS_HIDE_GUI | UV_PROCESS_WINDOWS_VERBATIM_ARGUMENTS))' failed.
Aborted (core dumped)
```
  - It may be worth trying to just recompile the same "File Source" block and see if I get the same error, and start debugging from there. The issue is that I fear that name conflicts will happen, and the things I changed the most in the first attempt were just names, and extended the function parameters.
  - Theres just so many adjustments to be made for such a simple task, and a lot of them are just guessed by looking at the code and putting in what seems to make sense.

## Week of 11/20/2024
- When looking up if GnuRadio can help us switch between Tx and Rx, a stackoverflow answer was helpful. For a half-duplex communication link, we can set both the USRP source and sink blocks to the same antenna. When we transmit, the USRP will automatically pause its reception. This makes switching between the two states much easier to the point of abstraction.
- The only step left is to get a better method of inputting data, because at the moment when no data is transmitted, a constant sequence of '0' is being sent. Also the counting is messy.

## Week of 10/23/2024
- Transmission success! (kind of).
  - After setting the bandwidth of the USRP sink to 5MHz, and doubling the frequency deviation, I managed to get results from the arduino. I see "Hello, World!" coming in now, albeit most transmissions are garbled, but occasionally it is clean. I also lifted the arduino off of the lab bench and onto a notebook so that the table's surface wouldn't interfere, and it gave better results.
  - Now I need to find a way to clean up the signal. I would also like to have the periodicity of the tranmissions to be smoother, as right now it feels a little uneven. In the C code, I have it set to be 0.1*CLOCKS_PER_SECOND, which should transmit every tenth of a second. When I first run the program, it does this, but after about 6 iterations it slows down to about once per second.
  - I will work on a formal report and presentation for this.
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
