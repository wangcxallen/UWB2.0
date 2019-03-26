#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Mar 25 23:31:29 2019

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
    
    wireless = ['192.168.1.1',
              '192.168.1.1',
              '192.168.1.212',
              '192.168.1.205',
              '192.168.1.194']
    
    server = headcount
    username = 'pi'  
    password = 'raspberry'
    rpi = [0,1,2,3,4]
    misconnection = []

    for idx, hostname in enumerate(server):
        rpi[idx] = paramiko.SSHClient()  
        rpi[idx].set_missing_host_key_policy(paramiko.AutoAddPolicy())
        try:
            rpi[idx].connect(hostname=hostname, username=username, password=password, timeout=2)
            stdin, stdout, stderr = rpi[idx].exec_command("cd UWB\n"\
                                       "cd data\n"\
                                       "find . -name \"*.csv\" -type f -print -exec rm -rf {} \;") # Set command here
            # This function just pass through the command. It doesn't wait for it to end.
            print(idx)
            print(stdout.readlines())
            
        except:
            misconnection.append(idx) # Establish a list of failure connected IP addresses
    
    if misconnection!=[]: # If any one of the client is not working, reject the program and print the error info
        print("Misconnection:")
        for idx in misconnection:
            print("%i:" % idx + server[idx])
#        print("Misbehave:") # This part is not finished yet
#        for idx in misbehave:
#            print("%i:" % idx + server[idx])
    
    for idx, hostname in enumerate(server):
        rpi[idx].close()
    return
        
if __name__ == "__main__":  
    main()  