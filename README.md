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
execute one or more of the commands below.

```
$ ./bin/pages www.eg.bucknell.edu /~xmeng/testpages/index.html
$ ./bin/pages www.eg.bucknell.edu /~xmeng/index.html
$ ./bin/pages www.bucknell.edu /
```