#include "../Dependency_files/Header_files/SeverityLog_api.h"
#include "GetOptions.h"
#include <stdlib.h>
#include <string.h>

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
int CheckOptLowerOrEqual(int opt_var_type, void* min, void* max)
{
    switch (opt_var_type)
    {
        case GET_OPT_TYPE_INT:
        {
            int minimum = *((int*)min);
            int maximum = *((int*)max);

            if(minimum > maximum)
            {
                return GET_OPT_ERR_WRONG_BOUNDARIES;
            }
        }
        break;

        case GET_OPT_TYPE_CHAR:
        {
            char minimum = *((char*)min);
            char maximum = *((char*)max);

            if(minimum > maximum)
            {
                return GET_OPT_ERR_WRONG_BOUNDARIES;
            }
        }
        break;

        case GET_OPT_TYPE_FLOAT:
        {
            float minimum = *((float*)min);
            float maximum = *((float*)max);

            if(minimum > maximum)
            {
                return GET_OPT_ERR_WRONG_BOUNDARIES;
            }
        }
        break;

        case GET_OPT_TYPE_DOUBLE:
        {
            double minimum = *((double*)min);
            double maximum = *((double*)max);

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
                    void* opt_min_value ,
                    void* opt_max_value )
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
int CheckDefaultValue(  int opt_var_type        ,
                        void* opt_min_value     ,
                        void* opt_max_value     ,
                        void* opt_default_value )
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
/// @return < 0 if any error happened.
//////////////////////////////////////////////////////////////////////////////
int GetOptionDefinition(char    opt_char            ,
                        char*   opt_long            ,
                        char*   opt_detail          ,
                        int     opt_var_type        ,
                        int     opt_needs_arg       ,
                        void*   opt_min_value       ,
                        void*   opt_max_value       ,
                        void*   opt_default_value   ,
                        void*   opt_dest_var        )
{
    // Check if option character exists.
    if(opt_char == '\0')
    {
        SeverityLog(SVRTY_LVL_ERR, GET_OPT_MSG_NO_OPT_CHAR);
        return GET_OPT_ERR_NO_OPT_CHAR;
    }

    // Check if option long string exists.
    if(opt_long == NULL)
    {
        SeverityLog(SVRTY_LVL_ERR, GET_OPT_MSG_NO_OPT_LONG);
        return GET_OPT_ERR_NO_OPT_LONG;
    }

    // If option long string exists, then check if its length exceeds the allowed maximum.
    if(opt_long != NULL)
    {
        if(strlen(opt_long) > GET_OPT_SIZE_LONG_MAX)
        {
            SeverityLog(SVRTY_LVL_ERR, GET_OPT_MSG_LONG_LENGTH_EXCEEDED);
            return GET_OPT_ERR_LONG_LENGTH_EXCEEDED;
        }
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
        return check_opt_arg_requirement;
    }

    // Check if minimum and maximum values are consistent or not.
    int check_opt_min_max = CheckBoundaries(opt_var_type, opt_min_value, opt_max_value);
    
    if(check_opt_min_max < 0)
    {
        SeverityLog(SVRTY_LVL_ERR               ,
                    GET_OPT_MSG_WRONG_BOUNDARIES,
                    opt_char                    ,
                    opt_long                    ,
                    opt_detail                  );
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
        return check_default_value;
    }

    // Check whether if the pointer to the target output variable is null or not.
    if(opt_dest_var == NULL)
    {
        SeverityLog(SVRTY_LVL_ERR               , 
                    GET_OPT_MSG_NULL_DEST_VAR   ,
                    opt_char                    ,
                    opt_long                    ,
                    opt_detail                  );
        return GET_OPT_ERR_NULL_DEST_VAR;
    }

    return GET_OPT_SUCCESS;
}
