#define _GNU_SOURCE
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parsnip.h"

// * LIST DATA STRUCTURE

#define RESIZE_FACTOR 2

typedef struct list {
    int count;           // no of items added to list
    int cap;             // the capacity of the list
    token* tokens;  // list of tokens (heap allocated)

} list;

// * SYNTAX FUNCTIONS 

// <argument> = SIMPLE
bool synArgument(list* l, token* arr, int* pos);

// <filename> = SIMPLE
bool synFilename(list* l, token* arr, int* pos);

// <local> = VARIABLE=VALUE
bool synLocal(list* l, token* arr, int* pos);

// <red_op> = < / << / > / >>
bool synRedOp(list* l, token* arr, int* pos);

// <red_sym> = 2>&1 / 2>&-
bool synRedSym(list* l, token* arr, int* pos);

// <redirect> = <red_op> <filename> / <red_sym>
bool synRedirect(list* l, token* arr, int* pos);

// <prefix> = [ <local> / <redirect> ]*
bool synPrefix(list* l, token* arr, int* pos);

// <suffix> = [ <argument> / <redirect> ]*
bool synSuffix(list* l, token* arr, int* pos);

// <red_list> = [ <redirect ]*
bool synRedList(list* l, token* arr, int* pos);

// <simple> = <prefix> <argument> <suffix>
bool synSimple(list* l, token* arr, int* pos);

// <subcmd> = <prefix> (command) <red_list>
bool synSubCmd(list* l, token* arr, int* pos);

// <stage> = <simple> / <subcmd>
bool synStage(list* l, token* arr, int* pos);

// <pipeline> = <stage> [ | <stage> ]*
bool synPipeline(list* l, token* arr, int* pos);

// <and_or> = <pipeline> [ && <pipeline> / || <pipeline> ]*
bool synAndOr(list* l, token* arr, int* pos);

// <list> = <and_or> [ ; <and_or> / & <and_or> ]*
bool synList(list* l, token* arr, int* pos);

// <command> = <list> / <list> ; / <list> &
bool synCommand(list* l, token* arr, int* pos);

// * LIST DATA STRUCTURE

void lSetup(list* l);

void lDestroy(list* l);

void lPush(list* l, token* tok);

token* lGet(list* l, int index);

void lExpand(list* l);

int lCount(list* l);

list* lCopy(list* l);

void lDeleteNumber(list *l, int no);

void lDeleteAll(list *l);

void lDeleteLast(list* l);

void lCap(list *l);

void lStageReorder(list *l, int start_of_stage);

// * UTILITY FUNCTIONS

char* concat(char* s1, char* s2, char* mode);

void initString(char* arr, int len);

bool charInVarChr(char c);

bool isSimple(char c);

// * TOKEN FUNCTIONS

token* newTokenFrom(token* src);

void copyToken(token* dest, token* src);

void emptyToken(token *t);

void freeToken(token* t);

token* newErrorToken(void);

token* newNoneToken(void);

token* newTerminatorToken(void);

char* printTokenType(int type);
