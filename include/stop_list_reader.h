#ifndef __STOP_LIST_READER_H_
#define __STOP_LIST_READER_H_

/**
 * These functions provide a convenient interface to read the content of a file
 * containing the stop list to be used by a crawler. It allows the retrieval
 * of the total count of words as well as a means of loading those words. Lastly,
 * it provides a memory management function of when the data is no longer needed.
 *
 * file format:
 * <word>\n
 *
 * example:
 * the
 * will
 *
 */

// returns the number of words found in the file (file must be formated correctly)
int get_stop_word_count(char *file_name);

// returns the words found in the file
char **get_stop_words(char *file_name);

// frees memory allocated by "get_stop_words"
void free_stop_words(char **paths, char *file_name);

#endif
