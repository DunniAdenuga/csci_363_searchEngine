# Michael Hammer & Greg Shrock
import re
import sys

while(True):
	#Read line in and find any URLs it contains
  print("Reading a line...")
  #line = input("")
  line = sys.stdin.readline()
  print(line)

  regEx = '(<.+?href=(".+?"|\'.+?\').*?>)'
  p = re.compile(regEx)
  for group in p.findall(line):
    #Convert relative URL to absolute URL
    relUrl = group[1].split("/")
    host = relUrl[0]+"//"+relUrl[2]+"/"
    relUrl = relUrl[3:]
    urlLen = len(relUrl)
    absUrl = ""
    skip = 0
    for i in range(1,urlLen+1):
      if (relUrl[urlLen-i] == ".."):
        skip = skip + 1
      elif (skip > 0):
        skip = skip - 1
      else:
        if (relUrl[urlLen-i] == "."):
          pass
        else:
          absUrl = relUrl[urlLen-i] + "/" + absUrl
    sys.stdout.write(host+absUrl)



