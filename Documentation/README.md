## Documentation timeline
#Everything as of 3/15/2024:

First few weeks of the semester were spent attempting to interface with the SDR:
-Involved troubleshooting which can be found in the SDRsetupdocs.pdf file, in Spring24.
-Sources can be found in that doc, leading to Ettus's manual.
-Main issue was that the laptop needs a dedicated ethernet port, or else complications will arise.
-For this reason, along with OS preferences, a laptop for the ground station was set up with Fedora.

To test reception, we started with broadband FM:
-details in FMProgress.pdf file, in Spring24 folder
-gqrx and GNURadio were used, which requires the UHD dependency to interface with the N200 SDR
-dependency is a massive pain to install on windows, another reason to just have a dedicated laptop with Linux

