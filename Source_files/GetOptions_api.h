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

////////////////////////////////////////////////////////////
/// @brief Public structure, meant to be filled by the user.
////////////////////////////////////////////////////////////
typedef struct
{
    char            opt_char                                ;
    char*           opt_long[GET_OPT_SIZE_LONG_MAX + 1]     ;
    char*           opt_detail[GET_OPT_SIZE_DETAIL_MAX + 1] ;
    int             opt_var_type                            ;
    int             opt_needs_arg                           ;
    OPT_DATA_TYPE   opt_min_value                           ;
    OPT_DATA_TYPE   opt_max_value                           ;
    OPT_DATA_TYPE   opt_default_value                       ;
    void*           opt_dest_var                            ;

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
int SetOptionDefinition(char            opt_char            ,
                        char*           opt_long            ,
                        char*           opt_detail          ,
                        int             opt_var_type        ,
                        int             opt_needs_arg       ,
                        OPT_DATA_TYPE   opt_min_value       ,
                        OPT_DATA_TYPE   opt_max_value       ,
                        OPT_DATA_TYPE   opt_default_value   ,
                        void*           opt_dest_var        );

//////////////////////////////////////////////////////////////////////////////
/// @brief Set boolean option definition. 
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
int SetOptionDefinitionBool(char            opt_char            ,
                            char*           opt_long            ,
                            char*           opt_detail          ,
                            OPT_DATA_TYPE   opt_default_value   ,
                            void*           opt_dest_var        );

//////////////////////////////////////////////////////////////////////////////
/// @brief Set integer option definition.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_min_value Option minimum value.
/// @param opt_max_value Option maximum value.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
int SetOptionDefinitionInt( char            opt_char            ,
                            char*           opt_long            ,
                            char*           opt_detail          ,
                            int             opt_min_value       ,
                            int             opt_max_value       ,
                            int             opt_default_value   ,
                            void*           opt_dest_var        );

//////////////////////////////////////////////////////////////////////////////
/// @brief Set integer option definition.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
int SetOptionDefinitionIntNL(   char            opt_char            ,
                                char*           opt_long            ,
                                char*           opt_detail          ,
                                int             opt_default_value   ,
                                void*           opt_dest_var        );

//////////////////////////////////////////////////////////////////////////////
/// @brief Set char option definition.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_min_value Option minimum value.
/// @param opt_max_value Option maximum value.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
int SetOptionDefinitionChar(char            opt_char            ,
                            char*           opt_long            ,
                            char*           opt_detail          ,
                            char            opt_min_value       ,
                            char            opt_max_value       ,
                            char            opt_default_value   ,
                            void*           opt_dest_var        );

//////////////////////////////////////////////////////////////////////////////
/// @brief Set char option definition.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
int SetOptionDefinitionCharNL(  char            opt_char            ,
                                char*           opt_long            ,
                                char*           opt_detail          ,
                                char            opt_default_value   ,
                                void*           opt_dest_var        );

//////////////////////////////////////////////////////////////////////////////
/// @brief Set float option definition.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_min_value Option minimum value.
/// @param opt_max_value Option maximum value.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
int SetOptionDefinitionFloat(   char            opt_char            ,
                                char*           opt_long            ,
                                char*           opt_detail          ,
                                float           opt_min_value       ,
                                float           opt_max_value       ,
                                float           opt_default_value   ,
                                void*           opt_dest_var        );

//////////////////////////////////////////////////////////////////////////////
/// @brief Set float option definition.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
int SetOptionDefinitionFloatNL( char            opt_char            ,
                                char*           opt_long            ,
                                char*           opt_detail          ,
                                float           opt_default_value   ,
                                void*           opt_dest_var        );

//////////////////////////////////////////////////////////////////////////////
/// @brief Set double option definition.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_min_value Option minimum value.
/// @param opt_max_value Option maximum value.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
int SetOptionDefinitionDouble(  char            opt_char            ,
                                char*           opt_long            ,
                                char*           opt_detail          ,
                                double          opt_min_value       ,
                                double          opt_max_value       ,
                                double          opt_default_value   ,
                                void*           opt_dest_var        );

//////////////////////////////////////////////////////////////////////////////
/// @brief Set double option definition.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
int SetOptionDefinitionDoubleNL(char            opt_char            ,
                                char*           opt_long            ,
                                char*           opt_detail          ,
                                float           opt_default_value   ,
                                void*           opt_dest_var        );

/*************************************/

///////////////////////////////////////////////////////////////////////////
/// @brief Set multiple option definition taking a struct array as input. 
/// @param pub_opt_def Head of the array which includes option definitions.
/// @param pub_opt_def_size Array size.
/// @return < 0 if any error happened, 0 otherwise.
///////////////////////////////////////////////////////////////////////////
int GetOptDefFromStruct(PUB_OPT_DEFINITION* pub_opt_def, int pub_opt_def_size);

////////////////////////////////////////////////////////
/// @brief Parses given options and arguments if needed.
/// @param argc Argument count.
/// @param argv Argument character string array.
/// @return < 0 if any error happened, 0 otherwise.
////////////////////////////////////////////////////////
int ParseOptions(int argc, char** argv);

#endif
