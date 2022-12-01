#ifndef HYC_ERROR_H
#define HYC_ERROR_H

#include <stdnoreturn.h>

#include <dtc/status.h>

#define HYC_EXIT_INV_STATE (-1)
#define HYC_EXIT_ARGS      (-2)


noreturn
status hyc_exit(int exit_code,  char const* exit_msg, ...);

#define hyc_exit_invstate(msg, ...) (\
            hyc_exit(\
                HYC_EXIT_INV_STATE,\
                "Invalid state: " msg "\n",\
                __VA_ARGS__\
            )\
        )

#endif