#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define GBUF 4

/* "Elaborate macro" section now follows */
#define CHKARGS(a, x); \
    if(a!=(x)+1) { \
        printf("Error. Pls supply %i %s\n", (x), ((x) == 1)? "argument." : "arguments."); \
        exit(EXIT_FAILURE); \
    }

/* Reminder: a is the array whose size is being incremented, and t is the type, a string  */
#define CONDREALLOC(x, b, c, a, t); \
    if((x)==((b)-1)) { \
        (b) += (c); \
        (a)=realloc((a), (b)*sizeof(t)); \
    }

typedef struct
{
    size_t lcc, soc; /* Line Character Count and Sequential OCcurences */
} lcc_t;

int main(int argc, char *argv[])
{
    CHKARGS(argc, 1);

    FILE *fp=fopen(argv[1], "r");

    size_t cc=0/*charcount on the line*/, ccold=0, lc=0 /*linecount on the file*/, ic=0, glbuf = GBUF;
    lcc_t *lcca=calloc(glbuf, sizeof(lcc_t)); /* Line Char Count array */

    int i, c;

    while((c=fgetc(fp)) != EOF) {
        if (c == '\n') {
            if(ccold!=cc) {
                CONDREALLOC(ic, glbuf, GBUF, lcca, lcc_t); /*  this turned out to be a fairly important one */
                lcca[ic].lcc=cc;
                lcca[ic++].soc=1;
            } else
                lcca[ic-1].soc++; /* the counts refer to the previous incidence number */
            lc++;
            ccold=cc;
            cc=0;
        } else
            cc++;
    }
    fclose(fp);
    lcca=realloc(lcca, ic*sizeof(lcc_t));

    printf("Total lines in file: %zu. Line-char count: sequential count pairs:\n", lc);
    for(i=0;i<ic;++i) 
        printf("%3zu ", lcca[i].lcc);
    printf("\n"); 
    for(i=0;i<ic;++i) 
        printf("%3zu ", lcca[i].soc);
    printf("\n"); 

    free(lcca);

    return 0;
}
