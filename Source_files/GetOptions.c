/************************************/
/******** Include statements ********/
/************************************/

#include "../Dependency_files/Header_files/SeverityLog_api.h"
#include "GetOptions.h"
#include <stdlib.h>
#include <string.h>

/************************************/

/***********************************/
/******** Define statements ********/
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
        if(strcmp(private_options[i].pub_opt.opt_long, current_opt_long) == 0)
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
static int CheckValidDataType(int opt_var_type)
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
static int CheckOptArgRequirement(int arg_requirement)
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
static int CheckOptLowerOrEqual(int opt_var_type, OPT_DATA_TYPE min, OPT_DATA_TYPE max)
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
static int CheckBoundaries(int opt_var_type    ,
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
/// @return GET_OPT_ERR_DEF_VAL_OUT_OF_BOUNDS if value is out of bounds, GET_OPT_SUCCESS otherwise.
///////////////////////////////////////////////////////////////////////////////////////////////////
static int CheckDefaultValue(  int             opt_var_type        ,
                        OPT_DATA_TYPE   opt_min_value     ,
                        OPT_DATA_TYPE   opt_max_value     ,
                        OPT_DATA_TYPE   opt_default_value )
{
    int check_def_val_be_min = CheckOptLowerOrEqual(opt_var_type, opt_min_value, opt_default_value);
    int check_def_val_le_max = CheckOptLowerOrEqual(opt_var_type, opt_default_value, opt_max_value);

    if( check_def_val_be_min < 0 ||
        check_def_val_le_max < 0)
    {
        return GET_OPT_ERR_DEF_VAL_OUT_OF_BOUNDS;
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
static int FillPrivateOptStruct(   char            opt_char            ,
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

    private_options[option_index].struct_opt_long.name      = private_options[option_index].pub_opt.opt_long;
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
int GetOptionDefinition(char            opt_char            ,
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
        SeverityLog(SVRTY_LVL_WNG, GET_OPT_MSG_NO_OPT_DETAIL);
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
        int check_default_value = CheckDefaultValue(opt_var_type        ,
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates short options string based on what's found in private_options.
/// @return GET_OPT_ERR_NULL_PTR if pointers to private options or short_options_string are uninitialized.
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static int GenerateShortOptStr(void)
{
    if(private_options        == NULL)
    {
        SeverityLog(SVRTY_LVL_DBG, "FILE: %s\tFUNCTION: %s\tLINE: %d\r\n", __FILE__, __func__, __LINE__);
        return GET_OPT_ERR_NULL_PTR;
    }

    char aux_short_options[3 * option_number + 1];
    memset(aux_short_options, 0, sizeof(aux_short_options));

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
static int GenerateOptLong(PRIV_OPT_LONG* priv_opt_long)
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

    int option;
    int option_index = 0;
    while((option = getopt_long(argc, argv, short_options_string, priv_opt_long, &priv_opt_long)) != -1)
    {
        if(option == -1)
        {
            break;
        }

        switch (option)
        {
            // Option sets a flag.
            case 0:
            {
                for(int i = 0; i < sizeof(priv_opt_long) / sizeof(priv_opt_long[0]); i++)
                {
                    if(strcmp(argv[optind], priv_opt_long[i].name == 0))
                    {
                        SeverityLog(SVRTY_LVL_DBG, "%s\r\n", priv_opt_long[i].name);
                        SeverityLog(SVRTY_LVL_INF, "%s flag set to %d.\r\n", getName(priv_opt_long[i].flag), *(priv_opt_long[i].flag));
                        break;
                    }
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

            }
            break;
        }
    }

    return GET_OPT_SUCCESS;
}

/**************************************/
