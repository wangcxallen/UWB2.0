# UWB2.0
The dw1000_tx's transmition part takes roughly 2ms while the dw1000_rx_cir's receiving part takes roughly 3ms.

## Overall description

## System configuration
### Enable SPI
API to dw1000 required raspberryPi to enable its SPI:

```
sudo raspi-config -> 5 Interfacing Options -> P4 SPI -> Enable
```

Open the config.txt:

```
sudo nano /boot/config.txt
```

add a line dtoverlay=spi1-2cs
then reboot the system:

```
sudo reboot
```

## Data collection


## Mount exFAT Drives
```
sudo apt-get install exfat-fuse exfat-utils
```
Mounting drive will eliminate all data from the mount point. Thus it's better to establish a new directory. 
```
sudo mkdir /media/headcount*
```
```
sudo mount -t exfat /dev/*** /media/headcount*
```
```
sudo umount /dev/***
```
