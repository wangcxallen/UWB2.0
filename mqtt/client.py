
# coding: utf-8

# In[8]:


import paho.mqtt.client as client
import time, subprocess, shlex, sys
import numpy as np

HOST = sys.argv[1] #235 in arena/199 in home
PORT = int(sys.argv[2]) #1884 in arena/1883 in home
TOPIC = "UWB"
FLAG = int(sys.argv[3])

def UWB_on_Message(client, userdata, msg):
    if(len(msg.payload)):
        speaker = int(msg.payload[0])
        sequence_num = int(msg.payload[1:])
        print( "%s %i" % (msg.topic, sequence_num) )
        if(speaker== FLAG):
            time.sleep(0.3)
            print("say")
            cmd = shlex.split("/home/pi/UWB/dw1000/src/dw1000_tx")
            p = subprocess.Popen(cmd, stdout=subprocess.PIPE)
            print(p.communicate()[0].decode("utf-8")) # If communicate is called, it will wait until the process is terminated
            print("-"*30)
        elif speaker<4 and speaker>-1:
            print("hear")
            cmd = shlex.split("/home/pi/UWB/dw1000/src/dw1000_rx_cir %i" % sequence_num)
            p = subprocess.Popen(cmd, stdout=subprocess.PIPE)
            print(p.communicate()[0].decode("utf-8"))
            print("-"*30)
        else:
            print("End client")
            sys.exit(0)

def on_connect(client, userdata, flags, rc):
    print("Connection returned result: " + connack_string(rc))
    
def on_disconnect(client, userdata, rc):
    print("Unexpected disconnection.")

UWB_client = client.Client("Rpi" + str(FLAG))
UWB_client.on_message = UWB_on_Message
UWB_client.on_connect = on_connect
UWB_client.on_disconnect = on_disconnect
        
def main():
    UWB_client.connect(HOST, PORT, 60)
    UWB_client.subscribe(topic = TOPIC)
    UWB_client.loop_forever()

if __name__=='__main__':
    main()

