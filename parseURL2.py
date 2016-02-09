# Michael Hammer & Greg Shrock
import re
import sys

def parse(line,host):
  #line = sys.stdin.read()
  regEx = '(href *= *([\'"](https?://)?[0-9a-zA-Z-/\.$#:\?]+[\'"]))'
  p = re.compile(regEx)
  output = ""
  for t in p.findall(line):
    u = t[1]
    if (u[1:5] == "http"): #tells if path is relative or not
      output = output + u + '\n'
    else: #if path is relative
      relUrl = u[1:len(u)].split("/")
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
      output = output+host+absUrl+"\n"
  sys.stdout.write(output)


host = sys.stdin.readline().split("/")[0]
#host = "www.MICHAEL.txt"
line = ""
l = ""
while(l != 'terminate\n'):
  line = line + l
  l = sys.stdin.readline()
#sys.stdout.write("This would be the urls"+host)
parse(line,host)  

