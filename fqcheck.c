#include <stdio.h>
#include "zlib.h"
#include "kseq.h"
#include <string.h>

KSEQ_INIT(gzFile, gzread)

int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "usage fqcheck <in_fq1.gz> <in_fq2.gz>\n");
        return 1;
    }

    gzFile fq1_p, fq2_p;
    if((fq1_p = gzopen(argv[1], "r")) == NULL ) {
        fprintf(stderr, "can't open %s\n", argv[1]);
        return 2;
    }
    if((fq2_p = gzopen(argv[2], "r")) == NULL ) {
        fprintf(stderr, "can't open %s\n", argv[2]);
        return 2;
    }

    kseq_t *seq_1, *seq_2;
    seq_1 = kseq_init(fq1_p);
    seq_2 = kseq_init(fq2_p);
    
    int l1 = 0, l2 = 0;
    l1 = kseq_read(seq_1);
    l2 = kseq_read(seq_2);
    while ( l1 >= 0 && l2 >= 0) {
        seq_1->name.s[strlen(seq_1->name.s)-1] = '\0';
        seq_2->name.s[strlen(seq_2->name.s)-1] = '\0';
        //printf("%s\t%s\n", seq_1->name.s, seq_2->name.s);
        if(strcmp(seq_1->name.s, seq_2->name.s) != 0) {
            printf("fq1 seq name different from seq2:\t%s\t%s\n%s\n%s\n", seq_1->name.s, seq_2->name.s, argv[1], argv[2]);
            return 1;
        }
        l1 = kseq_read(seq_1);
        l2 = kseq_read(seq_2);
    }
    if (l1 == -2 ) {
        printf("fq1 seq length is differnt with qual\n");
        printf("%s\n", seq_1->name.s);
        printf("%s\n", seq_1->seq.s);
        printf("%s\n", seq_1->qual.s);
		printf("%s\n", argv[1]);
		printf("%s\n", argv[2]);
        return  2;
    }

    if (l2 == -2 ) {
        printf("fq2 seq length is differnt with qual\n");
        printf("%s\n", seq_2->name.s);
        printf("%s\n", seq_2->seq.s);
        printf("%s\n", seq_2->qual.s);
		printf("%s\n", argv[1]);
		printf("%s\n", argv[2]);
        return  2;
    }

    if ((l1 >=0 && l2 < 0) || (l1 < 0 && l2 >=0) ) {
        printf("fq1 seq number is different with fq2\n");
		printf("%s\n", argv[1]);
		printf("%s\n", argv[2]);
        return  3;
    }
    return 0;
}
