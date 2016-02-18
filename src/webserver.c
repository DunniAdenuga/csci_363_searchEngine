/*
 * Original version from Doug Comer's book "Internetworking with TCP/IP"
 * Revisions
 * 1. Added tcplib.c to simplify the socket creation.
 * 2. Removed the checking of Solaris and Windows, just concentrating
 *    on linux.
 * 
 * Xiannong Meng <xmeng@bucknell.edu>
 * 2015-12-11
 *
 * Note:
 *  1. This version of web server processes only the first pair of
 *     name/value in the "POST" message (e.g., forms), the rest paris 
 *     will be ignored. To handle more pairs, one need to re-write 
 *     part of the "process_post()" function.
 */

/* webserver.c */

#define _GNU_SOURCE           // we want to use the strcasestr() function

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#include "tcplib.h"
#include "http_response.h"    // HTTP response codes and strings

#define NONEXST -1            // result of file access attempt
#define NONREAD 0
#define READBLE 1


#define BUFFSIZE	256
#define FILESIZE        65536*16
#define SERVER_NAME	"localhost"


// transmissions
int	    recvln(int, char *, int);
void	  send_head(int, int, int, char*);

// file operations
int     read_file(char*, char*);
int     get_file_size(char*);

// HTTP processing
int     is_valid_http_req(char*, char*);
void    process_post(int, char*);
void    process_get(char*, int);
void    process_head(char*, int);


/*-----------------------------------------------------------------------
 *
 * Program: webserver
 * Purpose: serve hard-coded webpages to web clients
 * Usage:   webserver <appnum>
 *
 *-----------------------------------------------------------------------
 */
int main(int argc, char *argv[]) {
  int   conn;     // connection point
  int   sock;     // socket number
  int   port;     // port number
  int		n;        // size
  char		buff[BUFFSIZE], cmd[16], path[64], vers[16];
  char    header[8*BUFFSIZE]; // http header


  if (argc != 2) {
    (void) fprintf(stderr, "usage: %s <app_port_num>\n", argv[0]);
    (void) fprintf(stderr, "for example : %s 8688\n", argv[0]);
    exit(1);
  }

  port = atoi(argv[1]);
  sock = socketServer(port);

  while(1) {

    /* wait for contact from a client on specified appnum */
    conn = acceptConn(sock);
    if (conn < 0)
      exit(1);

    /* read and parse the request line */

    n = recvln(conn, buff, BUFFSIZE);
    buff[n] = 0;
    fprintf(stderr,"cmd from browser: %s\n", buff);
    sscanf(buff, "%s %s %s", cmd, path, vers);

    header[0] = 0; // initialize header, to be used in processing "post"

    /*
     * Read until we get \r\n alone, saving the header.
     * We read one line at a time so we can catch "\r\n"
     * by itself.
     */
    while((n = recvln(conn, buff, BUFFSIZE)) > 0) {
      buff[n] = 0;
      strncat(header, buff, n);
      fprintf(stderr,"cmd from browser: %s\n", buff);
      if (n == 2 && buff[0] == '\r' && buff[1] == '\n')
        break;
    }

    /* check for unexpected end of file */
    if (n < 1) {
      (void) send_eof(conn);
      continue;
    }

    /* check for a request that we cannot understand */
    if (is_valid_http_req(cmd, vers) == 0 ) {  // invalid request
      send_head(conn, 400, strlen(ERROR_400), "text/html");
      (void) send(conn, ERROR_400, strlen(ERROR_400), 0);
      (void) send_eof(conn);
      continue;
    }

    /* process posted form */
    if (strcasecmp(cmd, "POST") == 0) {
      process_post(conn, header);
    } else if (strcasecmp(cmd, "GET") == 0) {
      /* process GET  */
      process_get(path, conn);
    } else if (strcasecmp(cmd, "HEAD") == 0) {
      /* process HEAD  */
      process_head(path, conn);
    }
  } /* end of while (1) */
}

/*-------------------------------------------------------
 * Determine if an HTTP request from the client is valid.
 *
 * parameters:
 *   cmd:  HTTP command such as GET or POSt
 *   vers: version of http, 1.0 or 1.1
 *
 * return:
 *   0 if the request is invliad, 1 otherwise
 *-------------------------------------------------------
 */
int 
is_valid_http_req(char * cmd, char * vers) {

  if ((strcasecmp(cmd, "GET") && 
        strcasecmp(cmd, "POST") &&
        strcasecmp(cmd, "HEAD"))
      || (strcasecmp(vers, "HTTP/1.0") &&
        strcasecmp(vers, "HTTP/1.1"))) {
    return 0;   // invalid
  } else {
    return 1;   // valid
  }
}

/*-------------------------------------------------------
 * Extract "content-length" from the header
 *
 * parameters:
 *   header: HTTP header which should include "content-length"
 *
 * return:
 *   the integer form of the "content-length"
 *-------------------------------------------------------
 */
int get_content_length(char * header)  {

  char *lengthPtr;

  // search where the length value is
  lengthPtr = strcasestr(header, "content-length");

  if (lengthPtr == NULL)
    return 0;

  /* now extract the integer value */

  // first skip all non-digit chars
  while (!isdigit(*lengthPtr))
    lengthPtr ++;

  // keep the begining of the digit
  char *digitsPtr = lengthPtr;  

  // now skip until the end of the digits
  while (isdigit(*lengthPtr))
    lengthPtr ++;

  char digits[32];   // assume the integer is less than 32 digits

  // copy the number, (len + 1) to terminate the string
  strncpy(digits, digitsPtr, (lengthPtr - digitsPtr) + 1);
  // terminate the string
  digits[(lengthPtr - digitsPtr)];

  return atoi(digits);
}

/*---------------------------------------------------
 * This is a simplified vesion of the function. It
 * assumes that there is only one pair of name/value!
 * The function processes the post information, returning
 * proper response in HTML form.
 *
 * parameters:
 *   conn:   socket connection
 *   header: HTTP header from the client (e.g., browser)
 *---------------------------------------------------
 */
void process_post(int conn, char * header) {

  int  n;
  char buff[4*BUFFSIZE];    // assume the form is no longer than this
  char name[BUFFSIZE], value[BUFFSIZE];
  // for the name/value pair
  char retContent[4*BUFFSIZE];
  // returning html page

  // extract the integer representing the content length
  int contentLength = get_content_length(header);

  if (contentLength == 0)  {
    send_head(conn, 404, strlen(ERROR_404), "text/html");
    (void) send(conn, ERROR_404, strlen(ERROR_404),0);

    return;
  }

  // read the form from browser
  n = recv(conn, buff, contentLength, 0);
  // buff should be in the form of "n1=v1&n2=v2&n3=v3"
  buff[n] = 0;

  // extract the first name/value pair, look for '='
  char * assign = strstr(buff, "=");   
  n = strlen(buff) - strlen(assign);
  strncpy(name, buff, n);
  name[n] = 0;
  // then look for '&'
  char * amp = strstr(buff, "&");
  n = strlen(assign) - strlen(amp);
  strncpy(value, (assign+1), n-1);
  value[n] = 0;

  // build HTML code for display
  strcpy(retContent, "<dl compact>\n");
  strcat(retContent, "<dt><b>");
  strcat(retContent, name);
  strcat(retContent, "</b> <dd>:<i> ");
  strcat(retContent, value);
  strcat(retContent, "</i>:<br>\n");
  strcat(retContent, "</dl>\n");

  // return the header first
  n = strlen(retContent);
  send_head(conn, 200, n, "text/html");

  // return the contents
  (void) send(conn, retContent, n, 0);
}

char *get_file_ext(char *filename) {
  char *dot = filename;
  while(*dot != '\0' && *dot != '.') {
    dot += 1;
  }
  return dot + 1;
}

char *get_response_type(char *filename) {
  char *fext = get_file_ext(filename);
  if(strcmp(fext, "html") == 0){
    return "text/html";
  }else if(strcmp(fext, "jpeg") == 0 || strcmp(fext, "jpg") == 0){
    return "image/jpeg";
  }else if(strcmp(fext, "png") == 0) {
    return "image/png";
  }else{
    return "";
  }
}

/*---------------------------------------------------
 * Process the HTTP "HEAD" command, returns meta-
 * information about the resource requested
 *
 * parameters:
 *   path: file path for the requested document
 *   conn: socket for the connection
 * 
 *---------------------------------------------------
 */
void
process_head(char * path, int conn) {

  int           n;                // read-write size
  int           fstatus;          // file status
  char          * fname = path;   // the file name
  char          * fext;           // the file extension
  char          * response_type;  // the type of the response
  int           ftype;            // file type
  int           fsize;            // file size

  // used for time page
  char          buff[BUFFSIZE];
  char          *timestr;
  struct        timeval tv;


  // test for special files
  if( strcmp(fname, "/") == 0){                     // default page
    send_head(conn, 200, strlen(HOME_PAGE), "text/html");
    (void) send_eof(conn); 
    return;
  } else if (strcmp(fname, "/moved.html") == 0) {   // moved page
    send_head(conn, 301, strlen(ERROR_301), "text/html");
    (void) send_eof(conn); 
    return;
  } else if (strcmp(fname, "/time") == 0) {         // time page
    gettimeofday(&tv, NULL);
    timestr = ctime(&tv.tv_sec);
    (void) sprintf(buff, TIME_PAGE, timestr);
    send_head(conn, 200, strlen(buff), "text/html");
    (void) send_eof(conn); 
    return;
  }

  fname++;    // remove leading slash
  fstatus = check_file_status(fname);

  if (fstatus == READBLE) {      // Good file
    response_type = get_response_type(fname);
    fsize = get_file_size(fname);
    send_head(conn, 200, fsize, response_type);
  } else if (fstatus == NONEXST) {    // Does not exist
    send_head(conn, 404, strlen(ERROR_404), "text/html");
  } else if (fstatus == NONREAD) {    // Permission denied
    send_head(conn, 403, strlen(ERROR_403), "text/html");
  } else {                            // Internal server error
    send_head(conn, 500, strlen(ERROR_500), "text/html");
  }
  (void) send_eof(conn);
}

/*---------------------------------------------------
 * Process the HTTP "GET" command, returns some hard-
 * coded HTML files.
 *
 * parameters:
 *   path: file path for the requested document
 *   conn: socket for the connection
 * 
 *---------------------------------------------------
 */
void
process_get(char * path, int conn) {

  char          data[FILESIZE]; /* file content, such as JLH.jpg photo */
  char          buff[BUFFSIZE];
  int           n;             /* read/write size */
  char		*timestr;
  struct timeval	tv;
  int           fstatus;  // file status

  /* send the requested web page or any error message */
  if (strcmp(path, "/") == 0) {
    send_head(conn, 200, strlen(HOME_PAGE), "text/html");
    (void) send(conn, HOME_PAGE, strlen(HOME_PAGE), 0);
  } else if (strcmp(path, "/default.html") == 0) {
    n = read_file("default.html", data);
    send_head(conn, 200, n, "text/html");
    (void) send(conn, data, n, 0);
  } else if (strcmp(path, "/time") == 0) {
    gettimeofday(&tv, NULL);
    timestr = ctime(&tv.tv_sec);
    (void) sprintf(buff, TIME_PAGE, timestr);
    send_head(conn, 200, strlen(buff), "text/html");
    (void) send(conn, buff, strlen(buff), 0);
  } else if (strcmp(path, "/moved.html") == 0) {
    send_head(conn, 301, strlen(ERROR_301), "text/html");
    (void) send(conn, ERROR_301, strlen(ERROR_301), 0);
  } else if (strcmp(path, "/JLH.jpg") == 0 || 
      strcmp(path, "/form.html") == 0) {
    /* request one of the two files */
    char * fname = path;
    fname++; // moveover the leading '/'
    fstatus = check_file_status(fname);
    if (fstatus == READBLE) {        // ok
      n = read_file(fname, data);
      if (strcmp(path, "/JLH.jpg") == 0)  // the image file
        send_head(conn, 200, n, "image/jpeg");
      else                                // the form.html, html file
        send_head(conn, 200, n, "text/html");
      (void) send(conn, data, n, 0);
    } else if (fstatus == NONEXST) {   // not found
      send_head(conn, 404, strlen(ERROR_404), "text/html");
      (void) send(conn, ERROR_404, strlen(ERROR_404), 0);
    } else if (fstatus == NONREAD) { // forbidden
      send_head(conn, 403, strlen(ERROR_403), "text/html");
      (void) send(conn, ERROR_403, strlen(ERROR_403), 0);
    } else {
      send_head(conn, 500, strlen(ERROR_500), "text/html");
      (void) send(conn, ERROR_500, strlen(ERROR_500), 0);
    }
  } else { /* not found */
    send_head(conn, 404, strlen(ERROR_404), "text/html");
    (void) send(conn, ERROR_404, strlen(ERROR_404), 0);
  }
  (void) send_eof(conn);
}

/*-----------------------------------------------------------------------
 * send_head - send an HTTP 1.0 header with given status and content-len
 *
 * parameters:
 *    conn: socket connection
 *    stat: HTTP status code
 *    len:  content length
 *    type: content type
 *-----------------------------------------------------------------------
 */
  void
send_head(int conn, int stat, int len, char * type)
{
  char	*statstr, buff[BUFFSIZE];
  char	*timestr;
  struct timeval  tv;

  /* convert the status code to a string */

  switch(stat) {
    case 200:
      statstr = "OK";
      break;
    case 301:
      statstr = "Moved";
      break;
    case 400:
      statstr = "Bad Request";
      break;
    case 403:
      statstr = "No Permission";
      break;
    case 404:
      statstr = "Not Found";
      break;
    case 500:
      statstr = "Internal Error";
      break;
    default:
      statstr = "Unknown";
      break;
  }

  /*
   * send an HTTP/1.0 response with Server, Content-Length,
   * and Content-Type headers.
   */

  (void) sprintf(buff, "HTTP/1.0 %d %s\r\n", stat, statstr);
  (void) send(conn, buff, strlen(buff), 0);

  /** try out cookie! **/
  gettimeofday(&tv, NULL); // retrieve current time
  tv.tv_sec += 120;        // so the cookie expires in time
  timestr = ctime(&tv.tv_sec);
  (void) sprintf(buff, "Set-Cookie: new=value; version=1; ");
  //	(void) sprintf(buff, "Set-Cookie: name3=value3;  ");
  (void) send(conn, buff, strlen(buff), 0);
  //  (void) sprintf(buff, "expires=Wed, 01-December-2004 16:00:00 GMT; ");
  (void) sprintf(buff, "expires=%s; ", timestr);
  (void) send(conn, buff, strlen(buff), 0);
  (void) sprintf(buff, "path=/time; domain=.eg.bucknell.edu\n");
  (void) send(conn, buff, strlen(buff), 0);

  /**
    (void) sprintf(buff, "Server: %s", SERVER_NAME);
    (void) send(conn, buff, strlen(buff), 0);
   **/
  (void) sprintf(buff, "Content-Length: %d\n", len);
  (void) send(conn, buff, strlen(buff), 0);

  (void) sprintf(buff, "Content-Type: %s\n", type);
  (void) send(conn, buff, strlen(buff), 0);

  (void) sprintf(buff, "\n");
  (void) send(conn, buff, strlen(buff), 0);
}

/*-----------------------------------------------------------------------
 * read_file: read content of given file name, pass back the buff
 *-----------------------------------------------------------------------
 */
  int
read_file(char * fname, char buff[FILESIZE])
{
  FILE * f = fopen(fname, "r");

  int size_read = fread(buff, 1, FILESIZE-1, f);
  if (size_read > 0) {
    buff[size_read] = 0;
  }
  fclose(f);

  return size_read;
}

/*-----------------------------------------------------------------------
 * Check the file status to see if the client has the permission to access
 * 
 * parameters:
 *   fname: the name of the file to access
 *
 *   return:
 *      -2: unknown
 *      -1: doesn't exist
 *       0: readable
 *       1: not readable
 *-----------------------------------------------------------------------
 */
  int 
check_file_status(char * fname) 
{
  // container for complete set of file permission bits (binary)
  unsigned int mode;
  // container for the three bits of other permission
  unsigned int omode;
  // structure to contain the result of the fstat call (info on this file)
  struct stat file_info;
  // return status
  int file_status = -2; // status unknown

  // try to access file
  file_status = stat(fname, &file_info);
  if (file_status != 0) { // stat() call failed
    file_status = -1;
  } else {
    // mode comes from the lower 9 bits in file_info.st_mode
    mode = file_info.st_mode & 0x1FF;
    // omode comes from the low 3 bits in mode
    omode = mode & S_IRWXO;   // 0o00007
    if (omode < 4) {    // 4 == 100 in rwx
      file_status = 0;  // unreadable
    } else {
      file_status = 1;  // readable
    }
  }  // end of file exists

  return file_status;
}

/*
 *---------------------------------------------------
 * Return the size of a given file.
 *---------------------------------------------------
 */
int get_file_size(char * fname) {

  // default file size
  int file_size   = 0;
  int file_status = 0; 
  // structure to contain the result of the fstat call (info on this file)
  struct stat file_info;

  // try to access file
  file_status = stat(fname, &file_info);
  if (file_status != 0) { // stat() call failed
    file_size = 0;
  } else {                // extract file size
    file_size = file_info.st_size;
  }

  return file_size;
}
