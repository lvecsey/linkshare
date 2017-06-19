#ifndef VOTE_H
#define VOTE_H

#include "critbit.h"

int vote_increment_core(char *hexstr);
int vote_decrement_core(char *hexstr);

int vote_increment(char *json_str);
int vote_decrement(char *json_str);

int upvote(critbit0_tree *ls_entries, char *title, char *url);
int downvote(critbit0_tree *ls_entries, char *title, char *url);

#endif
