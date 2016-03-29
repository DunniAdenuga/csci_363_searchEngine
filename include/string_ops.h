#ifndef __SMART_STRING_H_
#define __SMART_STRING_H_

/**
 * Provides a simple save and load interface for strings.
 */

// save a string to the file descriptor
void str_save(char *, int fd);
// load a string from the file descriptor
char *str_load(int fd);

#endif
