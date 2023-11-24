#ifndef GET_OPTIONS_API
#define GET_OPTIONS_API

#ifdef __cplusplus
extern "C" {
#endif

/************************************/
/******** Include statements ********/
/************************************/

#include <limits.h> // INT_MIN, INT_MAX, CHAR_MIN, CHAR_MAX
#include <float.h>  // FLT_MIN, FLT_MAX, DBL_MIN, DBL_MAX

/************************************/

/***********************************/
/******** Private constants ********/
/***********************************/

/******** Field sizes ********/

#define C_GET_OPTIONS_API __attribute__((visibility("default")))

#define GET_OPT_SIZE_LONG_MAX           20
#define GET_OPT_SIZE_DETAIL_MAX         50

/*****************************/

/**********************************/
/******** Type definitions ********/
/**********************************/

typedef enum C_GET_OPTIONS_API
{
    GET_OPT_TYPE_MIN            = 0,
    GET_OPT_TYPE_INT            = 0,
    GET_OPT_TYPE_CHAR           = 1,
    GET_OPT_TYPE_FLOAT          = 2,
    GET_OPT_TYPE_DOUBLE         = 3,
    GET_OPT_TYPE_CHAR_STRING    = 4,
    GET_OPT_TYPE_MAX            = 4,

} OPT_DATA_SUPPORTED_TYPES;

typedef enum C_GET_OPTIONS_API
{
    GET_OPT_ARG_REQ_MIN         = 0,
    GET_OPT_ARG_REQ_NO          = 0,
    GET_OPT_ARG_REQ_REQUIRED    = 1,
    GET_OPT_ARG_REQ_OPTIONAL    = 2,
    GET_OPT_ARG_REQ_MAX         = 2,

} OPT_ARG_REQUIREMENT;

typedef union C_GET_OPTIONS_API
{
    int     integer;
    char    character;
    float   floating;
    double  doubling;
    char*   char_string;

} OPT_DATA_TYPE;

////////////////////////////////////////////////////////////
/// @brief Public structure, meant to be filled by the user.
////////////////////////////////////////////////////////////
typedef struct C_GET_OPTIONS_API
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
C_GET_OPTIONS_API int SetOptionDefinition(  char            opt_char            ,
                                            char*           opt_long            ,
                                            char*           opt_detail          ,
                                            int             opt_var_type        ,
                                            int             opt_needs_arg       ,
                                            OPT_DATA_TYPE   opt_min_value       ,
                                            OPT_DATA_TYPE   opt_max_value       ,
                                            OPT_DATA_TYPE   opt_default_value   ,
                                            void*           opt_dest_var        );

//////////////////////////////////////////////////////////////////////////////
/// @brief Set double option definition without boundaries.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
C_GET_OPTIONS_API int SetOptionDefinitionStringNoLimits(char opt_char           ,
                                                        char* opt_long          ,
                                                        char* opt_detail        ,
                                                        char* opt_default_value ,
                                                        void* opt_dest_var      );

//////////////////////////////////////////////////////////////////////////////
/// @brief Set boolean option definition. 
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
#define SetOptionDefinitionBool(opt_char            ,                           \
                                opt_long            ,                           \
                                opt_detail          ,                           \
                                opt_default_value   ,                           \
                                opt_dest_var        )                           \
                                                                                \
        SetOptionDefinition(opt_char                                        ,   \
                            opt_long                                        ,   \
                            opt_detail                                      ,   \
                            GET_OPT_TYPE_INT                                ,   \
                            GET_OPT_ARG_REQ_NO                              ,   \
                            (OPT_DATA_TYPE){.integer = 0}                   ,   \
                            (OPT_DATA_TYPE){.integer = 1}                   ,   \
                            (OPT_DATA_TYPE){.integer = opt_default_value}   ,   \
                            opt_dest_var                                    )   

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
#define SetOptionDefinitionInt( opt_char            ,                               \
                                opt_long            ,                               \
                                opt_detail          ,                               \
                                opt_min_value       ,                               \
                                opt_max_value       ,                               \
                                opt_default_value   ,                               \
                                opt_dest_var        )                               \
                                                                                    \
        SetOptionDefinition(    opt_char                                        ,   \
                                opt_long                                        ,   \
                                opt_detail                                      ,   \
                                GET_OPT_TYPE_INT                                ,   \
                                GET_OPT_ARG_REQ_REQUIRED                        ,   \
                                (OPT_DATA_TYPE){.integer = opt_min_value}       ,   \
                                (OPT_DATA_TYPE){.integer = opt_max_value}       ,   \
                                (OPT_DATA_TYPE){.integer = opt_default_value}   ,   \
                                opt_dest_var                                    )   

//////////////////////////////////////////////////////////////////////////////
/// @brief Set integer option definition without boundaries.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
#define SetOptionDefinitionIntNL(   opt_char                ,                   \
                                    opt_long                ,                   \
                                    opt_detail              ,                   \
                                    opt_default_value       ,                   \
                                    opt_dest_var            )                   \
                                                                                \
        SetOptionDefinition(opt_char                                        ,   \
                            opt_long                                        ,   \
                            opt_detail                                      ,   \
                            GET_OPT_TYPE_INT                                ,   \
                            GET_OPT_ARG_REQ_REQUIRED                        ,   \
                            (OPT_DATA_TYPE){.integer = INT_MIN}             ,   \
                            (OPT_DATA_TYPE){.integer = INT_MAX}             ,   \
                            (OPT_DATA_TYPE){.integer = opt_default_value}   ,   \
                            opt_dest_var                                    )

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
#define SetOptionDefinitionChar(opt_char                    ,                   \
                                opt_long                    ,                   \
                                opt_detail                  ,                   \
                                opt_min_value               ,                   \
                                opt_max_value               ,                   \
                                opt_default_value           ,                   \
                                opt_dest_var                )                   \
                                                                                \
        SetOptionDefinition(opt_char                                        ,   \
                            opt_long                                        ,   \
                            opt_detail                                      ,   \
                            GET_OPT_TYPE_CHAR                               ,   \
                            GET_OPT_ARG_REQ_REQUIRED                        ,   \
                            (OPT_DATA_TYPE){.character = opt_min_value}     ,   \
                            (OPT_DATA_TYPE){.character = opt_max_value}     ,   \
                            (OPT_DATA_TYPE){.character = opt_default_value} ,   \
                            opt_dest_var                                    )   


//////////////////////////////////////////////////////////////////////////////
/// @brief Set char option definition without boundaries.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
#define SetOptionDefinitionCharNL(  opt_char                ,                   \
                                    opt_long                ,                   \
                                    opt_detail              ,                   \
                                    opt_default_value       ,                   \
                                    opt_dest_var            )                   \
                                                                                \
        SetOptionDefinition(opt_char                                        ,   \
                            opt_long                                        ,   \
                            opt_detail                                      ,   \
                            GET_OPT_TYPE_CHAR                               ,   \
                            GET_OPT_ARG_REQ_REQUIRED                        ,   \
                            (OPT_DATA_TYPE){.character = CHAR_MIN}          ,   \
                            (OPT_DATA_TYPE){.character = CHAR_MAX}          ,   \
                            (OPT_DATA_TYPE){.character = opt_default_value} ,   \
                            opt_dest_var                                    )

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
#define SetOptionDefinitionFloat(   opt_char                                ,   \
                                    opt_long                                ,   \
                                    opt_detail                              ,   \
                                    opt_min_value                           ,   \
                                    opt_max_value                           ,   \
                                    opt_default_value                       ,   \
                                    opt_dest_var                            )   \
                                                                                \
        SetOptionDefinition(opt_char                                        ,   \
                            opt_long                                        ,   \
                            opt_detail                                      ,   \
                            GET_OPT_TYPE_FLOAT                              ,   \
                            GET_OPT_ARG_REQ_REQUIRED                        ,   \
                            (OPT_DATA_TYPE){.floating = opt_min_value}      ,   \
                            (OPT_DATA_TYPE){.floating = opt_max_value}      ,   \
                            (OPT_DATA_TYPE){.floating = opt_default_value}  ,   \
                            opt_dest_var                                    )

//////////////////////////////////////////////////////////////////////////////
/// @brief Set float option definition without boundaries.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
#define SetOptionDefinitionFloatNL( opt_char            ,                       \
                                    opt_long            ,                       \
                                    opt_detail          ,                       \
                                    opt_default_value   ,                       \
                                    opt_dest_var        )                       \
                                                                                \
        SetOptionDefinition(opt_char                                        ,   \
                            opt_long                                        ,   \
                            opt_detail                                      ,   \
                            GET_OPT_TYPE_FLOAT                              ,   \
                            GET_OPT_ARG_REQ_REQUIRED                        ,   \
                            (OPT_DATA_TYPE){.floating = FLT_MIN}            ,   \
                            (OPT_DATA_TYPE){.floating = FLT_MAX}            ,   \
                            (OPT_DATA_TYPE){.floating = opt_default_value}  ,   \
                            opt_dest_var                                    )

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
#define SetOptionDefinitionDouble(  opt_char                ,                   \
                                    opt_long                ,                   \
                                    opt_detail              ,                   \
                                    opt_min_value           ,                   \
                                    opt_max_value           ,                   \
                                    opt_default_value       ,                   \
                                    opt_dest_var            )                   \
                                                                                \
        SetOptionDefinition(opt_char                                        ,   \
                            opt_long                                        ,   \
                            opt_detail                                      ,   \
                            GET_OPT_TYPE_DOUBLE                             ,   \
                            GET_OPT_ARG_REQ_REQUIRED                        ,   \
                            (OPT_DATA_TYPE){.doubling = opt_min_value}      ,   \
                            (OPT_DATA_TYPE){.doubling = opt_max_value}      ,   \
                            (OPT_DATA_TYPE){.doubling = opt_default_value}  ,   \
                            opt_dest_var                                    )

//////////////////////////////////////////////////////////////////////////////
/// @brief Set double option definition without boundaries.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
#define SetOptionDefinitionDoubleNL(opt_char                ,                   \
                                    opt_long                ,                   \
                                    opt_detail              ,                   \
                                    opt_default_value       ,                   \
                                    opt_dest_var            )                   \
                                                                                \
        SetOptionDefinition(opt_char                                        ,   \
                            opt_long                                        ,   \
                            opt_detail                                      ,   \
                            GET_OPT_TYPE_DOUBLE                             ,   \
                            GET_OPT_ARG_REQ_REQUIRED                        ,   \
                            (OPT_DATA_TYPE){.doubling = DBL_MIN}            ,   \
                            (OPT_DATA_TYPE){.doubling = DBL_MAX}            ,   \
                            (OPT_DATA_TYPE){.doubling = opt_default_value}  ,   \
                            opt_dest_var                                    )

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
#define SetOptionDefinitionString(  opt_char                ,                       \
                                    opt_long                ,                       \
                                    opt_detail              ,                       \
                                    opt_min_value           ,                       \
                                    opt_max_value           ,                       \
                                    opt_default_value       ,                       \
                                    opt_dest_var            )                       \
                                                                                    \
        SetOptionDefinition(opt_char                                            ,   \
                            opt_long                                            ,   \
                            opt_detail                                          ,   \
                            GET_OPT_TYPE_CHAR_STRING                            ,   \
                            GET_OPT_ARG_REQ_REQUIRED                            ,   \
                            (OPT_DATA_TYPE){.char_string = opt_min_value}       ,   \
                            (OPT_DATA_TYPE){.char_string = opt_max_value}       ,   \
                            (OPT_DATA_TYPE){.char_string = opt_default_value}   ,   \
                            opt_dest_var                                        )

//////////////////////////////////////////////////////////////////////////////
/// @brief Set double option definition without boundaries.
/// @param opt_char Option character.
/// @param opt_long Option string.
/// @param opt_detail Option details.
/// @param opt_default_value Option default value.
/// @param opt_dest_var Address to the variable meant to be set after parsing.
/// @return < 0 if any error happened, 0 otherwise.
//////////////////////////////////////////////////////////////////////////////
#define SetOptionDefinitionStringNL(opt_char            ,           \
                                    opt_long            ,           \
                                    opt_detail          ,           \
                                    opt_default_value   ,           \
                                    opt_dest_var        )           \
                                                                    \
        SetOptionDefinitionStringNoLimits(  opt_char            ,   \
                                            opt_long            ,   \
                                            opt_detail          ,   \
                                            opt_default_value   ,   \
                                            opt_dest_var        )   

///////////////////////////////////////////////////////////////////////////
/// @brief Set multiple option definition taking a struct array as input. 
/// @param pub_opt_def Head of the array which includes option definitions.
/// @param pub_opt_def_size Array size.
/// @return < 0 if any error happened, 0 otherwise.
///////////////////////////////////////////////////////////////////////////
C_GET_OPTIONS_API int GetOptDefFromStruct(PUB_OPT_DEFINITION* pub_opt_def, int pub_opt_def_size);

////////////////////////////////////////////////////////
/// @brief Parses given options and arguments if needed.
/// @param argc Argument count.
/// @param argv Argument character string array.
/// @return < 0 if any error happened, 0 otherwise.
////////////////////////////////////////////////////////
C_GET_OPTIONS_API int ParseOptions(int argc, char** argv);

/*************************************/

#ifdef __cplusplus
}
#endif

#endif
