#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "../../Source_files/GetOptions_api.h"
#include "../../Source_files/GetOptions.h"
#include "../../Dependency_files/Header_files/SeverityLog_api.h"

#define TEST_FLG_SUCCESS    0
#define TEST_FLG_ERROR      -1

#define TEST_MSG_SUCCEED    "Test %d succeed!\r\n"
#define TEST_MSG_FAILED     "Test %d failed!\r\n"

/* Flag set by ‘--verbose’. */
static int verbose_flag;

void ParseDemo(int argc, char** argv)
{
      int c;

  while (1)
    {
      static struct option long_options[] =
        {
          /* These options set a flag. */
          {"verbose", no_argument,       &verbose_flag, 1},
          {"brief",   no_argument,       &verbose_flag, 0},
          /* These options don’t set a flag.
             We distinguish them by their indices. */
          {"add",     no_argument,       0, 'a'},
          {"append",  no_argument,       0, 'b'},
          {"delete",  required_argument, 0, 'd'},
          {"create",  required_argument, 0, 'c'},
          {"file",    required_argument, 0, 'f'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "abc:d:f:",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
        {
        case 0:
          /* If this option set a flag, do nothing else now. */
          if (long_options[option_index].flag != 0)
            break;
          printf ("option %s", long_options[option_index].name);
          if (optarg)
            printf (" with arg %s", optarg);
          printf ("\n");
          break;

        case 'a':
          puts ("option -a\n");
          break;

        case 'b':
          puts ("option -b\n");
          break;

        case 'c':
          printf ("option -c with value `%s'\n", optarg);
          break;

        case 'd':
          printf ("option -d with value `%s'\n", optarg);
          break;

        case 'f':
          printf ("option -f with value `%s'\n", optarg);
          break;

        case '?':
          /* getopt_long already printed an error message. */
          break;

        default:
          abort ();
        }
    }
}

typedef struct
{
    char            opt_char[16]        ;
    char*           opt_long[14]       ;
    char*           opt_det[3]        ;
    int             opt_var_type[3]     ;
    int             opt_needs_arg[4]    ;
    OPT_DATA_TYPE   opt_min_value[2]    ;
    OPT_DATA_TYPE   opt_max_value[3]    ;
    OPT_DATA_TYPE   opt_default_value[2];
    int*            opt_dest_var[2]    ;

}SetOptionDefinition_Test_Data;

int dummy = 1;

SetOptionDefinition_Test_Data SetOptionDefinition_data =
{
    .opt_char =             {'z', '\0', 'a','b','c','d','e','f','g','h','i','j','k','l','m','n'},
    .opt_long =             {
                            "xxxxxxxxxx",
                            NULL,
                            "xxxxxxxxxxx",
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
    .opt_var_type =         {0, -1, 4   },
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

int Test_SetOptionDefinition()
{
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

    free(test_results);

    return test_overall_result;
}

int TestParseOptions(int argc, char** argv)
{
    int test_1, test_2, test_3;

    SetOptionDefinition('r',
                        "TestParse1",
                        "Test option argument parser BOOL.",
                        GET_OPT_TYPE_INT,
                        GET_OPT_ARG_REQ_REQUIRED,
                        (OPT_DATA_TYPE)0,
                        (OPT_DATA_TYPE)4,
                        (OPT_DATA_TYPE)3,
                        &test_1);

    SetOptionDefinition('s',
                        "TestParse2",
                        "Test option argument parser OPTIONAL OPTION.",
                        GET_OPT_TYPE_INT,
                        GET_OPT_ARG_REQ_REQUIRED,
                        (OPT_DATA_TYPE)0,
                        (OPT_DATA_TYPE)4,
                        (OPT_DATA_TYPE)3,
                        &test_2);

    SetOptionDefinition('t',
                        "TestParse3",
                        "Test option argument parser OPTION.",
                        GET_OPT_TYPE_INT,
                        GET_OPT_ARG_REQ_REQUIRED,
                        (OPT_DATA_TYPE)0,
                        (OPT_DATA_TYPE)4,
                        (OPT_DATA_TYPE)3,
                        &test_3);

    int parse_options = ParseOptions(argc, argv);

    if(parse_options < 0)
    {
        return parse_options;
    }

    return GET_OPT_SUCCESS;
}

int main(int argc, char **argv)
{
    if(Test_SetOptionDefinition() < 0)
    {
        SeverityLog(SVRTY_LVL_WNG, "SetOptionDefinition test failed.\r\n");
    }
    else
    {
        SeverityLog(SVRTY_LVL_INF, "SetOptionDefinition test succeed!\r\n");
    }

    if(TestParseOptions(argc, argv) < 0)
    {
        SeverityLog(SVRTY_LVL_WNG, "ParseOptions test failed.\r\n");
    }
    else
    {
        SeverityLog(SVRTY_LVL_INF, "ParseOptions test succeed!\r\n");
    }

    return 0;
}
