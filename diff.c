#include<stdio.h>
#include"diff.h"
#include<limits.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
//initilize LL
void init(list *head) {
    *head = NULL;
    return;
}
//inserting char upto EOF
void insert_in_list(list *head, char ch[]) {
    node* nn = (node*) malloc(sizeof(node));
    if(nn) {
        strcpy(nn->chr, ch);
        nn->next = NULL;
    }
    else
        return;
    if(*head == NULL)
        *head = nn;
    else {
        node* p = *head;
        while(p->next)
            p = p->next;
        p->next = nn;
    }
    return;
}
//displays contents form file i.e from linked list
void display_file_content(list head) {
    if(head == NULL)
        printf("file is empty,nothing to show!\n");
    else {
        node* p = head;
        while(p) {
            printf("%s",  p->chr);
            p = p->next;
        }
    }
    printf("\n");
    return;
}
//used when both -y and -c options are passed by the user
void conflictError() {
	printf("diff: conflicting output style options\n");
}
//basic diff command with c a d mode
void difference(list l1, list l2) {
    node *p = l1, *q = l2;
    int flag = 0;
    while(p) {
        if(strcmp(p->chr, q->chr) == 0) {
            flag = 1;
            p = p->next;
            q = q->next;
        }
        else {
            flag = 0;
            break;
        }
    }
     if(flag == 1) {
        printf("files are identical\n");
        return;
    }
    lines st[20];
    p = l1;
    q = l2;
    char line1[100], line2[100];
    int line_count1 = 0,flag2[100],flag1[100],line_count2 = 0;
    int i = 0,j = 0,k = 0,n = 0,m = 0,d = 0;
    int ff;
    //calculating length or lines from file
    int a1 = getline(l1);
    int a2 = getline(l2);
    if(a1 == a2)
        ff = 0;
    else if(a1 < a2)
        ff = 1;
    else
        ff = 2;
    while(1) {
        line_count1++;
        line_count2++;
        if(p && q) {
            if(strcmp(p->chr, q->chr) == 0) {
                p = p->next;
                q = q->next;
                continue;
            }
            else {
                if(ff == 1) {
                    if(q->chr != NULL) {//for add mode
                    strcpy(st[i].stack_lines2, q->chr);
                    strcpy(st[i].stack_lines1, p->chr);
                    flag2[d] = line_count2;
                    d++;
                    flag1[m] = --line_count1;
                    m++;
                    q = q->next;
                    }
                }
                else if(ff == 2) {//for delete mode
                    if(p->chr != NULL) {
                    strcpy(st[i].stack_lines1, p->chr);
                    strcpy(st[i].stack_lines2, q->chr);
                    flag1[m] = line_count1;
                    m++;
                    flag2[d] = --line_count2;
                    d++;
                    p = p->next;
                    }
                }
                else { //for change mode
                    if(p->chr != NULL) {
                    strcpy(st[i].stack_lines1, p->chr);
                    flag1[m] = line_count1;
                    m++;
                    p = p->next;
                    }
                    if(q->chr != NULL) {
                    strcpy(st[i].stack_lines2, q->chr);
                    flag2[d] = line_count2;
                    d++;
                    q = q->next;
                    }
                }
            }
        }
        else if(p){
            strcpy(st[i].stack_lines1 , line1);
            flag1[m]=line_count1;
            m++;
            p = p->next;
        }
        else if(q){
            strcpy(st[i].stack_lines2 , line2);
            flag2[d]=line_count2;
            d++;
            q = q->next;
        }
        else
            break;
        i++;
        n++;
    }
    //printing ouput of simple diff
    for(i = 0;i < m;i++)
        printf("%d ",flag1[i]);
    if(ff == 0)
        printf("c");
    else if(ff == 1)
        printf("a");
    else
        printf("d");
    for(i = 0;i < d;i++)
        printf("%d ",flag2[i]);
    printf("\n");
    for(i = 0;i < n;i++)
        printf("<%s\n",st[i].stack_lines1);
    printf("---\n");
    for(i = 0;i < n;i++)
        printf(">%s\n",st[i].stack_lines2);

   return;
}
//returns number of lines from file
int getline(list l1) {
    node *p = l1;
    int cnt = 0;
    while(p) {
        cnt++;
        p = p->next;
    }
    return cnt;
}
//cPrintDiff prints the diff in context format
void cPrintDiff(list l1, list l2, char *filename1, char *filename2) {
    time_t t;   // not a primitive datatype
    time(&t);
    //printing date time in specified format with ctime
    printf("*** %s\t%s", filename1, ctime(&t));
    printf("--- %s\t%s", filename2, ctime(&t));
    printf("\n***************\n");
    node *p = l1, *q = l2;
    int flag = 0;
    //checking files are identical or not
    while(p && q) {
        if(strcmp(p->chr, q->chr) == 0) {
            flag = 1;
            p = p->next;
            q = q->next;
        }
        else {
            flag = 0;
            break;
        }
    }
     if(flag == 1) {
        p = l1;
        q = l2;
        printf("*** %s ****\n", filename1);
        while(p) {
            printf("  %s",p->chr);
            p = p->next;
        }
        printf("\n--- %s ----\n", filename2);
        while(q) {
            printf("  %s",q->chr);
            q = q->next;
        }
        return;
    }
    p = l1;
    q = l2;
    int line_count1 = -1,flag2[100],flag1[100],line_count2 = -1;
    int m = 0,d = 0;
    //storing line nnumber which not matches
    while(p || q) {
        line_count1++;
        line_count2++;
        if(strcmp(p->chr, q->chr) == 0) {
            p = p->next;
            q = q->next;
            continue;
        }
        else  {
            flag1[d] = line_count1;
            flag2[m] = line_count2;
            d++;
            m++;
            p = p->next;
            q = q->next;
        }
    }
    int range1 = getline(l1);
    int range2 = getline(l2);
    printf("*** 1,%d ****\n", range1);
    p = l1;
    int i = 0, j = 0;
    while(p) {
        if(i == flag1[j]) {
            printf("- %s", p->chr);
            j++;
        }
        else
            printf("  %s", p->chr);
        p = p->next;
        i++;
    }
    printf("\n*** 1,%d ****\n", ++range2);
    q = l2;
    i = 0;
    j = 0;
    while(q) {
        if(i == flag2[j]) {
            printf("+ %s", q->chr);
            j++;
        }
        else
            printf("  %s", q->chr);
        q = q->next;
        i++;
    }
    return;
}
//used if iFlag is 1(ignores uppercase and lowercase)
void istrcmp(list *l1, list *l2,FILE *f1, FILE *f2) {
    char word[100];
     while(fgets(word, sizeof(word), f1)) {
            for(int i = 0; word[i] != '\0'; i++) {
                if(word[i] >= 'A' && word[i] <= 'Z')
                    word[i] += 32;
            }
            insert_in_list(l1, word);
        }
        while(fgets(word, sizeof(word), f2)) {
            for(int j = 0; word[j] != '\0'; j++) {
                if(word[j] >= 'A' && word[j] <= 'Z')
                    word[j] += 32;
            }
            insert_in_list(l2, word);
        }
        return;
}
//used if wFlag is 1(ignores all the white spaces)
void wstrcmp(list *l1, list *l2, FILE *f1, FILE *f2) {
    char word[100], word1[100];
    int j;
    while(fgets(word, sizeof(word), f1)) {
            j = 0;
            for(int i = 0; word[i] != '\0'; i++)
                if(word[i] != ' ')
                    word1[j++] = word[i];
            word1[j] = '\0';
            if(!isEmpty(word1))
                insert_in_list(l1, word1);
            else
                continue;
        }
        while(fgets(word, sizeof(word), f2)) {
            j = 0;
            for(int i = 0; word[i] != '\0'; i++)
                if(word[i] != ' ')
                    word1[j++] = word[i];
            word1[j] = '\0';
            if(!isEmpty(word1))//checking line is empty or not
                insert_in_list(l2, word1);
            else
                continue;
        }
        return;
}
//to check line from file is empty or not
int isEmpty(char *str) {
    char ch;
    do {
        ch = *(str++);
        //check for non-white space character
        if(ch != ' ' && ch != '\t' && ch != '\n' && ch != '\0')
            return 0;
    }while(ch != '\0');
    return 1;
}
//used if bFlag is 1(ignores change in amount of existing white spaces) blank lines
void bstrcmp(list *l1, list *l2, FILE *f1, FILE *f2) {
    char word[100];
    while(fgets(word, sizeof(word), f1)) {
        if(!isEmpty(word))
            insert_in_list(l1, word);
        else
            continue;
    }
    while(fgets(word, sizeof(word), f2)) {
        if(!isEmpty(word))
            insert_in_list(l2, word);
        else
            continue;
    }
    return;
}
//destroy list
void destroy(list *l1) {
    node * curr = *l1;
    node* next = NULL;
    if(*l1 == NULL) {
        free(*l1);
        return;
    }
    else {
        while(curr != NULL) {
            next = curr->next;
            free(curr);
            curr = next;
        }
        free(*l1);
    }
    return;
}
