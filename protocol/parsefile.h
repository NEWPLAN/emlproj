#ifndef _PARSEFILE_H
#define _PARSEFILE_H


typedef struct paramlist
{
    char * name;
    char * value;
    struct paramlist * next;
} paramlist;

/* parses infile to outfile, all words given in params will be replaced
 * leading (\r\n or \n) is one of WRITELINE_LEADING_[NONE|N|RN]
 */
int parsefile(char * infile, char * outfile, paramlist * params, int leading);

/* parses file descriptor in to out, all words given in params will be replaced
 * leading (\r\n or \n) is one of WRITELINE_LEADING_[NONE|N|RN]
 */
int parsefds(int in, int out , paramlist * params, int leading);

/* Adds/Updates name to paramlist.
 * To delete a name, call it with value set to NULL.
 */
int paramlist_set(struct paramlist * params, const char * name, const char * value);

/* returnes the value of name */
char * paramlist_get(struct paramlist * params, const char * name);

/* initialize paramlist */
struct paramlist * paramlist_init(void);

/* unitializes the paramlist */
void paramlist_uninit(struct paramlist ** params);

#endif /* ifndef _PARSEFILE_H */
