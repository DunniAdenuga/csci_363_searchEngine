# Michael Hammer & Greg Shrock & Dunni
import re
import sys

htmlCap = "<!DOCTYPE html><html><head><title>Web Crawler</title><link rel='stylesheet' type='text/css' href='web/HTML_Styling.css'></head><body><div class='header'>CSCI 363 Search Engine by Michael Hammer, Greg Shrock, & Dunni Something</div><div class='title'>WEB CRAWLER</div><div class='submitRow'><form method='POST' action='/form'><input class='textBox' type='text' name='input'><input class='submitBtn' type='submit' name='Submit' value='submit'></form></div><div class='links'>"
htmlTail = "</div></body></html>"


links = ""
l = ""
while(l != 'terminate\n'):
  if(l!=""):
    l = l[:-1]
    links = links + '<a href="'+l+'">'+l+'</a><br>'
  l = sys.stdin.readline()

page = htmlCap+links+htmlTail
sys.stdout.write(str(len(page)) + "\n")
sys.stdout.write(page) 
