#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <args.h>
#include <lex.h>
#include <hyc.h>

lexer* lexer_init(hyc_args* args, FILE* is)
{
    lexer* lxr = calloc(1, sizeof(lexer));

    lxr->args = args;
    lxr->is = is;
    lxr->indent = 0;

    return lxr;
}

void lexer_fini(lexer* lxr)
{
    if(lxr)
        free(lxr);
}


/* lexing */

int open_pbck(lexer* lxr)
{
    
}

token* lexer_next(lexer* lxr)
{
    
}

void lexer_tok_fini(token* tok)
{
    if(!tok)
        return;
    // switch tok->type, handle special cases

    if(tok->text)
        free(tok->text);
    if(tok->more)
        free(tok->more);
    free(tok);
}
