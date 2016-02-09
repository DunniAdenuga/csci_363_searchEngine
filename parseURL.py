# Michael Hammer & Greg Shrock
import re
import sys

def parse(line):
  #regEx = '(<a.+?href=(".+?//.+?"|\'.+?//.+?\').*?>)'
  regEx ='<a\\s*href\\s*=\\s*\"[a-z:/.0-9?#]*\"'
  p = re.compile(regEx)
  output = ""
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
    output = output +host+absUrl+"\n"
  sys.stdout.write(output)


#while(True):
  #Read line in and find any URLs it contains
line = ""
l = ""
while(l != 'terminate\n'):
  line = line + l
    #  l = sys.stdin.readline()
  l = sys.stdin.readline()
parse(line)  
#sys.stdout.write(l)
#sys.stdout.write("%d" , len(line));
#sys.stdout.write(line)

