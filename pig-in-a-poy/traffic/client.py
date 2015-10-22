import telnetlib
import re


tn = telnetlib.Telnet("193.218.139.39", 31337)
tn.read_some()
tn.write('John Smith')

task = tn.read_some()
parts = re.findall('(\d+) \+ (\d+)', task)[0]
result = int(parts[0]) + int(parts[1])
tn.write(str(result))

tn.read_some()
tn.write('USSR IS SO GULAG')

print tn.read_some()
