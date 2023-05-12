#define _GNU_SOURCE
#include "parsnip.h"
#include "parse.h"

void setText(token* tok, char* text);
void dumpList (token *tok);
token* freeList(token* tok);

    int main(void) {
    // * TEST charInVarCHAR
    // if (charInVarChr('J')) {
    //     printf("True\n");
    // }
    // if (charInVarChr(']')) {
    //     printf("True\n");
    // }

    // * TEST RED_OP Macro
    // if RED_OP(RED_ERR_CLS) {
    //     printf("true\n");
    // }

    // * Test cloneToken
    // token* old = malloc(sizeof(token));
    // old->type = RED_ERR_CLS;
    // old->text = malloc(sizeof(char) * 3);
    // old->text[0] = 'H';
    // old->text[1] = 'i';
    // old->text[2] = '\0';
    // token* new = newTokenFrom(old);
    // freeToken(old);
    // freeToken(new);

    // * TEST synCommand()
    // token* list = malloc(sizeof(token) * 18);
    // list[0].type = SIMPLE;          setText(&(list[0]), "A=B");
    // list[1].type = SIMPLE;          setText(&(list[1]), "C");
    // list[2].type = RED_IN;          setText(&(list[2]), NULL);
    // list[3].type = SIMPLE;          setText(&(list[3]), "D");
    // list[4].type = SIMPLE;          setText(&(list[4]), "E");
    // list[5].type = PIPE;            setText(&(list[5]), NULL);
    // list[6].type = RED_OUT;         setText(&(list[6]), NULL);
    // list[7].type = SIMPLE;          setText(&(list[7]), "F");
    // list[8].type = SIMPLE;          setText(&(list[8]), "G=H");
    // list[9].type = PAR_LEFT;        setText(&(list[9]), NULL);
    // list[10].type = SIMPLE;         setText(&(list[10]), "I");             
    // list[11].type = SEP_END;        setText(&(list[11]), NULL);
    // list[12].type = SIMPLE;         setText(&(list[12]), "J");
    // list[13].type = PAR_RIGHT;      setText(&(list[13]), NULL);
    // list[14].type = RED_ERR_OUT;    setText(&(list[14]), NULL);
    // list[15].type = SEP_AND;        setText(&(list[15]), NULL);
    // list[16].type = SIMPLE;         setText(&(list[16]), "K");
    // list[17].type = NONE;           setText(&(list[17]), NULL);


    // * TEST 2
    // token* list = malloc(sizeof(token) * 18);
    // list[0].type = PAR_LEFT;        setText(&(list[0]), NULL);
    // list[1].type = SIMPLE;          setText(&(list[1]), "A=B");
    // list[2].type = PAR_LEFT;        setText(&(list[2]), NULL);
    // list[3].type = SIMPLE;          setText(&(list[3]), "C");
    // list[4].type = PAR_RIGHT;       setText(&(list[4]), NULL);
    // list[5].type = PAR_RIGHT;       setText(&(list[5]), NULL);
    // list[6].type = NONE;            setText(&(list[6]), NULL);

    // * TEST 3: zoo <file | bar (redirect counts across pipes)
    // token* list = malloc(sizeof(token) * 18);
    // list[0].type = SIMPLE;          setText(&(list[0]), "zoo");
    // list[1].type = RED_IN;          setText(&(list[1]), NULL);
    // list[2].type = SIMPLE;          setText(&(list[2]), "file");
    // list[3].type = PIPE;            setText(&(list[3]), NULL);
    // list[4].type = SIMPLE;          setText(&(list[4]), "bar");
    // list[5].type = NONE;            setText(&(list[5]), NULL);

    // * TEST Missing filename
    // token* list = malloc(sizeof(token) * 18);
    // list[0].type = SIMPLE;          setText(&(list[0]), "zoo");
    // list[1].type = RED_IN;          setText(&(list[1]), NULL);
    // list[2].type = NONE;            setText(&(list[2]), NULL);

    // * TEST redirects
    // (a >file) >file
    // token* list = malloc(sizeof(token) * 18);
    // list[0].type = PAR_LEFT;          setText(&(list[0]), NULL);
    // list[1].type = SIMPLE;          setText(&(list[1]), "a");
    // list[2].type = RED_OUT;            setText(&(list[2]), NULL);
    // list[3].type = SIMPLE;          setText(&(list[3]), "file");
    // list[4].type = PAR_RIGHT;       setText(&(list[4]), NULL);
    // list[5].type = RED_OUT;       setText(&(list[5]), NULL);
    // list[6].type = SIMPLE;            setText(&(list[6]), "file");
    // list[7].type = NONE;          setText(&(list[7]), NULL);

    // * TEST two subcommands
    // (a) (b)
    // token* list = malloc(sizeof(token) * 18);
    // list[0].type = PAR_LEFT;          setText(&(list[0]), NULL);
    // list[1].type = SIMPLE;               setText(&(list[1]), "a");
    // list[2].type = PAR_RIGHT;            setText(&(list[2]), NULL);
    // list[3].type = PAR_LEFT;          setText(&(list[3]), NULL);
    // list[4].type = SIMPLE;       setText(&(list[4]), "b");
    // list[5].type = PAR_RIGHT;       setText(&(list[5]), NULL);
    // list[6].type = NONE;            setText(&(list[6]), NULL);

    // * TEST weird example
    // token* list = malloc(sizeof(token) * 18);
    // list[0].type = SIMPLE;          setText(&(list[0]), "A");
    // list[1].type = SEP_END;          setText(&(list[1]), NULL);
    // list[2].type = SIMPLE;          setText(&(list[2]), "B");
    // list[3].type = SEP_BG;          setText(&(list[3]), NULL);
    // list[4].type = SIMPLE;          setText(&(list[4]), "C");
    // list[5].type = SEP_END;            setText(&(list[5]), NULL);
    // list[6].type = SIMPLE;         setText(&(list[6]), "D");
    // list[7].type = SEP_OR;          setText(&(list[7]), NULL);
    // list[8].type = SIMPLE;          setText(&(list[8]), "E");
    // list[9].type = SEP_END;        setText(&(list[9]), NULL);
    // list[10].type = NONE;         setText(&(list[10]), NULL);

    // * TEST umm what
    // token* list = malloc(sizeof(token) * 18);
    // list[0].type = SIMPLE;          setText(&(list[0]), "zoo");
    // list[1].type = NONE;          setText(&(list[1]), NULL);

    // * TEST here doc 1
    // token* list = malloc(sizeof(token) * 18);
    // list[0].type = SIMPLE;          setText(&(list[0]), "zoo");
    // list[1].type = RED_IN_HERE;          setText(&(list[1]), NULL);
    // list[2].type = SIMPLE;          setText(&(list[2]), "here");
    // list[3].type = NONE;          setText(&(list[3]), NULL);

    // * TEST local
    token* list = malloc(sizeof(token) * 18);
    list[0].type = SIMPLE;          setText(&(list[0]), "A=");
    list[1].type = SIMPLE;          setText(&(list[1]), "C");
    list[2].type = NONE;          setText(&(list[2]), NULL);

    token* parsed = parse(list);
    if (parsed != NULL) {
        dumpList(parsed);
    }
    freeList(list);

    return 0;
}


void setText(token* tok, char* text) {
    if (text == NULL) {
        tok->text = NULL;
        return;
    } else {
        tok->text = strdup(text);
    }
}


// Print list of tokens LIST
void dumpList (token *tok)
{
    for (int i = 0; tok[i].type != NONE; i++) { // Walk down linked list
        if (tok[i].type == SIMPLE)                  //   printing token and type
            printf ("%s:%d ", tok[i].text, tok[i].type);
        else if (tok[i].type == RED_IN)
            printf ("<%s ", tok[i].text);
        else if (tok[i].type == RED_IN_HERE)
            printf ("<<BEGIN-HERE\n%sEND-HERE ", tok[i].text);
        else if (tok[i].type == RED_OUT)
            printf (">%s ", tok[i].text);
        else if (tok[i].type == RED_OUT_APP)
            printf (">>%s ", tok[i].text);
        else if (tok[i].type == RED_ERR_OUT)
            printf ("2>&1 ");
        else if (tok[i].type == RED_ERR_CLS)
            printf ("2>&- ");
        else if (tok[i].type == PIPE)
            printf ("| ");
        else if (tok[i].type == SEP_AND)
            printf ("&& ");
        else if (tok[i].type == SEP_OR)
            printf ("|| ");
        else if (tok[i].type == SEP_END)
            printf ("; ");
        else if (tok[i].type == SEP_BG)
            printf ("& ");
        else if (tok[i].type == PAR_LEFT)
            printf ("( ");
        else if (tok[i].type == PAR_RIGHT)
            printf (") ");
        else if (tok[i].type == SUBCMD)
            printf ("SUBCMD ");
        else if (tok[i].type == LOCAL)
            printf ("LOCAL:%s ", tok[i].text);
        else if (tok[i].type == ARG)
            printf ("ARG:%s ", tok[i].text);
        else
            printf ("UNKNOWN:%d ", tok[i].type);
    }
    putchar ('\n');                             // Terminate line
}

token* freeList(token* tok) {
    for (int i = 0; tok[i].type != NONE; i++) {
        if (tok[i].text) free(tok[i].text);
        tok[i].type = NONE;
        tok[i].text = NULL;
    }
    free(tok);
    return NULL;
}
