#ifndef HYC_LEX_H
#define HYC_LEX_H

#include <args.h>
#include <tok.h>

struct HYC_LEXER
{
    hyc_args* args;

    FILE* is;
    
    size_t indent;
    
};
typedef struct HYC_LEXER lexer;

lexer* lexer_init(hyc_args* args, FILE* is);
void   lexer_fini(lexer* lxr);

token* lexer_next(lexer* lxr);
void   lexer_tok_fini(token* tok);

#endif