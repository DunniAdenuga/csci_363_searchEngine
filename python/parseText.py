# Michael Hammer & Greg Shrock
import re
import sys

dict = {} #dictionary to hold word frequency

def updateDict(key):
	'''Given a key, this funciton either adds the new key to 
	the dictionary or it updates the existing key'''
	if key in dict.keys():
		dict[key] = dict[key] + 1
	else:
		dict[key] = 1


'''Main section of code that reads in lines and adds the
words to a dicitonary'''
l = sys.stdin.readline() 
while(l != "terminate\n"):
	regEx = "[A-Za-z\-]+"
	p = re.compile(regEx)
	for t in p.findall(l):
		updateDict(t.lower())
#		print("word: "+t) 
		sys.stdout.write(t.lower()) 
	l = sys.stdin.readline()
#for elem in dict.keys():
#	print("The element: "+elem)
#	print("      value: "+str(dict[elem]))
