#!/usr/bin/env python2.7

import sys
import pexpect

if  len(sys.argv) != 2:
    raise TypeError("Wrong input, Useage: ./upload.sh load_name server_name")

print("Download load from server......\n")

cmd = "scp ts@10.89.255.148:/tftpboot/"+sys.argv[1]+" ."
child = pexpect.spawn(cmd, timeout=120)
child.expect("password: ")
child.sendline("ts")
child.read()
'''
if sys.argv[2] == "1":
        print("Upload to sercomm......")
elif sys.argv[2] == "2":
        print("Upload to unihan......")
else:
        raise TypeError("Bad server name")
'''
