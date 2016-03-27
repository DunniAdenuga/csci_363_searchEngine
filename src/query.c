struct query_interface{
  struct inv_list *il;
}

struct query_interface *qi_create(struct crawler *c){
  struct query_interface *q = calloc(1, sizeof(struct query_interface));
  q->il = get_inv_list(c);
}

void qi_destroy();


struct site_list *qi_query_expression(struct query_interface *q);
struct inv_list *il = cl_get_inv_list(q->crawler);

struct site_list *qi_query_word(struct query_interface *q, char *word){
  struct site_list *s = il_get_sites(c->word_results, word);
  if(s == NULL)
    return sl_create();
  return s;
}
