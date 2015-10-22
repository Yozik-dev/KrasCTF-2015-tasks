import telnetlib
import time


tn = telnetlib.Telnet("localhost", 31337)
time.sleep(1)
print tn.read_some()
print tn.read_some()
time.sleep(1)

tn.write('GET_SESSION')
print tn.read_some()
time.sleep(1)

tn.write('GET_SESSION') 
print tn.read_some()
time.sleep(1)

tn.write('GET_SESSION')
print tn.read_some()
time.sleep(1)
