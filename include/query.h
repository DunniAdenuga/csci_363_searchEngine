#ifndef __QUERY_INTERFACE_H_
#define __QUERY_INTERFACE_H_

#include "inv_list.h"
#include "site_list.h"

struct query_interface{ //query API
  struct inv_list *il;
};

struct query_interface *qi_create(struct inv_list *il);
struct site_list *qi_query_expression(struct query_interface *q, char *exp);
struct site_list *qi_query_word(struct query_interface *q, char *word);
void qi_destroy_query_results(struct site_list *s);

#endif
