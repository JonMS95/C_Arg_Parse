#ifndef GET_OPTIONS_H
#define GET_OPTIONS_H

/************************************/
/******** Include statements ********/
/************************************/

#include "GetOptions_api.h"
#include <stdbool.h>
#include <getopt.h>

/************************************/

/***********************************/
/******** Define statements ********/
/***********************************/

/******** Field sizes ********/

#define GET_OPT_SIZE_VERB_BRIEF 2

/******** Error codes ********/

#define GET_OPT_SUCCESS                     0
#define GET_OPT_ERR_NO_OPT_CHAR             -1
#define GET_OPT_ERR_OPT_CHAR_ALREADY_EXISTS -2
#define GET_OPT_ERR_NO_OPT_LONG             -3
#define GET_OPT_ERR_LONG_LENGTH_EXCEEDED    -4
#define GET_OPT_ERR_OPT_LONG_ALREADY_EXISTS -5
#define GET_OPT_ERR_DETAIL_LENGTH_EXCEEDED  -6
#define GET_OPT_ERR_UNKNOWN_TYPE            -7
#define GET_OPT_ERR_UNKNOWN_ARG_REQ         -8
#define GET_OPT_ERR_WRONG_BOUNDARIES        -9
#define GET_OPT_ERR_DEF_VAL_OUT_OF_BOUNDS   -10
#define GET_OPT_ERR_NULL_DEST_VAR           -11
#define GET_OPT_ERR_OPT_NUM_ZERO            -12
#define GET_OPT_ERR_NULL_PTR                -13

/******** Messages ********/
#define GET_OPT_MSG_NO_OPT_CHAR             "No option character provided.\r\n"
#define GET_OPT_MSG_OPT_CHAR_ALREADY_EXISTS "Option char already exists: -%c --%s (%s).\r\n"
#define GET_OPT_MSG_NO_OPT_LONG             "No option long form provided.\r\n"
#define GET_OPT_MSG_LONG_LENGTH_EXCEEDED    "Long option definition exceeds maximum length.\r\n"
#define GET_OPT_MSG_OPT_LONG_ALREADY_EXISTS "Option long string already exists -%c --%s (%s).\r\n"
#define GET_OPT_MSG_NO_OPT_DETAIL           "No option detail provided.\r\n"
#define GET_OPT_MSG_DETAIL_LENGTH_EXCEEDED  "Option detail definition exceeds maximum length.\r\n"
#define GET_OPT_MSG_UNKNOWN_TYPE            "Unknown data type: -%c --%s (%s).\r\n"
#define GET_OPT_MSG_UNKNOWN_ARG_REQ         "Provided unknown argument requirement for the current option: -%c --%s (%s)\r\n"
#define GET_OPT_MSG_WRONG_BOUNDARIES        "Minimum > maximum for the current option: -%c --%s (%s).\r\n"
#define GET_OPT_MSG_DEF_VAL_OUT_OF_BOUNDS   "Default value is out of boundaries for current option: -%c --%s (%s).\r\n"
#define GET_OPT_MSG_NULL_DEST_VAR           "Current option destination value address is not set: -%c --%s (%s).\r\n"
#define GET_OPT_MSG_OPT_NUM_ZERO            "Current option number is zero. Should not have reached this point.\r\n"
#define GET_OPT_MSG_NULL_PTR                "NULL pointer found.\r\n"

/***********************************/

/**********************************/
/******** Type definitions ********/
/**********************************/

typedef struct option PRIV_OPT_LONG;

typedef struct 
{
    PUB_OPT_DEFINITION  pub_opt         ;
    PRIV_OPT_LONG       struct_opt_long ;
    bool                opt_has_value   ;

} PRIV_OPT_DEFINITION;

/**********************************/

/*************************************/
/******** Function prototypes ********/
/*************************************/

/*************************************/

#endif