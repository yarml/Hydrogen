#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <debug.h>
#include <args.h>
#include <hyc.h>

hyc_args* args_parse(int argc, char** argv)
{
    hyc_args* args = calloc(1, sizeof(hyc_args)); // free() in args.c:args_clean
    args->argc = argc;
    args->argv = argv;
    
    char* of  = NULL;
    char* inf = NULL;


    /* parse arguments */
    int c;
    opterr = 0;
    while((c = getopt(argc, argv, "hvio:"), c != -1))
    {
        switch(c)
        {
            case 'h':
                print_help();
                exit(EXIT_SUCCESS);
            case 'v':
                print_ver();
                exit(EXIT_SUCCESS);
            case 'o':
                of = optarg;
                break;
            case '?':
                if(optopt == 'o')
                {
                    print_help();
                    exit_error(EXIT_ARGS, "Missing argument for -%c.\n", optopt);
                }
                else
                {
                    print_help();
                    if(isprint(c))
                        exit_error(EXIT_ARGS, "Unknown option argument -%c.\n", optopt);
                    else
                        exit_error(EXIT_ARGS, "Unknown option argument -\\x%x.\n", optopt);
                }
            default:
                exit_invlst(__FILE__, __LINE__);
        }
    }
    for(int i = optind; i < argc; ++i)
    {
        if(inf)
            exit_error(EXIT_ARGS, "Unknown argument `%s`\n", argv[i]);
        inf = argv[i];
    }
    
    /* open files */
    if(of)
    {
        stdout = freopen(of, "w", stdout);
        if(!stdout)
            exit_error(EXIT_ARGS, "Couldnt't open output file `%s`\n", of);
        DPRINTF("Using output file `%s`\n", of);
    }
    
    if(inf)
    {
        stdin = freopen(inf, "r", stdin);
        if(!stdin)
            exit_error(EXIT_ARGS, "Couldn't open input file `%s`\n", inf);
        DPRINTF("Using input file `%s`\n", inf);
    }

    return args;
}

void args_clean(hyc_args* args)
{
    free(args); // *alloc() in args.c:args_parse
}
