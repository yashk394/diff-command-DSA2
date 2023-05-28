#include<stdio.h>
#include"diff.h"
#include<limits.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
/*below flags represent the status of the options specified in the command line, if not mention in comman line then they
   are reset*/
int iFlag = 0;
int wFlag = 0;
int cFlag = 0;
int yFlag = 0;
int bFlag = 0;
int main(int argc, char *argv[]) {
    int option, fileCount = 0;
	extern int optind;
	char *argument1, *argument2;
	argument1 = (char *)malloc(sizeof(char) * 1024);//name of the first argument
	argument2 = (char *)malloc(sizeof(char) * 1024);//name of the second argument
    char argument3[20];
	//getting the file name from the command line
	while(argv[optind] != NULL) {
		if(fileCount == 0)
			strcpy(argument1, argv[optind]);
		else if(fileCount == 1)
			strcpy(argument2, argv[optind]);
        else
			strcpy(argument3, argv[optind]);
		optind++;
		if(fileCount == 2) {
			fileCount++;
			break;
		}
		fileCount++;
	}
	//missing operand error
	if(fileCount < 2) {
		printf("diff: missing operand after '%s'\n", argv[optind - 1]);
		return 0;
	}
	//extra operand error
	if(fileCount > 3) {
		printf("diff: extra operand '%s'\n", argv[optind - 1]);
		return 0;
	}
	if(!strcmp(argument1, argument2))//if file names are same
		return 0;

    //falgs seting
    if(strcmp(argument3, "-c") == 0) {
        if(!yFlag)
            cFlag = 1;
        else {
            conflictError();
            return 0;
        }
    }
    else if(strcmp(argument3, "-i") == 0)
        iFlag = 1;
    else if(strcmp(argument3, "-w") == 0)
        wFlag = 1;
    else if(strcmp(argument3, "-iw") == 0) {
        iFlag = 1;
        wFlag = 1;
    }
    else if(strcmp(argument3, "-b") == 0)
        bFlag = 1;
    list l1, l2;
    init(&l1);
    init(&l2);
    int j = 0, i = 0;
    char word[100], ch, word1[100];
    FILE *f1, *f2;
    f1 = fopen(argument1, "r");
    f2 = fopen(argument2, "r");
    if(!f1 || !f2) {
        printf("not able to open file\n");
        exit(0);
    }
    while(fgets(word, sizeof(word), f1))
        insert_in_list(&l1, word);
    while(fgets(word, sizeof(word), f2))
        insert_in_list(&l2, word);
    printf("contents of file 1\n");
    display_file_content(l1);
    printf("contents of file 2\n");
    display_file_content(l2);
    fseek(f1, 0, SEEK_SET);//reseting both file pointers
    fseek(f2, 0, SEEK_SET);
    list l3, l4;
    init(&l3);
    init(&l4);
    printf("\n");
    if(iFlag && !wFlag) {
        istrcmp(&l3, &l4, f1, f2);
        difference(l3, l4);
    }
    else if(wFlag && !iFlag) {
        wstrcmp(&l3, &l4, f1, f2);
        difference(l3, l4);
    }
    else if(cFlag)
        cPrintDiff(l1, l2, argument1, argument2);
    else if(iFlag && wFlag) {
        wstrcmp(&l3, &l4, f1, f2);
        istrcmp(&l3, &l4, f1, f2);
        difference(l3, l4);
    }
    else if(bFlag) {
        bstrcmp(&l3, &l4, f1, f2);
        difference(l3, l4);
    }

    else {
        difference(l1, l2);
    }

    //destroying all lists
    destroy(&l1);
    destroy(&l2);
    destroy(&l3);
    destroy(&l4);
    fclose(f1);//closing opened files
    fclose(f2);
    return 0;
}
