#ifndef __QUERY_INTERFACE_H_
#define __QUERY_INTERFACE_H_

#include "inv_list.h"
#include "site_list.h"

/**
 * This set of files provides a means of querying an inverted list. It allows a
 * query as contained in an http post or a query of a single word. It also
 * gives a means of managing memory by providing a destroy function which will
 * free memory appropriatly for site lists returned by the queries.
 */

struct query_interface{ //query API
  struct inv_list *il;
};

// creates a new query_interface for the specified inverted list
struct query_interface *qi_create(struct inv_list *il);
// queries the system using the given expression
struct site_list *qi_query_expression(struct query_interface *q, char *expression);
// queries the system using the given single word
struct site_list *qi_query_word(struct query_interface *q, char *word);
// properly frees the memory allocated for the query results
void qi_destroy_query_results(struct site_list *s);

#endif
