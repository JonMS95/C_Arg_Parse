#ifndef GET_OPTIONS_API
#define GET_OPTIONS_API

/***********************************/
/******** Define statements ********/
/***********************************/

/******** Field sizes ********/

#define GET_OPT_SIZE_LONG_MAX   10
#define GET_OPT_SIZE_DETAIL_MAX 50

/*****************************/

/**********************************/
/******** Type definitions ********/
/**********************************/

typedef enum
{
    GET_OPT_TYPE_MIN    = 0,
    GET_OPT_TYPE_INT    = 0,
    GET_OPT_TYPE_CHAR   = 1,
    GET_OPT_TYPE_FLOAT  = 2,
    GET_OPT_TYPE_DOUBLE = 3,
    GET_OPT_TYPE_MAX    = 3,

} OPT_DATA_SUPPORTED_TYPES;

typedef enum
{
    GET_OPT_ARG_REQ_MIN         = 0,
    GET_OPT_ARG_REQ_NO          = 0,
    GET_OPT_ARG_REQ_REQUIRED    = 1,
    GET_OPT_ARG_REQ_OPTIONAL    = 2,
    GET_OPT_ARG_REQ_MAX         = 2,

} OPT_ARG_REQUIREMENT;

typedef union
{
    int     integer;
    char    character;
    float   floating;
    double  doubling;

} OPT_DATA_TYPE;

/// @brief Public structure, meant to be filled by the user.
typedef struct
{
    char            opt_char;
    char*           opt_long[GET_OPT_SIZE_LONG_MAX];
    char*           opt_detail[GET_OPT_SIZE_DETAIL_MAX];
    int             opt_var_type;
    int             opt_needs_arg;
    OPT_DATA_TYPE   opt_min_value;
    OPT_DATA_TYPE   opt_max_value;
    OPT_DATA_TYPE   opt_default_value;
    void*           opt_dest_var;

} PUB_OPT_DEFINITION;

/**********************************/

/*************************************/
/******** Function prototypes ********/
/*************************************/

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
int GetOptionDefinition(char            opt_char            ,
                        char*           opt_long            ,
                        char*           opt_detail          ,
                        int             opt_var_type        ,
                        int             opt_needs_arg       ,
                        OPT_DATA_TYPE   opt_min_value       ,
                        OPT_DATA_TYPE   opt_max_value       ,
                        OPT_DATA_TYPE   opt_default_value   ,
                        void*           opt_dest_var        );

/*************************************/

#endif
