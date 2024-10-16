#!/usr/bin/env python3
from bcc import BPF
import time
import sys

if len(sys.argv) != 4:
    print("Bad input: usage: ./filter.py interface cfile func")
    exit(0)

if_name = sys.argv[1]
file_name = sys.argv[2]
func_name = sys.argv[3]

device = if_name
b = BPF(src_file=file_name)
fn = b.load_func(func_name, BPF.XDP)
b.attach_xdp(device, fn, 0)
 
try:
  b.trace_print()
except KeyboardInterrupt:
  pass
 
b.remove_xdp(device, 0)
