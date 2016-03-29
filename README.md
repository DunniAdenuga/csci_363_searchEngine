# Custom Search Engine

## Phase 1

### Description

In the first phase of the project, we are working to retrieve urls from 
single web page. In it's current state, we can run a client provided with a web
host and a path. The client will retrieve all of the urls found on that page
and print them to the console.

### Compilation

To test the functionality of the project, we must first compile the programs.
To do this, `cd` into the root folder of the project and call `make`. This
should compile the program without error.

### Execution

The compiled programs are placed into the `bin` directory. The program for
phase 1 is `pages`. To run a demonstration of the program on some example pages, 
execute one or more of the commands below. The commands consist of the program, 
the desired host, and the path to the file being requested from the host.

```
$ ./bin/pages www.eg.bucknell.edu /~xmeng/testpages/index.html
$ ./bin/pages www.eg.bucknell.edu /~xmeng/index.html
$ ./bin/pages www.bucknell.edu /
```

## Phase 2

### Description

In the second phase of the project, we created a web server (a simple search engine) that can answer user queries about a web page our program has visited and indexed. 

### Compilation

To test the functionality of the project, we must first compile the programs.
To do this, `cd` into the root folder of the project and call `make`. This
should compile the program without error.

Then, run the following commands:
```
$chmod 644 web/Shmoogle_noresults.html
$chmod 644 web/Shmoogle.html
$chmod 644 web/HTML_Styling.css
```
### Execution

The compiled programs are placed into the `bin` directory.
Run the command below where `PORT` is a valid port number 

```
$./bin/webserver PORT
```
Wait for few seconds(to allow webserver retrieve pages).
Then, open a web browser and enter the following:

```
localhost:PORT
```

## Phase 3

### Description
The main purpose of this phase was to handle crawling to different urls and generating a list of urls for searches. onality. 

### Compilation

To test the functionality of the project, we must first compile the programs.
To do this, `cd` into the root folder of the project and call `make`. This
should compile the program without error.

Then, run the following commands:
```
$chmod 644 web/Shmoogle_noresults.html
$chmod 644 web/Shmoogle.html
$chmod 644 web/HTML_Styling.css
```

### Execution
 Now, run the crawler with the following command:
```
./bin/crawl_program NUMPAGES
```
where `NUMPAGES` is an integer for the number of pages to crawl
- rerun the command to crawl more pages

Then, run the webserver with this command:
```
./bin/webserver PORT
```
where `PORT` is the port number (e.g 14025)

4. Open a web browser and enter:
```
localhost:PORT
```
5. Enter word or words you want to search for into textbox of webpage shown to you and result would be returned to you.