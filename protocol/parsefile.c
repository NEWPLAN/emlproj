
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <string.h>
#include <sys/stat.h>

#include "parsefile.h"
#include "getline.h"

#define NONULL(x) ( x==NULL ? "" : x) /* this is nice, found in the mutt code */

struct paramlist * paramlist_init(void)
{
    struct paramlist *pl;
    if ((pl = malloc(sizeof(struct paramlist))) == NULL) return NULL;
    pl->name = NULL;
    pl->value = NULL;
    pl->next = NULL;
    return pl;
}

void paramlist_uninit(struct paramlist ** p)
{
    struct paramlist * next;
    while (*p)
    {
        next = (*p)->next;
        free((*p)->name);
        free((*p)->value);
        free(*p);
        *p = next;
    }
}

int paramlist_set(struct paramlist * p, const char * name, const char * value)
{
    /* TODO: deleting (value==NULL) does nothing */
    struct paramlist *last = NULL;
    if (!name) return 1;
    while (p && (p->name) && (strcasecmp(p->name, name)))
    {
        last = p;
        p = p->next;
    }
    if (!p)
    {
        if (!value) return 0;
        if ((p = paramlist_init()) == NULL) return 1;
        if (last) last->next = p;
    }
    free(p->name);
    free(p->value);
    p->name = strdup(name);
    p->value = value ? strdup(value) : NULL;
    return 0;
};

char * paramlist_get(struct paramlist * params, const char * name)
{
    if (!name) return NULL;
    while (params)
    {
        if (params->name && !strcasecmp(params->name, name)) return params->value;
        params = params->next;
    }
    return NULL;
}

int paramlist_strreplace(char ** dst, char * in, struct paramlist * params)
{
    typedef struct replacelist
    {
        int pos;    /* position in in where to replace      */
        int newlen; /* length of new text (params->value)   */
        int oldlen; /* length of old text (params->name)    */
        char * new; /* char to replace with */
        struct replacelist * next;
    } replacelist;

    int len, pos, l;
    int replacecount = 0;
    char * occ;
    char * in_inc;
    int lastreplacepos;
    int incremented;
    struct replacelist * rl_first = NULL, * rl = NULL, * rl_last, *rl_next;
    struct paramlist * p;
    if (!in) return -1;
    if (!params)  /* nothing to replace */
    {
        *dst = in;
        return 0;
    }
    *dst = NULL;
    len = strlen(in);
    p = params;
    /* look for replacements and save them, count size of new string */
    while (p)
    {
        if (p->name)
        {
            in_inc = in;
            incremented = 0;
            while ((occ = strstr(in_inc, p->name)) != NULL)
            {
                pos = occ - in;
                /* add entry to replacelist, which has to be sorted by pos,
                * we have to look up our entry point */
                rl = rl_first;
                rl_last = NULL;
                while (rl)
                {
                    if (rl->pos > pos) break;
                    rl_last = rl;
                    rl = rl->next;
                }
                rl_next = rl;
                if ((rl = malloc(sizeof(struct replacelist))) == NULL) return 0;
                if (rl_last) rl_last->next = rl;
                else rl_first = rl;
                rl->next = rl_next;
                rl->pos = pos;
                rl->newlen = strlen(NONULL(p->value));
                rl->new = p->value;
                rl->oldlen = strlen(p->name);
                len += rl->newlen - rl->oldlen;
                replacecount++;
                /* override old string to ensure no further shorter
                * string will match */
                memset(occ, 1, strlen(p->name));
                /* we loop strstr until we cannot find this p->name */
                in_inc += rl->oldlen;
                incremented += pos;
            }
        }
        p = p->next;
    }
    if (replacecount > 0)
    {
        if ((*dst = malloc(len + 1)) == NULL) return 0;
        (*dst)[len] = '\0';
        rl = rl_first;
        pos = 0;
        lastreplacepos = 0;
        while (rl)
        {
            /* copy unreplaced text into buf */
            l = rl->pos - lastreplacepos;
            memcpy(&(*dst)[pos], &in[lastreplacepos], l);
            lastreplacepos = rl->pos + rl->oldlen;
            pos += l;
            if (rl->newlen > 0)
            {
                /* copy replaced text */
                memcpy(&(*dst)[pos], rl->new, rl->newlen);
                pos += rl->newlen;
            }
            rl = rl->next;
        }
        /* copy the rest of the line */
        if ((l = len - pos) > 0) memcpy(&(*dst)[pos], &in[lastreplacepos], len - pos);
    }
    else *dst = in;
    rl = rl_first;
    while (rl)
    {
        rl_last = rl;
        rl = rl->next;
        free(rl_last);
    }
    return replacecount;
}

int parsefile(char * infile, char * outfile , struct paramlist * params, int leading)
{
    int in, out, ret;
    //if ( (in=open(infile, O_RDONLY))<0) return 1;
    if ( (in = open(infile, O_RDONLY)) < 0) return -1;
    /** if file already exists just send that file and don't do any parsing
      * prim vt */
    if ( (out = open(outfile, O_RDONLY)) >= 0)
    {
        close(in);
        close(out);
        return 0;
    }

    if ( (out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC,  S_IRUSR | S_IWUSR )) < 0) return 1;
    ret = parsefds(in, out, params, leading);
    close(in);
    close(out);
    return ret;
}

int parsefds(int in, int out , struct paramlist * params, int leading)
{
    int res, replacements;
    char * replaced;
    LineBuffer *l;
    if ((l = linebuf_init(4096)) == NULL) return 1;
    while ( (res = get_line(in, l)) >= 0)
    {
        if (l->linelen >= 0 )
        {
            replacements = paramlist_strreplace(&replaced, l->line, params);
            if (write_line(out, leading, replaced))
            {
                //error
            }
            if (replacements > 0) free(replaced);
        }
    }
    linebuf_uninit(l);
    if (res != GETLINE_EOF) return 1;
    return 0;
}
