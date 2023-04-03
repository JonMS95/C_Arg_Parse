#ifndef GET_OPTIONS_H
#define GET_OPTIONS_H

/************************************/
/******** Include statements ********/
/************************************/

#include "GetOptions_api.h"
#include <stdbool.h>

/************************************/

/***********************************/
/******** Define statements ********/
/***********************************/

/******** Error codes ********/

#define GET_OPT_SUCCESS                     0
#define GET_OPT_ERR_NO_OPT_CHAR             -1
#define GET_OPT_ERR_NO_OPT_LONG             -2
#define GET_OPT_ERR_LONG_LENGTH_EXCEEDED    -3
#define GET_OPT_ERR_DETAIL_LENGTH_EXCEEDED  -4
#define GET_OPT_ERR_UNKNOWN_TYPE            -5
#define GET_OPT_ERR_UNKNOWN_ARG_REQ         -6
#define GET_OPT_ERR_WRONG_BOUNDARIES        -7
#define GET_OPT_ERR_DEF_VAL_OUT_OF_BOUNDS   -8
#define GET_OPT_ERR_NULL_DEST_VAR           -9

/******** Messages ********/
#define GET_OPT_MSG_NO_OPT_CHAR             "No option character provided.\r\n"
#define GET_OPT_MSG_NO_OPT_LONG             "No option long form provided.\r\n"
#define GET_OPT_MSG_LONG_LENGTH_EXCEEDED    "Long option definition exceeds maximum length.\r\n"
#define GET_OPT_MSG_NO_OPT_DETAIL           "No option detail provided.\r\n"
#define GET_OPT_MSG_DETAIL_LENGTH_EXCEEDED  "Option detail definition exceeds maximum length.\r\n"
#define GET_OPT_MSG_UNKNOWN_TYPE            "Unknown data type: -%c --%s (%s).\r\n"
#define GET_OPT_MSG_UNKNOWN_ARG_REQ         "Provided unknown argument requirement for the current option: -%c --%s (%s)\r\n"
#define GET_OPT_MSG_WRONG_BOUNDARIES        "Minimum > maximum for the current option: -%c --%s (%s).\r\n"
#define GET_OPT_MSG_DEF_VAL_OUT_OF_BOUNDS   "Default value is out of boundaries for current option: -%c --%s (%s).\r\n"
#define GET_OPT_MSG_NULL_DEST_VAR           "Current option destination value address is not set: -%c --%s (%s).\r\n"

/***********************************/

/**********************************/
/******** Type definitions ********/
/**********************************/

typedef struct 
{
    PUB_OPT_DEFINITION  pub_opt         ;
    bool                opt_has_value   ;

} private_option_definition;

/**********************************/

/*************************************/
/******** Function prototypes ********/
/*************************************/

/*************************************/

#endif