# Documentation timeline
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

## Week of 3/22/2024
###
