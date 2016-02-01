# Michael Hammer & Greg Shrock
import re
import sys

while(True):
	line = input("")
	regEx = '(<.+?href=(".+?"|\'.+?\').*?>)'
	p = re.compile(regEx)
	for t in p.findall(line):
		sys.stdout.write(t[0])

