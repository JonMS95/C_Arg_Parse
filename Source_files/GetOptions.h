#ifndef GET_OPTIONS_H
#define GET_OPTIONS_H

/************************************/
/******** Include statements ********/
/************************************/

#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h> // malloc in SetOptionDefinitionStringNL
#include "GetOptions_api.h"

/************************************/

/***********************************/
/******** Define statements ********/
/***********************************/

/***** Private variables *****/

#define GET_OPT_HOME_OS_DIR_NAME            "HOME"

/******** Field sizes ********/

#define GET_OPT_SIZE_VERB_BRIEF             2

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
#define GET_OPT_ERR_VAL_OUT_OF_BOUNDS       -10
#define GET_OPT_ERR_NULL_DEST_VAR           -11
#define GET_OPT_ERR_OPT_NUM_ZERO            -12
#define GET_OPT_ERR_NULL_PTR                -13
#define GET_OPT_ERR_NO_ARG_FOUND            -14
#define GET_OPT_ERR_UNKNOWN_OPTION          -15

/******** Messages ********/
#define GET_OPT_MSG_NO_OPT_CHAR             "No option character provided."
#define GET_OPT_MSG_OPT_CHAR_ALREADY_EXISTS "Option char already exists: -%c --%s (%s)."
#define GET_OPT_MSG_NO_OPT_LONG             "No option long form provided."
#define GET_OPT_MSG_LONG_LENGTH_EXCEEDED    "Long option definition exceeds maximum length."
#define GET_OPT_MSG_OPT_LONG_ALREADY_EXISTS "Option long string already exists -%c --%s (%s)."
#define GET_OPT_MSG_NO_OPT_DETAIL           "No option detail provided for the current option: -%c --%s."
#define GET_OPT_MSG_DETAIL_LENGTH_EXCEEDED  "Option detail definition exceeds maximum length."
#define GET_OPT_MSG_UNKNOWN_TYPE            "Unknown data type: -%c --%s (%s)."
#define GET_OPT_MSG_UNKNOWN_ARG_REQ         "Provided unknown argument requirement for the current option: -%c --%s (%s)"
#define GET_OPT_MSG_WRONG_BOUNDARIES        "Minimum > maximum for the current option: -%c --%s (%s)."
#define GET_OPT_MSG_DEF_VAL_OUT_OF_BOUNDS   "Default value is out of boundaries for current option: -%c --%s (%s)."
#define GET_OPT_MSG_NULL_DEST_VAR           "Current option destination value address is not set: -%c --%s (%s)."
#define GET_OPT_MSG_OPT_NUM_ZERO            "Current option number is zero. Should not have reached this point."
#define GET_OPT_MSG_NULL_PTR                "NULL pointer found."
#define GET_OPT_MSG_ALREADY_NULL_PTR        "Pointer to memory meant to be freed is NULL."
#define GET_OPT_MSG_NO_ARG_FOUND            "Option %c requires an argument!"
#define GET_OPT_MSG_UNKNOWN_OPTION          "Unknown option (%c)."
#define GET_OPT_MSG_STRING_NOT_CHAR         "Passed string as argument to an option that requires a character instead: -%c --%s (%s)."
#define GET_OPT_MSG_PROV_VAL_OUT_OF_BOUNDS  "Provided value is out of boundaries for current option: -%c --%s (%s)."
#define GET_OPT_MSG_OPT_SUMMARY_HEADER      "*********** Options summary ***********"
#define GET_OPT_MSG_OPT_SUMMARY_FOOTER      "***************************************"
#define GET_OPT_MSG_OPT_NAME                "Option: %*s%c"
#define GET_OPT_MSG_OPT_NAME_LONG           "Option long: %*s%s"
#define GET_OPT_MSG_OPT_DESC                "Description: %*s%s"
#define GET_OPT_STR_FORMAT_PRECEDENT        '%'
#define GET_OPT_STR_FORMAT_TARGET           '@'
#define GET_OPT_MSG_OPT_MIN_VALUE           "Minimum value: %*s%@"
#define GET_OPT_MSG_OPT_MAX_VALUE           "Maximum value: %*s%@"
#define GET_OPT_MSG_OPT_DEFAULT_VALUE       "Default value: %*s%@"
#define GET_OPT_MSG_OPT_ASSIGNED_VALUE      "Assigned value: %*s%@"
#define GET_OPT_MSG_OPT_VAL_SEPARATOR       ""
#define GET_OPT_MSG_OPT_MIN_STR_VALUE       "Minimum value: %*s\\0"
#define GET_OPT_MSG_OPT_MAX_STR_VALUE       "Maximum value: %*sUCHAR_MAX * %d"

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

void FreeHeapOptData(void);
int CheckExistingOptionChar(char current_opt_char);
int CheckExistingOptionLong(char* current_opt_long);
int CheckValidDataType(int opt_var_type);
int CheckOptArgRequirement(int arg_requirement);
int CheckOptLowerOrEqual(int opt_var_type, OPT_DATA_TYPE min, OPT_DATA_TYPE max);
int CheckBoundaries(int opt_var_type    ,
             OPT_DATA_TYPE opt_min_value ,
             OPT_DATA_TYPE opt_max_value );
int CheckValueInRange(   int             opt_var_type        ,
                         OPT_DATA_TYPE   opt_min_value     ,
                         OPT_DATA_TYPE   opt_max_value     ,
                         OPT_DATA_TYPE   opt_check_value );
int FillPrivateOptStruct(char            opt_char            ,
                         char*           opt_long            ,
                         char*           opt_detail          ,
                         int             opt_var_type        ,
                         int             opt_needs_arg       ,
                         OPT_DATA_TYPE   opt_min_value       ,
                         OPT_DATA_TYPE   opt_max_value       ,
                         OPT_DATA_TYPE   opt_default_value   ,
                         void*           opt_dest_var        );
int GenerateShortOptStr(void);
int GenerateOptLong(PRIV_OPT_LONG* priv_opt_long);
void CastParsedArgument(PRIV_OPT_DEFINITION* priv_opt_def, char* arg, OPT_DATA_TYPE* dest);
void AssignValue(PRIV_OPT_DEFINITION* priv_opt_def, OPT_DATA_TYPE src);
char* GetOptionsGenFormattedStr(char* string_to_format, int data_type);
void PrintBoundaryData(char* option_summary_msg, int var_type, int blank_spaces_count, OPT_DATA_TYPE var_to_print);
void ShowOptions(void);
void GetOptionsExpandPath(char** src_short_path);

/*************************************/

#endif