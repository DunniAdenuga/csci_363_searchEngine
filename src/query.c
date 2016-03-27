#define MAXWORDS 20

struct query_interface{ //query API
  struct inv_list *il;
};

struct query_interface *qi_create(struct inv_list *il){
  struct query_interface *q = calloc(1, sizeof(struct inv_list));
  q->il = il;
}

void qi_destroyi(struct query_interface *q) {
	il_destroy(q->il);
}

struct site_list *qi_query_expression(struct query_interface *q, char *exp) {
	/* creates a list list per word */
	struct site_list *site_lists[MAXWORDS];
	char *toks[MAXWORDS];
	int i = 0; 
	toks[0] = strtok(exp," ");
	while(toks[i] != NULL && i < MAXWORDS) { 
		site_lists[i] = qi_query_word(q, toks[i]);
		toks = strtok(NULL," ");
		i++;
	}
	i--; //sets i to last element in list

	/* combines site_lists */
	struct site_list *master_list;
	while (i >= 0) { 
		struct tmp_list = site_lists[i];
		/* iterate through temporary site list and add to master */
		struct site_node *tmp_node = sl_iter_begin(site_list[i]);
		char *host = tmp_node->host;
		char *path = tmp_node->path;
		while (tmp_node != NULL) {
			sl_add(master_list,host,path,1);
			tmp_node = sl_iter_next(site_list[i]);
		}
		sl_destroy(site_lists[i]);
		i--;
	}
	return master_list;
}

struct site_list *qi_query_word(struct query_interface *q, char *word){
  struct site_list *s = il_get_sites(q->il, word);
  if(s == NULL)
    return sl_create();
  return s;
}
