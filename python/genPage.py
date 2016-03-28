''' The purpose of this code is to read in urls and add insert them into the HTML of our results webpage '''
# Michael Hammer & Greg Shrock & Dunni
import re
import sys



# HTML code that represents the output page of a search
htmlCap = "<!DOCTYPE html><html><head><title>Web Searcher</title><link rel='stylesheet' type='text/css' href='web/HTML_Styling.css'></head><body><div class='header'>CSCI 363 Search Engine by Michael Hammer, Greg Shrock, & Dunni Adenuga</div><div class='title'>WEB SEARCHER</div><div class='submitRow'><form method='POST' action='/form'><input class='textBox' type='text' name='input'><input class='submitBtn' type='submit' name='Search' value='Search'></form></div><div class='links'>"
htmlTail = "</div></body></html>"


links = ""
l = ""
while(l != 'terminate\n'): # reads until terminatino key is found
  if(l!="" and l!="\n"):
    l = l[:-1]
    links = links + '<div class="linkDiv"><a href="'+l+'">'+l+'</a></div>' # appends url in HTML format
  l = sys.stdin.readline()

if (links == ""):
    links = "No Results"
page = htmlCap+links+htmlTail           # creates HTML page string
sys.stdout.write(str(len(page)) + "\n") # writes length of page
sys.stdout.write(page)                  # writes the page
