#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Mar 25 17:49:15 2019

@author: wangchenxi
"""

import paramiko, shlex, subprocess

def main():  
      
    arena = ['192.168.1.209',
              '192.168.1.209',
              '192.168.1.209',
              '192.168.1.209',
              '192.168.1.209']
    
    coffee = ['192.168.1.209',
              '192.168.1.200',
              '192.168.1.200',
              '192.168.1.200',
              '192.168.1.200']
    
    headcount = ['10.10.10.176',
              '10.10.10.105',
              '10.10.10.149',
              '10.10.10.196',
              '10.10.10.117']
    
    HOST="10.10.10.207" #235 in arena/199 in home
    PORT="1883" #1884 in arena/1883 in home
    
    server = headcount
    username = 'pi'  
    password = 'raspberry'
    rpi = [0,1,2,3,4]
#    misbehave = [] # This part is not finished yet
    misconnection = []

    for idx, hostname in enumerate(server):
        rpi[idx] = paramiko.SSHClient()  
        rpi[idx].set_missing_host_key_policy(paramiko.AutoAddPolicy())
        try:
            rpi[idx].connect(hostname=hostname, username=username, password=password, timeout=2)
            stdin, stdout, stderr = rpi[idx].exec_command("python /home/pi/UWB/mqtt/client.py "+HOST+" "+PORT+" %i" % idx) # Set flag of each Rpi
            # This function just pass through the command. It doesn't wait for it to end.
#            misbehave.append(idx) # This part is not finished yet
        except:
            misconnection.append(idx) # Establish a list of failure connected IP addresses
    
    if misconnection!=[]: # If any one of the client is not working, reject the program and print the error info
        print("Misconnection:")
        for idx in misconnection:
            print("%i:" % idx + server[idx])
#        print("Misbehave:") # This part is not finished yet
#        for idx in misbehave:
#            print("%i:" % idx + server[idx])
        
    else:
        cmd = shlex.split("python -u host.py "+HOST+" "+PORT)
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE)
#        print(p.communicate()[0].decode("utf-8")) # If communicate is called, it will wait until the process is terminated
        for line in p.stdout:
            print(line.decode("utf-8"))
    
    for idx, hostname in enumerate(server):
        rpi[idx].close()
    return
        
if __name__ == "__main__":  
    main()  