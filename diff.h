#ifndef DIFF_H_INCLUDED
#define DIFF_H_INCLUDED

typedef struct node{
    char chr[100];
    struct node *next;
}node;

typedef struct lines{
    char stack_lines1[100];
    char stack_lines2[100];
 }lines;

typedef node* list;

void init(list *head);
void insert_in_list(list *head, char ch[]);
void display_file_content(list head);
void difference(list l1, list l2);
int getline(list l1);
void conflictError();
void cPrintDiff(list l1, list l2, char *filename1, char *filename2);
void istrcmp(list *l1, list *l2, FILE *f1, FILE *f2);
void wstrcmp(list *l1, list *l2, FILE *f1, FILE *f2);
void bstrcmp(list *l1, list *l2, FILE *f1, FILE *f2);
void destroy(list *l1);
#endif // DIFF_H_INCLUDED
