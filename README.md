# UWB2.0
MQTT + Batch UWB
The dw1000_tx's transmition part takes roughly 2ms while the dw1000_rx_cir's receiving part takes roughly 3ms.

## Mount exFAT Drives
```
sudo apt-get install exfat-fuse exfat-utils
```
```
sudo mkdir /media/headcount*
```
```
sudo mount -t exfat /dev/*** /media/headcount*
```
`
sudo umount /dev/***
`
