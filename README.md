# UDP-to-UART Gateway on Raspberry Pi for nRF5340 DK CLI Control

# Description

This project implements a simple and effective gateway that:

- Receives **UDP packets** from external devices (e.g., mobile phones)
- Forwards the content via **USB serial (UART)** to an **nRF5340 DK** running OpenThread RCP firmware

This allows wireless control of an OpenThread device using standard IP (UDP) over Wi-Fi.

# How to set it up

The UDP_Client.py is the application that running on your PC to send the UDP packets to the RPi. To change the status of the LED on the CLI Device, Users can type in 1, 2, 3. But remember, you can only send one number at a time right now. How should the LED status is changed is not the focus of this project. This project test the possibility of a bespoke RCP-like application.

The UDP_to_SerialPort.py is the application that running on your RPi to receive UDP packets from UDP_Client and forward it to the Serial Port of the RCP DK.

The RCP_DK should be flashed with Sample_RCP and the end node such as a CLI device should be flashed with Sample_CLI. Remember to remove the build folder if you want to turn to another DK rather than nRF 5340 DK.

Thanks for reading!
