
# coding: utf-8

# In[ ]:


import paho.mqtt.publish as publish
import time
import numpy as np
import sys

HOST = sys.argv[1] #235 in arena/199 in home
PORT = int(sys.argv[2]) #1884 in arena/1883 in home

def main():
    epoch = 0
    squence_num = 0
    while(epoch<40):
        for i in np.arange(4):
            print("RPI %i speaks..." % i)
            print("MSG %i" % squence_num)
            print("-"*30)
            publish.single("UWB", "%i%i"% (i, squence_num), hostname=HOST, port=PORT)
            squence_num += 1
            time.sleep(0.5)
        epoch +=1
        
if __name__ == "__main__":  
    main()

