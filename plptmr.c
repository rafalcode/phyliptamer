#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define GBUF 4
#define MXICN 60 /* max interleaved column number */
#define NMBUF 10

#define CONDREALLOC(x, b, c, a, t); \
    if((x)==((b)-1)) { \
        (b) += (c); \
        (a)=realloc((a), (b)*sizeof(t)); \
    }

int main(int argc, char *argv[])
{
    if(argc!=3) {
        printf("Error. Pls supply 2 argument: 1) option \"-a\" for all column printout\n");
        printf("                                 option \"-fa\" for fasta all column printout\n");
        printf("                                 option \"-1\" for one gap column (first) AND all non-gapped printout\n");
        printf("                                 option \"-g\" for only gap columns\n");
        printf("                                 option \"-1g\" for only first gap column\n");
        printf("And finally the second argument should be the PHYLIP alignment file.\n");
        exit(EXIT_FAILURE);
    }

    int i, j, c;
    FILE *fp=fopen(argv[2], "r");
    unsigned txa, cols, conli=0 /* char on line index */, li=0 /* line number index */;
    if( (fscanf(fp, "%u%u\n", &txa, &cols)) != 2) {
        printf("An alignment file in PHYLIP style is expected as input: first line must contain correct taxa and column quantity of alignment\n"); 
        exit(EXIT_FAILURE);
    }
    printf("First line of phylip file says #taxa=%u and #cols=%u\n", txa, cols);
    char **nms=malloc(txa*sizeof(char*));
    for(i=0;i<txa;++i) 
        nms[i]=calloc(NMBUF, sizeof(char));
    char **alm=malloc(txa*sizeof(char*));
    for(i=0;i<txa;++i) 
        alm[i]=calloc(cols, sizeof(char));
    char cold='\0';
    char seen1spa=0;
    unsigned *gapa=calloc(cols, sizeof(unsigned));

#ifdef DBG
    printf("l_%u:", li);
#endif
    unsigned sli=0;/* sequence on line index */
    while((c=fgetc(fp)) != EOF) {
        if(c == ' ') {
            if(!seen1spa) {
                seen1spa=1;
                sli=0;
            }
        } else if(c != '\n') {
            if(seen1spa) {
#ifdef DBG
                printf("%i ", sli);
#endif
                alm[li%txa][MXICN*(li/txa)+sli]=c;
                if(c=='-')
                    gapa[MXICN*(li/txa)+sli]++;
            } else if(li<txa) {
                nms[li][conli]=c;
            }
            conli++;
            sli++;
        } else if ( (c == '\n') && (cold != '\n') ) {
            conli=0;
            li++;
#ifdef DBG
            printf("\nl_%u:", li%txa);
#endif
            seen1spa=0;
        }
        cold=c;
    }
    fclose(fp);

#ifdef DBG
    printf("Total lines in file: %u.\n", li);
#endif

#ifdef DBG
    for(i=0;i<txa;++i) {
        for(j=0;j<cols;++j) 
            printf("%c", alm[i][j]);
        printf("\n"); 
    }
    printf("\n"); 
#endif
    if(!strcmp(argv[1], "-a") ) { /* all columns */
        for(i=0;i<txa;++i) {
            printf("%s: ", nms[i]);
            for(j=0;j<cols;++j) 
                printf("%c", alm[i][j]);
            printf("\n"); 
        }
    } else if(!strcmp(argv[1], "-fa") ) { /* all columns in fasta format */
        for(i=0;i<txa;++i) {
            printf(">%s\n", nms[i]);
            for(j=0;j<cols;++j) 
                printf("%c", alm[i][j]);
            printf("\n"); 
        }
    } else if(!strcmp(argv[1], "-1") ) { /* nongap columns and first of the gap-columns */
        for(i=0;i<txa;++i) {
            printf("%s: ", nms[i]);
            for(j=0;j<cols;++j) 
                if((!gapa[j]) || (!gapa[j-1]) || (j==0) )
                    printf("%c", alm[i][j]);
            printf("\n"); 
        }
    } else if(!strcmp(argv[1], "-g") ) { /*only gap columns */
        for(i=0;i<txa;++i) {
            printf("%s: ", nms[i]);
            for(j=0;j<cols;++j) 
                if(gapa[j])
                    printf("%c", alm[i][j]);
            printf("\n"); 
        }
    } else if(!strcmp(argv[1], "-1g") ) { /* only the first gap columns */
        for(i=0;i<txa;++i) {
            printf("%s: ", nms[i]);
            for(j=0;j<cols;++j) 
                if(gapa[j] && ((!gapa[j-1]) || ((j==0))) )
                    printf("%c", alm[i][j]);
            printf("\n"); 
        }
    }

    for(i=0;i<txa;++i) 
        free(alm[i]);
    free(alm);
    for(i=0;i<txa;++i) 
        free(nms[i]);
    free(nms);


#ifdef DBG
    for(i=0;i<cols;++i) 
        printf("%u ", gapa[i]);
    printf("\n"); 
#endif

    free(gapa);
    return 0;
}
