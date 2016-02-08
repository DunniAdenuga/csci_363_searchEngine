# Michael Hammer & Greg Shrock
import re
import sys

while(True):
  #Read line in and find any URLs it contains
  line = sys.stdin.readline()
  regEx = '(<a.+?href=(".+?//.+?"|\'.+?//.+?\').*?>)'
  p = re.compile(regEx)
  for group in p.findall(line):
    #Convert relative URL to absolute URL
    #print("REL URL = " + group[0]);
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

    f = open("something.txt", 'a')
    print(host+absUrl, file=f)
    f.close()

    sys.stdout.write(host+absUrl+"\n")
