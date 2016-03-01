# Michael Hammer & Greg Shrock
import re
import sys

l = ""

while(l != "terminate\n"):
	regEx = "[A-Za-z]*"
	p = re.compile(regEx)
	for t in p.findall(l):
		sys.stdout.write(t)
	l = sys.stdin.readline();

