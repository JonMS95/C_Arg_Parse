/************************************/
/******** Include statements ********/
/************************************/

#include "SeverityLog_api.h"
#include "GetOptions.h"
#include <stdlib.h>
#include <string.h> // strlen, strcmp
#include <limits.h> // INT_MIN, INT_MAX, CHAR_MIN, CHAR_MAX
#include <float.h>  // FLT_MIN, FLT_MAX, DBL_MIN, DBL_MAX


/************************************/

/***********************************/
/******** Private constants ********/
/***********************************/

#define getName(var)  #var

/***********************************/

/***********************************/
/******** Private variables ********/
/***********************************/

/******** Private stack variables ********/

static int                  option_number           = 0;
static int                  verbose_flag            = 1;
static PRIV_OPT_LONG        opt_long_verbose        = {"verbose"    , no_argument   ,   &verbose_flag,  1};
static PRIV_OPT_LONG        opt_long_brief          = {"brief"      , no_argument   ,   &verbose_flag,  0};

/******** Private heap variables ********/

static char*                short_options_string    = NULL;
static PRIV_OPT_DEFINITION* private_options         = NULL;

/***********************************/

/**************************************/
/******** Function definitions ********/
/**************************************/

void FreeHeapOptData(void)
{
    if(short_options_string == NULL)
    {
        SeverityLog(SVRTY_LVL_WNG, GET_OPT_MSG_ALREADY_NULL_PTR);
        SeverityLog(SVRTY_LVL_DBG, "FILE: %s\tFUNCTION: %s\tLINE: %d\tPOINTER NAME: %s\r\n", __FILE__, __func__, __LINE__, getName(short_options_string));
    }
    else
    {
        free(short_options_string);
        short_options_string = NULL;
        option_number = 0;
    }

    if(private_options == NULL)
    {
        SeverityLog(SVRTY_LVL_WNG, GET_OPT_MSG_ALREADY_NULL_PTR);
        SeverityLog(SVRTY_LVL_DBG, "FILE: %s\tFUNCTION: %s\tLINE: %d\tPOINTER NAME: %s\r\n", __FILE__, __func__, __LINE__, getName(private_options));
    }
    else
    {
        free(private_options);
        private_options = NULL;
        option_number = 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Looks for every character in private option definitions. Checks if the given character already exists.
/// @param current_opt_char Character that's meant to be checked. 
/// @return GET_OPT_ERR_OPT_CHAR_ALREADY_EXISTS if the character already exists, 0 otherwise.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CheckExistingOptionChar(char current_opt_char)
{
    for(int i = 0; i < option_number; i++)
    {
        if(private_options[i].pub_opt.opt_char == current_opt_char)
        {
            return GET_OPT_ERR_OPT_CHAR_ALREADY_EXISTS;
        }
    }

    return GET_OPT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Looks for every character in private option definitions. Checks if the given long option string already exists.
/// @param current_opt_char Long option that's meant to be checked. 
/// @return GET_OPT_ERR_OPT_LONG_ALREADY_EXISTS if the character already exists, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CheckExistingOptionLong(char* current_opt_long)
{
    for(int i = 0; i < option_number; i++)
    {
        if(strcmp((char*)(private_options[i].pub_opt.opt_long), current_opt_long) == 0)
        {
            SeverityLog(SVRTY_LVL_WNG,
                        GET_OPT_MSG_OPT_LONG_ALREADY_EXISTS,
                        private_options[i].pub_opt.opt_char,
                        private_options[i].pub_opt.opt_long,
                        private_options[i].pub_opt.opt_detail);
            return GET_OPT_ERR_OPT_LONG_ALREADY_EXISTS;
        }
    }

    return GET_OPT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Checks if the data type specifier is valid or not.
/// @param opt_var_type Option variable type.
/// @return GET_OPT_ERR_UNKNOWN_TYPE if the data type is unknown, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
int CheckValidDataType(int opt_var_type)
{
    for(int i = GET_OPT_TYPE_MIN; i <= GET_OPT_TYPE_MAX; i++)
    {
        if(opt_var_type == i)
        {
            return GET_OPT_SUCCESS;
        }
    }

    return GET_OPT_ERR_UNKNOWN_TYPE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks if the argument requirement specifier is valid or not.
/// @param arg_requirement Argument requirement level (GET_OPT_ARG_REQ_NO, GET_OPT_ARG_REQ_REQUIRED, GET_OPT_ARG_REQ_OPTIONAL).
/// @return GET_OPT_ERR_UNKNOWN_ARG_REQ if the option argument requirement specifier is unknown, 0 otherwise.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CheckOptArgRequirement(int arg_requirement)
{
    for(int i = GET_OPT_ARG_REQ_MIN; i <= GET_OPT_ARG_REQ_MAX; i++)
    {
        if(arg_requirement == i)
        {
            return GET_OPT_SUCCESS;
        }
    }

    return GET_OPT_ERR_UNKNOWN_ARG_REQ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks whether if the minimum value is lower or equal than the maximum or not, regardless of the type.
/// @param opt_var_type Option variable type.
/// @param min Minimum value.
/// @param max Maximum value.
/// @return GET_OPT_ERR_WRONG_BOUNDARIES if min > max, 0 otherwise.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CheckOptLowerOrEqual(int opt_var_type, OPT_DATA_TYPE min, OPT_DATA_TYPE max)
{
    switch (opt_var_type)
    {
        case GET_OPT_TYPE_INT:
        {
            int minimum = min.integer;
            int maximum = max.integer;

            if(minimum > maximum)
            {
                return GET_OPT_ERR_WRONG_BOUNDARIES;
            }
        }
        break;

        case GET_OPT_TYPE_CHAR:
        {
            char minimum = min.character;
            char maximum = max.character;

            if(minimum > maximum)
            {
                return GET_OPT_ERR_WRONG_BOUNDARIES;
            }
        }
        break;

        case GET_OPT_TYPE_FLOAT:
        {
            float minimum = min.floating;
            float maximum = max.floating;

            if(minimum > maximum)
            {
                return GET_OPT_ERR_WRONG_BOUNDARIES;
            }
        }
        break;

        case GET_OPT_TYPE_DOUBLE:
        {
            double minimum = min.doubling;
            double maximum = max.doubling;

            if(minimum > maximum)
            {
                return GET_OPT_ERR_WRONG_BOUNDARIES;
            }
        }
        break;
        
        default:
        break;
    }

    return GET_OPT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////
/// @brief Checks if the boundaries are consistent (min <= max) or not.
/// @param opt_var_type Option variable type.
/// @param opt_min_value Option minimum value.
/// @param opt_max_value Option maximum value.
/// @return GET_OPT_ERR_WRONG_BOUNDARIES if min > max, 0 otherwise.
///////////////////////////////////////////////////////////////////////
int CheckBoundaries(int opt_var_type    ,
                    OPT_DATA_TYPE opt_min_value ,
                    OPT_DATA_TYPE opt_max_value )
{
    int check_boundaries = CheckOptLowerOrEqual(opt_var_type, opt_min_value, opt_max_value);

    if(check_boundaries < 0)
    {
        return check_boundaries;
    }

    return GET_OPT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Checks whether if the default option value exists within boundaries or not.
/// @param opt_var_type Option variable type.
/// @param opt_min_value Option minimum value.
/// @param opt_max_value Option maximum value.
/// @param opt_default_value Option default value.
/// @return GET_OPT_ERR_VAL_OUT_OF_BOUNDS if value is out of bounds, GET_OPT_SUCCESS otherwise.
///////////////////////////////////////////////////////////////////////////////////////////////////
int CheckValueInRange(   int             opt_var_type        ,
                                OPT_DATA_TYPE   opt_min_value     ,
                                OPT_DATA_TYPE   opt_max_value     ,
                                OPT_DATA_TYPE   opt_check_value )
{
    int check_def_val_be_min = CheckOptLowerOrEqual(opt_var_type, opt_min_value, opt_check_value);
    int check_def_val_le_max = CheckOptLowerOrEqual(opt_var_type, opt_check_value, opt_max_value);

    if( check_def_val_be_min < 0 ||
        check_def_val_le_max < 0)
    {
        return GET_OPT_ERR_VAL_OUT_OF_BOUNDS;
    }

    return GET_OPT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generate private option struct (allocated in heap) once an option definition has been retrieved.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_var_type Option variable type.
/// @param opt_needs_arg Specifies if the option needs arguments or not.
/// @param opt_min_value Option minimum value.
/// @param opt_max_value Option maximum value.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return GET_OPT_ERR_OPT_NUM_ZERO if there are no options defined, 0 otherwise..
///////////////////////////////////////////////////////////////////////////////////////////////////////////
int FillPrivateOptStruct(   char            opt_char            ,
                            char*           opt_long            ,
                            char*           opt_detail          ,
                            int             opt_var_type        ,
                            int             opt_needs_arg       ,
                            OPT_DATA_TYPE   opt_min_value       ,
                            OPT_DATA_TYPE   opt_max_value       ,
                            OPT_DATA_TYPE   opt_default_value   ,
                            void*           opt_dest_var        )
{
    option_number++;

    if(option_number == 0)
    {
        return GET_OPT_ERR_OPT_NUM_ZERO;
    }

    if(option_number == 1)
    {
        private_options = (PRIV_OPT_DEFINITION*)calloc(option_number, sizeof(PRIV_OPT_DEFINITION));
    }
    else
    {
        private_options = (PRIV_OPT_DEFINITION*)realloc(private_options, option_number * sizeof(PRIV_OPT_DEFINITION));
    }

    int option_index = option_number - 1;
    
    private_options[option_index].pub_opt.opt_char  = opt_char;
    strcpy((char*)(private_options[option_index].pub_opt.opt_long), opt_long);

    if(opt_detail != NULL)
    {
        strcpy((char*)(private_options[option_index].pub_opt.opt_detail), opt_detail);
    }

    private_options[option_index].pub_opt.opt_var_type      = opt_var_type      ;
    private_options[option_index].pub_opt.opt_needs_arg     = opt_needs_arg     ;
    private_options[option_index].pub_opt.opt_min_value     = opt_min_value     ;
    private_options[option_index].pub_opt.opt_max_value     = opt_max_value     ;
    private_options[option_index].pub_opt.opt_default_value = opt_default_value ;
    private_options[option_index].pub_opt.opt_dest_var      = opt_dest_var      ;

    private_options[option_index].struct_opt_long.name      = (char*)private_options[option_index].pub_opt.opt_long;
    private_options[option_index].struct_opt_long.has_arg   = private_options[option_index].pub_opt.opt_needs_arg;
    private_options[option_index].struct_opt_long.flag      = NULL;
    private_options[option_index].struct_opt_long.val       = private_options[option_index].pub_opt.opt_char;

    private_options[option_index].opt_has_value = false;

    return GET_OPT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Gets and checks option definition.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_var_type Option variable type.
/// @param opt_needs_arg Specifies if the option needs arguments or not.
/// @param opt_min_value Option minimum value.
/// @param opt_max_value Option maximum value.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
int SetOptionDefinition(char            opt_char            ,
                        char*           opt_long            ,
                        char*           opt_detail          ,
                        int             opt_var_type        ,
                        int             opt_needs_arg       ,
                        OPT_DATA_TYPE   opt_min_value       ,
                        OPT_DATA_TYPE   opt_max_value       ,
                        OPT_DATA_TYPE   opt_default_value   ,
                        void*           opt_dest_var        )
{
    // Check if option character exists.
    if(opt_char == '\0')
    {
        SeverityLog(SVRTY_LVL_ERR, GET_OPT_MSG_NO_OPT_CHAR);
        FreeHeapOptData();
        return GET_OPT_ERR_NO_OPT_CHAR;
    }

    // If the option character is valid, check whether it already exists.
    int check_existing_option_char = CheckExistingOptionChar(opt_char);

    if(check_existing_option_char < 0)
    {
        // SeverityLog is called within CheckExistingOptionChar, as long as the data to be shown depends on the private options data.
        FreeHeapOptData();
        return check_existing_option_char;
    }

    // Check if option long string exists.
    if(opt_long == NULL)
    {
        SeverityLog(SVRTY_LVL_ERR, GET_OPT_MSG_NO_OPT_LONG);
        FreeHeapOptData();
        return GET_OPT_ERR_NO_OPT_LONG;
    }

    // If option long string exists, then check if its length exceeds the allowed maximum.
    if(opt_long != NULL)
    {
        if(strlen(opt_long) > GET_OPT_SIZE_LONG_MAX)
        {
            SeverityLog(SVRTY_LVL_ERR, GET_OPT_MSG_LONG_LENGTH_EXCEEDED);
            FreeHeapOptData();
            return GET_OPT_ERR_LONG_LENGTH_EXCEEDED;
        }
    }

    // If the option long string is valid, check whether it already exists.
    int check_existing_option_long = CheckExistingOptionLong(opt_long);

    if(check_existing_option_long < 0)
    {
        // SeverityLog is called within CheckExistingOptionLong, as long as the data to be shown depends on the private options data.
        FreeHeapOptData();
        return check_existing_option_long;
    }

    // Check if option detail exists.
    if(opt_detail == NULL)
    {
        SeverityLog(SVRTY_LVL_WNG               ,
                    GET_OPT_MSG_NO_OPT_DETAIL   ,
                    opt_char                    ,
                    opt_long                    );
    }

    // If option detail exists, then check if its length exceeds the allowed maximum.
    if(opt_detail != NULL)
    {
        if(strlen(opt_detail) > GET_OPT_SIZE_DETAIL_MAX)
        {
            SeverityLog(SVRTY_LVL_ERR, GET_OPT_MSG_DETAIL_LENGTH_EXCEEDED);
            FreeHeapOptData();
            return GET_OPT_ERR_DETAIL_LENGTH_EXCEEDED;
        }
    }

    // Check if the specified data type is understood by the program.
    int check_valid_data_type = CheckValidDataType(opt_var_type);

    if(check_valid_data_type < 0)
    {
        SeverityLog(SVRTY_LVL_ERR           ,
                    GET_OPT_MSG_UNKNOWN_TYPE,
                    opt_char                ,
                    opt_long                ,
                    opt_detail              );
        FreeHeapOptData();
        return check_valid_data_type;
    }
    
    // Check whether if the option requires an argument or not.
    int check_opt_arg_requirement = CheckOptArgRequirement(opt_needs_arg);
    
    if(check_opt_arg_requirement < 0)
    {
        SeverityLog(SVRTY_LVL_ERR               ,
                    GET_OPT_MSG_UNKNOWN_ARG_REQ ,
                    opt_char                    ,
                    opt_long                    ,
                    opt_detail                  );
        FreeHeapOptData();
        return check_opt_arg_requirement;
    }

    // If the current option requires arguments, then:
    if(opt_needs_arg != GET_OPT_ARG_REQ_NO)
    {
        // Check if minimum and maximum values are consistent or not.
        int check_opt_min_max = CheckBoundaries(opt_var_type, opt_min_value, opt_max_value);
        
        if(check_opt_min_max < 0)
        {
            SeverityLog(SVRTY_LVL_ERR               ,
                        GET_OPT_MSG_WRONG_BOUNDARIES,
                        opt_char                    ,
                        opt_long                    ,
                        opt_detail                  );
            FreeHeapOptData();
            return check_opt_min_max;
        }

        // Check if default value is within boundaries or not.
        int check_default_value = CheckValueInRange(opt_var_type        ,
                                                    opt_min_value       ,
                                                    opt_max_value       ,
                                                    opt_default_value   );

        if(check_default_value < 0)
        {
            SeverityLog(SVRTY_LVL_ERR                       ,
                        GET_OPT_MSG_DEF_VAL_OUT_OF_BOUNDS   ,
                        opt_char                            ,
                        opt_long                            ,
                        opt_detail                          );
            FreeHeapOptData();
            return check_default_value;
        }
    }

    // Check whether if the pointer to the target output variable is null or not.
    if(opt_dest_var == NULL)
    {
        SeverityLog(SVRTY_LVL_ERR               , 
                    GET_OPT_MSG_NULL_DEST_VAR   ,
                    opt_char                    ,
                    opt_long                    ,
                    opt_detail                  );
        FreeHeapOptData();
        return GET_OPT_ERR_NULL_DEST_VAR;
    }

    // Generate private option definition based on the values received by the current function.
    int fill_private_opt_struct =   FillPrivateOptStruct(   opt_char            ,
                                                            opt_long            ,
                                                            opt_detail          ,
                                                            opt_var_type        ,
                                                            opt_needs_arg       ,
                                                            opt_min_value       ,
                                                            opt_max_value       ,
                                                            opt_default_value   ,
                                                            opt_dest_var        );

    if(fill_private_opt_struct < 0)
    {
        SeverityLog(SVRTY_LVL_ERR, GET_OPT_MSG_OPT_NUM_ZERO);
        FreeHeapOptData();
        return fill_private_opt_struct;
    }

    return GET_OPT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////
/// @brief Set multiple option definition taking a struct array as input. 
/// @param pub_opt_def Head of the array which includes option definitions.
/// @param pub_opt_def_size Array size.
/// @return < 0 if any error happened, 0 otherwise.
///////////////////////////////////////////////////////////////////////////
int GetOptDefFromStruct(PUB_OPT_DEFINITION* pub_opt_def, int pub_opt_def_size)
{
    for(int i = 0; i < pub_opt_def_size; i++)
    {
        int set_opt_def =   SetOptionDefinition(pub_opt_def[i].opt_char         ,
                                                (char*)pub_opt_def[i].opt_long  ,
                                                (char*)pub_opt_def[i].opt_detail,
                                                pub_opt_def[i].opt_var_type     ,
                                                pub_opt_def[i].opt_needs_arg    ,
                                                pub_opt_def[i].opt_min_value    ,
                                                pub_opt_def[i].opt_max_value    ,
                                                pub_opt_def[i].opt_default_value,
                                                pub_opt_def[i].opt_dest_var     );
        
        if(set_opt_def < 0)
        {
            return set_opt_def;
        }
    }

    return GET_OPT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates short options string based on what's found in private_options.
/// @return GET_OPT_ERR_NULL_PTR if pointers to private options or short_options_string are uninitialized.
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int GenerateShortOptStr(void)
{
    if(private_options        == NULL)
    {
        SeverityLog(SVRTY_LVL_DBG, "FILE: %s\tFUNCTION: %s\tLINE: %d\r\n", __FILE__, __func__, __LINE__);
        return GET_OPT_ERR_NULL_PTR;
    }

    char aux_short_options[1 + 3 * option_number + 1];
    memset(aux_short_options, 0, sizeof(aux_short_options));
    aux_short_options[0] = ':';

    for(int i = 0; i < option_number; i++)
    {
        aux_short_options[strlen(aux_short_options)] = private_options[i].pub_opt.opt_char;

        switch (private_options[i].pub_opt.opt_needs_arg)
        {
            case GET_OPT_ARG_REQ_REQUIRED:
            {
                aux_short_options[strlen(aux_short_options)] = ':';
            }
            break;

            case GET_OPT_ARG_REQ_OPTIONAL:
            {
                aux_short_options[strlen(aux_short_options)] = ':';
                aux_short_options[strlen(aux_short_options)] = ':';
            }
            break; 

            case GET_OPT_ARG_REQ_NO:
            default:
            break;
        }
    }

    short_options_string = (char*)calloc(strlen(aux_short_options), sizeof(char));
    strcpy(short_options_string, aux_short_options);

    return GET_OPT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////
/// @brief Generates the data needed by the long options parser.
/// @param priv_opt_long Pointer to the struct array that's meant to be populated.
/// @return GET_OPT_ERR_NULL_PTR if the pointer is null, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////////
int GenerateOptLong(PRIV_OPT_LONG* priv_opt_long)
{
    if(priv_opt_long == NULL)
    {
        SeverityLog(SVRTY_LVL_DBG, "FILE: %s\tFUNCTION: %s\tLINE: %d\r\n", __FILE__, __func__, __LINE__);
        return GET_OPT_ERR_NULL_PTR;
    }

    memcpy(&priv_opt_long[0], &opt_long_verbose, sizeof(PRIV_OPT_LONG));
    memcpy(&priv_opt_long[1], &opt_long_brief, sizeof(PRIV_OPT_LONG));

    for(int i = GET_OPT_SIZE_VERB_BRIEF; i < GET_OPT_SIZE_VERB_BRIEF + option_number; i++)
    {
        memcpy(&priv_opt_long[i], &private_options[i - GET_OPT_SIZE_VERB_BRIEF].struct_opt_long, sizeof(PRIV_OPT_LONG));
    }
    
    memset(&priv_opt_long[GET_OPT_SIZE_VERB_BRIEF + option_number], 0, sizeof(PRIV_OPT_LONG));

    return GET_OPT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Casts the provided option argument to the proper type and stores it.
/// @param priv_opt_def Private option definition.
/// @param arg Argument to be casted.
/// @param dest Destination variable.
///////////////////////////////////////////////////////////////////////////////
void CastParsedArgument(PRIV_OPT_DEFINITION* priv_opt_def, char* arg, OPT_DATA_TYPE* dest)
{
    int var_type = priv_opt_def->pub_opt.opt_var_type;

    switch(var_type)
    {
        case GET_OPT_TYPE_INT:
        {
            dest->integer = atoi(arg);
        }
        break;

        case GET_OPT_TYPE_CHAR:
        {
            if(strlen(optarg) > 1)
            {
                SeverityLog(SVRTY_LVL_WNG                   ,
                            GET_OPT_MSG_STRING_NOT_CHAR     ,
                            priv_opt_def->pub_opt.opt_char  ,
                            priv_opt_def->pub_opt.opt_long  ,
                            priv_opt_def->pub_opt.opt_detail);
            }
            dest->character = (char)atoi(arg);
        }
        break;

        case GET_OPT_TYPE_FLOAT:
        {
            dest->floating = atof(arg);
        }
        break;

        case GET_OPT_TYPE_DOUBLE:
        {
            dest->doubling = strtod(arg, NULL);
        }
        break;

        default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////
/// @brief Assign value to the destination variable.
/// @param priv_opt_def Private option definition, where the target variable is found.
/// @param src Variable which stores the value to set.
//////////////////////////////////////////////////////////////////////////////////////
void AssignValue(PRIV_OPT_DEFINITION* priv_opt_def, OPT_DATA_TYPE src)
{
    switch(priv_opt_def->pub_opt.opt_var_type)
    {
        case GET_OPT_TYPE_INT:
        {
            *(int*)(priv_opt_def->pub_opt.opt_dest_var) = src.integer;
        }
        break;

        case GET_OPT_TYPE_CHAR:
        {
            *(char*)(priv_opt_def->pub_opt.opt_dest_var) = src.character;
        }
        break;

        case GET_OPT_TYPE_FLOAT:
        {
            *(float*)(priv_opt_def->pub_opt.opt_dest_var) = src.floating;
        }
        break;

        case GET_OPT_TYPE_DOUBLE:
        {
            *(double*)(priv_opt_def->pub_opt.opt_dest_var) = src.doubling;
        }
        break;

        default:
        break;
    }

    priv_opt_def->opt_has_value = true;
}

////////////////////////////////////////////////////////
/// @brief Parses given options and arguments if needed.
/// @param argc Argument count.
/// @param argv Argument character string array.
/// @return < 0 if any error happened, 0 otherwise.
////////////////////////////////////////////////////////
int ParseOptions(int argc, char** argv)
{
    int generate_short_options_string = GenerateShortOptStr();
    
    if(generate_short_options_string < 0)
    {
        SeverityLog(SVRTY_LVL_ERR, GET_OPT_MSG_NULL_PTR);
        FreeHeapOptData();
        return generate_short_options_string;
    }

    PRIV_OPT_LONG priv_opt_long[GET_OPT_SIZE_VERB_BRIEF + option_number + 1];

    int generate_opt_long = GenerateOptLong(priv_opt_long);

    if(generate_opt_long < 0)
    {
        SeverityLog(SVRTY_LVL_ERR, GET_OPT_MSG_NULL_PTR);
        FreeHeapOptData();
        return generate_opt_long;
    }

    int current_option;
    int current_option_index = 0;
    while((current_option = getopt_long(argc, argv, short_options_string, priv_opt_long, &current_option_index)) != -1)
    {
        if(current_option == -1)
        {
            break;
        }

        switch (current_option)
        {
            // Option sets a flag.
            case 0:
            {
                if(verbose_flag == 0)
                {
                    SetSeverityLogMask(SVRTY_LOG_MASK_ERR);
                }
                else
                {
                    SetSeverityLogMask(SVRTY_LOG_MASK_ALL);
                }
            }
            break;

            // Missing option argument.
            case ':':
            {
                SeverityLog(SVRTY_LVL_ERR, GET_OPT_MSG_NO_ARG_FOUND, optopt);
                FreeHeapOptData();
                return GET_OPT_ERR_NO_ARG_FOUND;
            }
            break;

            // Invalid option.
            case '?':
            {
                SeverityLog(SVRTY_LVL_ERR, GET_OPT_MSG_UNKNOWN_OPTION, optopt);
                FreeHeapOptData();
                return GET_OPT_ERR_UNKNOWN_OPTION;
            }
            break;

            default:
            {
                // First of all, get the index of the current option within the private option structure array.

                int current_option_index;
                for(current_option_index = 0; current_option_index < option_number; current_option_index++)
                {
                    if(private_options[current_option_index].pub_opt.opt_char == current_option)
                    {
                        break;
                    }
                }

                // Check if the option is boolean.
                if(private_options[current_option_index].pub_opt.opt_needs_arg == GET_OPT_ARG_REQ_NO)
                {
                    *((bool*)(private_options[current_option_index].pub_opt.opt_dest_var)) = (bool)atoi(optarg);
                    private_options[current_option_index].opt_has_value = true;
                    // Go to next option.
                    break;
                }

                if(private_options[current_option_index].pub_opt.opt_needs_arg == GET_OPT_ARG_REQ_OPTIONAL)
                {
                    // Check whether anyvalue has been provided for the current option.
                    if(optarg == NULL)
                    {
                        // Go to next option. The default value will be assigned to the current option later.
                        break;
                    }
                }

                OPT_DATA_TYPE parsed_argument;
                int check_value_in_range;

                CastParsedArgument(&private_options[current_option_index], optarg, &parsed_argument);

                // Check if the provided value fits in the range delimited by the option's boundaries
                check_value_in_range = CheckValueInRange(   private_options[current_option_index].pub_opt.opt_var_type  ,
                                                            private_options[current_option_index].pub_opt.opt_min_value ,
                                                            private_options[current_option_index].pub_opt.opt_max_value ,
                                                            parsed_argument                                             );

                if(check_value_in_range < 0)
                {
                    SeverityLog(SVRTY_LVL_ERR                                           ,
                                GET_OPT_MSG_PROV_VAL_OUT_OF_BOUNDS                      ,
                                private_options[current_option_index].pub_opt.opt_char  ,
                                private_options[current_option_index].pub_opt.opt_long  ,
                                private_options[current_option_index].pub_opt.opt_detail);
                    FreeHeapOptData();
                    return check_value_in_range;
                }

                // If the value provided value is OK, then assign it to the destination variable.
                AssignValue(&private_options[current_option_index], parsed_argument);
            }
            break;
        }
    }

    // For each option, check if any value has been provided (has_value).
    // If not, give it its default value.
    for(int current_option_index = 0; current_option_index < option_number; current_option_index++)
    {
        if(private_options[current_option_index].opt_has_value == false)
        {
            // If the value provided value is OK, then assign it to the destination variable.
            AssignValue(&private_options[current_option_index], private_options[current_option_index].pub_opt.opt_default_value);
            private_options[current_option_index].opt_has_value = true;
        }
    }

    ShowOptions();

    return GET_OPT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
/// @brief Changes the printf formatting characters depending on the variable type.
///        Use %z to refer to the formatters that are meant to be changed.
/// @param string_to_format Target string to be formatted.
/// @param data_type Data type.
/// @return Unmodified string to format if data_type is an unknown data type,
/// formatted string otherwise.
///////////////////////////////////////////////////////////////////////////////////
char* GetOptionsGenFormattedStr(char* string_to_format, int data_type)
{
    bool percentage_found = false;
    char target_formatter;

    switch (data_type)
    {
        case GET_OPT_TYPE_INT:
        {
            target_formatter = 'd';
        }
        break;

        case GET_OPT_TYPE_CHAR:
        {
            target_formatter = 'c';
        }
        break;

        case GET_OPT_TYPE_FLOAT:
        case GET_OPT_TYPE_DOUBLE:
        {
            target_formatter = 'f';
        }
        break;
        
        default:
        {
            SeverityLog(SVRTY_LVL_ERR, GET_OPT_MSG_UNKNOWN_TYPE);
            return string_to_format;
        }
        break;
    }

    for(int i = 0; i < strlen(string_to_format); i++)
    {
        if(string_to_format[i] == '%')
        {
            percentage_found = true;
            continue;
        }

        if(percentage_found == true && string_to_format[i] == 'z')
        {
            string_to_format[i] = target_formatter;
        }

        percentage_found = false;
    }

    return string_to_format;
}

void ShowOptions()
{
    LOG_INF(GET_OPT_MSG_OPT_SUMMARY_HEADER);
    for(int option_num = 0; option_num < option_number; option_num++)
    {
        char option_summary_msg[GET_OPT_SIZE_DETAIL_MAX + 1];

        LOG_INF(GET_OPT_MSG_OPT_NAME, private_options[option_num].pub_opt.opt_char, private_options[option_num].pub_opt.opt_long);
        
        LOG_INF(GET_OPT_MSG_OPT_DESC, private_options[option_num].pub_opt.opt_detail);
        
        memset(option_summary_msg, 0, sizeof(option_summary_msg)); strcpy(option_summary_msg, GET_OPT_MSG_OPT_MIN_VALUE);
        LOG_INF(GetOptionsGenFormattedStr(option_summary_msg, private_options[option_num].pub_opt.opt_var_type), private_options[option_num].pub_opt.opt_min_value);
        
        memset(option_summary_msg, 0, sizeof(option_summary_msg)); strcpy(option_summary_msg, GET_OPT_MSG_OPT_MAX_VALUE);
        LOG_INF(GetOptionsGenFormattedStr(option_summary_msg, private_options[option_num].pub_opt.opt_var_type), private_options[option_num].pub_opt.opt_max_value);
        
        memset(option_summary_msg, 0, sizeof(option_summary_msg)); strcpy(option_summary_msg, GET_OPT_MSG_OPT_DEFAULT_VALUE);
        LOG_INF(GetOptionsGenFormattedStr(option_summary_msg, private_options[option_num].pub_opt.opt_var_type), private_options[option_num].pub_opt.opt_default_value);

        memset(option_summary_msg, 0, sizeof(option_summary_msg)); strcpy(option_summary_msg, GET_OPT_MSG_OPT_ASSIGNED_VALUE);
        switch(private_options[option_num].pub_opt.opt_var_type)
        {
            case GET_OPT_TYPE_INT:
                LOG_INF(GetOptionsGenFormattedStr(option_summary_msg, private_options[option_num].pub_opt.opt_var_type), *((int*)(private_options[option_num].pub_opt.opt_dest_var)));
            break;

            case GET_OPT_TYPE_CHAR:
                LOG_INF(GetOptionsGenFormattedStr(option_summary_msg, private_options[option_num].pub_opt.opt_var_type), *((char*)(private_options[option_num].pub_opt.opt_dest_var)));
            break;

            case GET_OPT_TYPE_FLOAT:
                LOG_INF(GetOptionsGenFormattedStr(option_summary_msg, private_options[option_num].pub_opt.opt_var_type), *((float*)(private_options[option_num].pub_opt.opt_dest_var)));
            break;

            case GET_OPT_TYPE_DOUBLE:
                LOG_INF(GetOptionsGenFormattedStr(option_summary_msg, private_options[option_num].pub_opt.opt_var_type), *((double*)(private_options[option_num].pub_opt.opt_dest_var)));
            break;
            
            default:
            break;
        }
        LOG_INF(GET_OPT_MSG_OPT_SUMMARY_FOOTER);
    }
}

/**************************************/
