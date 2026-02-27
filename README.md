# Project Overview: High-Accuracy PPG Heart Rate Monitor-
This project demonstrates the development of a Photoplethysmography (PPG) device utilizing a microcontroller and the TCRT5000 infrared reflective sensor. While the TCRT5000 is often used for line tracking, this implementation leverages its infrared emitter and phototransistor to detect volumetric changes in blood circulation.

How it Works
The system operates on the principle of optical detection. The TCRT5000 emits infrared light into the tissue (typically a fingertip). As the heart pumps, blood vessels expand and contract, altering the amount of light reflected back to the phototransistor.

Key Components & Methodology
Sensor: TCRT5000 (Infrared Emitter and Phototransistor).

Processing Unit: Microcontroller (e.g., Arduino/ESP32) for analog-to-digital conversion.

Signal Conditioning: The raw analog signal is processed to remove "noise" (DC offset from ambient light) and isolate the pulsatile AC component.

Algorithm: A custom peak-detection algorithm calculates the time interval between pulses to determine the Beats Per Minute (BPM).

Performance & Validation
The defining achievement of this project is its reliability. To verify the system's precision, the output was benchmarked against a medical-grade Pulse Oximeter.

Result: The BPM readings from this DIY PPG system showed near-identical correlation with the commercial device, proving that high-level diagnostic accuracy can be achieved through optimized hardware-software integration.
