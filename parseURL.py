# Michael Hammer & Greg Shrock
import re
import sys

def parse(line):

  regEx = '(<a.*? href=(".+?https?://[\da-z\.-]+\.[a-z\.]{2,6}[/\w \.-]*"|\'.+?://.+?\').*?>)'
  p = re.compile(regEx)
  output = ""
  for group in p.findall(line):
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
    output = output +host+absUrl+"\n"
  sys.stdout.write(output)


while(True):
  line = ""
  l = ""
  while(l != 'terminate\n'):
    line = line + l
    l = sys.stdin.readline()
  parse(line)  
