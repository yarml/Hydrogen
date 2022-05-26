#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <debug.h>
#include <args.h>
#include <hyc.h>

hyc_args* args_parse(int argc, char** argv)
{
    hyc_args* args = calloc(1, sizeof(hyc_args)); // free() in args.c:args_clean
    args->argc = argc;
    args->argv = argv;
    char* of = 0;

    /* default values */
    args->is = stdin ;
    args->os = stdout;
    
    /* parse arguments */
    int c;
    opterr = 0;
    while((c = getopt(argc, argv, "hvdo:"), c != -1))
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
                    if(isprint(c))
                        exit_error(EXIT_ARGS, "Unknown option argument -%c.\n", optopt);
                    else
                        exit_error(EXIT_ARGS, "Unknown option argument -\\x%x.\n", optopt);
                }
            default:
                exit_invlst(__FILE__, __LINE__);
        }
    if(of)
    {
        args->os = fopen(of, "w"); // fclose() in args.c:args_clean
        if(!args->os)
            exit_error(EXIT_ARGS, "Couldnt't open output file `%s`\n", of);
        DPRINTF("Using output file `%s`\n", of);
    }
    for(int i = optind; i < argc; ++i)
    {
        if(args->is && args->is != stdin)
            exit_error(EXIT_ARGS, "Unknown argument `%s`\n", argv[i]);
        args->is = fopen(argv[i], "r"); // fclose() in args.c:args_clean
        if(!args->is)
            exit_error(EXIT_ARGS, "Couldn't open input file `%s`\n", argv[i]);
        DPRINTF("Using input file `%s`\n", argv[i]);
    }
    return args;
}

void args_clean(hyc_args* args)
{
    if(args->os && args->os != stdout)
        fclose(args->os); // fopen() in args.c:args_parse
    if(args->is && args->is != stdin)
        fclose(args->is); // fopen() in args.c:args_parse
    free(args); // *alloc() in args.c:args_parse
}
