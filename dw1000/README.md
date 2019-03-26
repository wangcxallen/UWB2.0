# dw1000_rpi
DW1000 &amp; RPI App

Please download DW1000 API from http://www.decawave.com/support/software. Copy
api files (decadriver) to src. Make and enjoy!

# Instructions

# Quick

1. `sudo rpi_config` -> 5 Interfacing Options -> P4 SPI -> Enable
1. add a line `dtoverlay=spi1-2cs` to `/boot/config.txt`
1. `cd dw1000_rpi/src`
1. `make clean`
1. `make <app_name>`

## Compile

    make <application_name>

The following applications are currently implemented:

1. `dw1000_tx`: simple periodic transmitter. Takes no parameters.
2. `dw1000_rx`: simple receiver that continuously listens for packets. Takes no parameters.
3. `dw1000_rx_cir`: like simple receiver but also outputs the CIR (channel impulse respone) for each reception to the console. Takes no parameters.
4. `dw1000_twr_resp`: adapted ranging application using one polling and one response message. Also outputs the entire CIR in a file. Takes 2 parameters:
5. 
    - `INIT` or `RESP`: Choose which device is the INITIATOR or RESPONDER
    - `<exp_number>`: An unsigned integer used for naming the output.
    
    Output files have the naming scheme `exp<exp_number>_msg<msg_number>_I/R.csv`. The `<msg_number>` is modulo 256.

# Known Quirks

# Code Sources

The Decawave Software API is provided by Decawave Ltd. Please refer to `Decawave_Disclaimer.txt` for further details
