# Documentation timeline
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
