# Michael Hammer & Greg Shrock
import re
import sys

def parse(line, main_host):
  '''Finds urls within webpage and returns them'''
  regEx = '(href *= *[\'"]((https?://)?[0-9a-zA-Z-/\.$#:\?]+)[\'"])'
  p = re.compile(regEx)
  output = ""
  for t in p.findall(line):
    url = t[1]
    if is_abs(url): #tells if path is relative or not
      (host, path) = handle_abs(url)
      output = add_url_output(output, host, path)       # creates formatted url
    else: #if path is relative
      path = handle_rel(url)
      output = add_url_output(output, main_host, path)  # creates formatted url
  return output


def handle_rel(url):
  '''handles realtive urls by formatting and 
   converting to absolute urls'''
  relUrl = url.split("/")
  urlLen = len(relUrl)
  absUrl = ""
  skip = 0 # handles if a .. was found
  # traverses url backwards to remove unnecessary path data
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
  '''handles formatting absolute urls'''
  (proto, page) = url.split("//")
  elements = page.split("/")
  host = elements[0]
  path = "/" + "/".join(elements[1:])
  return (host, path)

def is_abs(path):
  '''true if starts with http - it is absolute'''
  return path[0:4] == "http"

def add_url_output(output, host, path):
  '''formats url with host and path'''
  output = output + host + "\n" + path + "\n"
  return output


main_host = sys.stdin.readline()[0:-1] # reads host name / removes new line
line = ""    # stores the complete text
l = ""       # temporary variable to store each line
# reads in all strings
while(l != 'terminate\n'):      # loop until terminate key is found
  line = line + l
  l = sys.stdin.readline()

urls = parse(line, main_host)   # parses the urls and formats them
sys.stdout.write(urls)          # writes urls to stdout
