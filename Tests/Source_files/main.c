#include <stdlib.h>
#include "GetOptions_api.h"
#include "SeverityLog_api.h"

#define TEST_FLG_SUCCESS    0
#define TEST_FLG_ERROR      -1

#define TEST_MSG_SUCCEED    "Test %d succeed!\r\n"
#define TEST_MSG_FAILED     "Test %d failed!\r\n"

// Same define statements as in GetOptions.h, so that it is not needed
// to include a file that does not actually belong to the API
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

typedef struct
{
    char            opt_char[16]        ;
    char*           opt_long[14]        ;
    char*           opt_det[3]          ;
    int             opt_var_type[3]     ;
    int             opt_needs_arg[4]    ;
    OPT_DATA_TYPE   opt_min_value[2]    ;
    OPT_DATA_TYPE   opt_max_value[3]    ;
    OPT_DATA_TYPE   opt_default_value[2];
    int*            opt_dest_var[2]     ;

}SetOptionDefinition_Test_Data;

int dummy = 1;

SetOptionDefinition_Test_Data SetOptionDefinition_data =
{
    .opt_char =             {'z', '\0', 'a','b','c','d','e','f','g','h','i','j','k','l','m','n'},
    .opt_long =             {
                            "xxxxxxxxxx",
                            NULL,
                            "xxxxxxxxxxxxxxxxxxxxxxxx",
                            "Option c",
                            "Option e",
                            "Option f",
                            "Option g",
                            "Option h",
                            "Option i",
                            "Option j",
                            "Option k",
                            "Option l",
                            "Option m",
                            "Option n",
                            },
    .opt_det =              {
                                "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                                NULL,
                                "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
                            },
    .opt_var_type =         {0, -1, 5   },
    .opt_needs_arg =        {1, -1, 3, 0},
    .opt_min_value =        {0, 2       },
    .opt_max_value =        {2, 0       },
    .opt_default_value =    {1, 3       },
    .opt_dest_var =         {&dummy, NULL},

};

typedef int SetOptionDefinition_Test_Case[11];
SetOptionDefinition_Test_Case SetOptionDefinition_Test_Indexes[]=
{
//  opt_char    opt_long    opt_det opt_var_type    opt_needs_arg   opt_min_value   opt_max_value   opt_default_value   opt_dest_var    Error code
    {0,         0,          0,      0,              0,              0,              0,              0,                  0,              GET_OPT_SUCCESS                     },
    {0,         0,          0,      0,              0,              0,              0,              0,                  0,              GET_OPT_ERR_OPT_CHAR_ALREADY_EXISTS },
    {1,         0,          0,      0,              0,              0,              0,              0,                  0,              GET_OPT_ERR_NO_OPT_CHAR             },
    {2,         1,          0,      0,              0,              0,              0,              0,                  0,              GET_OPT_ERR_NO_OPT_LONG             },
    {3,         2,          0,      0,              0,              0,              0,              0,                  0,              GET_OPT_ERR_LONG_LENGTH_EXCEEDED    },
    {4,         3,          0,      0,              0,              0,              0,              0,                  0,              GET_OPT_SUCCESS                     },
    {0,         0,          0,      0,              0,              0,              0,              0,                  0,              GET_OPT_SUCCESS                     },
    {5,         0,          1,      0,              0,              0,              0,              0,                  0,              GET_OPT_ERR_OPT_LONG_ALREADY_EXISTS },
    {6,         4,          2,      0,              0,              0,              0,              0,                  0,              GET_OPT_ERR_DETAIL_LENGTH_EXCEEDED  },
    {7,         5,          0,      1,              0,              0,              0,              0,                  0,              GET_OPT_ERR_UNKNOWN_TYPE            },
    {8,         6,          0,      2,              0,              0,              0,              0,                  0,              GET_OPT_ERR_UNKNOWN_TYPE            },
    {9,         7,          0,      0,              1,              0,              0,              0,                  0,              GET_OPT_ERR_UNKNOWN_ARG_REQ         },
    {10,        8,          0,      0,              2,              0,              0,              0,                  0,              GET_OPT_ERR_UNKNOWN_ARG_REQ         },
    {11,        9,          0,      0,              0,              1,              1,              0,                  0,              GET_OPT_ERR_WRONG_BOUNDARIES        },
    {12,        10,         0,      0,              3,              1,              1,              0,                  0,              GET_OPT_SUCCESS                     },
    {13,        11,         0,      0,              0,              0,              0,              1,                  0,              GET_OPT_ERR_VAL_OUT_OF_BOUNDS       },
    {14,        12,         0,      0,              0,              0,              0,              0,                  1,              GET_OPT_ERR_NULL_DEST_VAR           },

};

///////////////////////////////////////////////////////////////
/// @brief Test SetOptionDefinition function. Test cases above.
/// @return < 0 if any test went wrong, 0 otherwise.
///////////////////////////////////////////////////////////////
int Test_SetOptionDefinition()
{
    SeverityLog(SVRTY_LVL_INF, "********** SetOptionDefinition Test Start **********\r\n");

    int number_of_tests = sizeof(SetOptionDefinition_Test_Indexes) / sizeof(SetOptionDefinition_Test_Indexes[0]);
    SeverityLog(SVRTY_LVL_INF, "About to do %d tests.\r\n", number_of_tests);

    int* test_results = (int*)calloc(number_of_tests, sizeof(int));

    int test_overall_result = TEST_FLG_SUCCESS;

    for(int i = 0; i < sizeof(SetOptionDefinition_Test_Indexes) / sizeof(SetOptionDefinition_Test_Indexes[0]); i++)
    {
        SeverityLog(SVRTY_LVL_INF, "Test %d.\r\n", i);
        int set_option_definition = SetOptionDefinition(SetOptionDefinition_data.opt_char[          SetOptionDefinition_Test_Indexes[i][0]] ,
                                                        SetOptionDefinition_data.opt_long[          SetOptionDefinition_Test_Indexes[i][1]] ,
                                                        SetOptionDefinition_data.opt_det[           SetOptionDefinition_Test_Indexes[i][2]] ,
                                                        SetOptionDefinition_data.opt_var_type[      SetOptionDefinition_Test_Indexes[i][3]] ,
                                                        SetOptionDefinition_data.opt_needs_arg[     SetOptionDefinition_Test_Indexes[i][4]] ,
                                                        SetOptionDefinition_data.opt_min_value[     SetOptionDefinition_Test_Indexes[i][5]] ,
                                                        SetOptionDefinition_data.opt_max_value[     SetOptionDefinition_Test_Indexes[i][6]] ,
                                                        SetOptionDefinition_data.opt_default_value[ SetOptionDefinition_Test_Indexes[i][7]] ,
                                                        SetOptionDefinition_data.opt_dest_var[      SetOptionDefinition_Test_Indexes[i][8]] );

        if(set_option_definition != SetOptionDefinition_Test_Indexes[i][9])
        {
            SeverityLog(SVRTY_LVL_ERR, "Test %d failed.\tExpected %d, got %d.\r\n", i, SetOptionDefinition_Test_Indexes[i][9], set_option_definition);
            test_results[i] = TEST_FLG_ERROR;
            if(test_overall_result != TEST_FLG_ERROR)
            {
                test_overall_result = TEST_FLG_ERROR;
            }
        }
    }

    SeverityLog(SVRTY_LVL_INF, "********** SetOptionDefinition test results **********\r\n");
    for(int i = 0; i < number_of_tests; i++)
    {
        if(test_results[i] == TEST_FLG_ERROR)
        {
            SeverityLog(SVRTY_LVL_ERR, TEST_MSG_FAILED, i);
            continue;
        }

        SeverityLog(SVRTY_LVL_INF, TEST_MSG_SUCCEED, i);
    }

    if(test_overall_result < 0)
    {
        SeverityLog(SVRTY_LVL_WNG, "SetOptionDefinition test failed.\r\n");
    }
    else
    {
        SeverityLog(SVRTY_LVL_INF, "SetOptionDefinition test succeed!\r\n");
    }

    SeverityLog(SVRTY_LVL_INF, "********** SetOptionDefinition Test End **********\r\n");

    free(test_results);

    return test_overall_result;
}

///////////////////////////////////////////////////
/// @brief Test ParseOptions function.
/// @param argc Argument count.
/// @param argv Argument vector.
/// @return < 0 if any error happened, 0 otherwise.
///////////////////////////////////////////////////
int TestParseOptions(int argc, char** argv)
{
    SeverityLog(SVRTY_LVL_INF, "********** ParseOptions Test Start **********\r\n");

    float test_1;
    int test_2, test_3;
    char* test_4 = calloc(20, 1);
    char test_5[20] = {};

    SetOptionDefinitionFloat(   'r'                                 ,
                                "TestParse1"                        ,
                                "Test option argument parser FLOAT.",
                                0.2                                 ,
                                4.1                                 ,
                                3.7                                 ,
                                &test_1                             );

    SetOptionDefinition('s'                     ,
                        "TestParse2"            ,
                        NULL                    ,
                        GET_OPT_TYPE_INT        ,
                        GET_OPT_ARG_REQ_OPTIONAL,
                        (OPT_DATA_TYPE)0        ,
                        (OPT_DATA_TYPE)4        ,
                        (OPT_DATA_TYPE)3        ,
                        &test_2                 );

    SetOptionDefinition('t'                                     ,
                        "TestParse3"                            ,
                        "Test option argument parser OPTION."   ,
                        GET_OPT_TYPE_INT                        ,
                        GET_OPT_ARG_REQ_REQUIRED                ,
                        (OPT_DATA_TYPE)0                        ,
                        (OPT_DATA_TYPE)4                        ,
                        (OPT_DATA_TYPE)1                        ,
                        &test_3                                 );

    SetOptionDefinition('c'                                     ,
                        "TestParse4"                            ,
                        "Test character option."                ,
                        GET_OPT_TYPE_CHAR_STRING                ,
                        GET_OPT_ARG_REQ_REQUIRED                ,
                        (OPT_DATA_TYPE)"aaa"                    ,
                        (OPT_DATA_TYPE)"zzz"                    ,
                        (OPT_DATA_TYPE)"bbb"                    ,
                        test_4                                 );

    SetOptionDefinitionStringNL('x'         ,
                                "TestParse5",
                                "String NL" ,
                                "zzzzz"     ,
                                test_5     );

    SetSeverityLogPrintTimeStatus(true);

    int parse_options = ParseOptions(argc, argv);

    SeverityLog(SVRTY_LVL_INF, "test_1 = %2f", test_1);
    SeverityLog(SVRTY_LVL_INF, "test_2 = %d", test_2);
    SeverityLog(SVRTY_LVL_INF, "test_3 = %d", test_3);
    SeverityLog(SVRTY_LVL_INF, "test_4 = %s", test_4);
    SeverityLog(SVRTY_LVL_INF, "test_5 = %s", test_5);

    if(parse_options < 0)
    {
        SeverityLog(SVRTY_LVL_WNG, "ParseOptions test failed.\r\n");
    }
    else
    {
        SeverityLog(SVRTY_LVL_INF, "ParseOptions test succeed!\r\n");
    }

    SeverityLog(SVRTY_LVL_INF, "********** ParseOptions Test End **********\r\n");

    return (parse_options < 0) ? parse_options : GET_OPT_SUCCESS;
}

int main(int argc, char **argv)
{
    Test_SetOptionDefinition();

    TestParseOptions(argc, argv);

    return 0;
}
