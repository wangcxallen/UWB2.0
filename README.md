# UWB2.0
The dw1000_tx's transmition part takes roughly 2ms while the dw1000_rx_cir's receiving part takes roughly 3ms.

## Find RPI0
```
sudo nmap -n -sn 192.168.1.1/24
```
Mac address table: 

`B8:EE:65:03:51:29` -> hostap(wifi)

`B8:27:EB:90:4B:A4` (Raspberry Pi Foundation) -> rpi0

`B8:27:EB:B5:64:EA` (Raspberry Pi Foundation) -> rpi1

`B8:27:EB:58:9D:C8` (Raspberry Pi Foundation) -> rpi2

`B8:27:EB:F9:66:89` (Raspberry Pi Foundation) -> rpi3

`B8:27:EB:22:BF:7A` (Raspberry Pi Foundation) -> rpi4

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
