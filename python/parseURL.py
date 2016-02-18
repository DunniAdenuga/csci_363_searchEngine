# Michael Hammer & Greg Shrock
import re
import sys

def parse(line, main_host):
  #line = sys.stdin.read()
  regEx = '(href *= *[\'"]((https?://)?[0-9a-zA-Z-/\.$#:\?]+)[\'"])'
  p = re.compile(regEx)
  output = ""
  for t in p.findall(line):
    url = t[1]

    if is_abs(url): #tells if path is relative or not
      (host, path) = handle_abs(url)
      output = add_url_output(output, host, path)

    else: #if path is relative
      path = handle_rel(url)
      output = add_url_output(output, main_host, path)

  return output


def handle_rel(url):
  relUrl = url.split("/")
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
        absUrl = "/" + relUrl[urlLen-i] + absUrl
  return absUrl


def handle_abs(url):
  (proto, page) = url.split("//")
  elements = page.split("/")
  host = elements[0]
  path = "/" + "/".join(elements[1:])
  return (host, path)

def is_abs(path):
  return path[0:4] == "http"

def add_url_output(output, host, path):
  output = output + host + "\n" + path + "\n"
  return output

main_host = sys.stdin.readline()[0:-1]
#host = "www.MICHAEL.txt"
line = ""
l = ""
while(l != 'terminate\n'):
  line = line + l
  l = sys.stdin.readline()

urls = parse(line, main_host)  
sys.stdout.write(urls)
