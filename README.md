# C_Arg_Parse: a friendly tool to parse CLI input parameters 🔀
A lightweight and user-friendly library designed to simplify the parsing of command line parameters in your applications.


## Table of contents 🗂️
* [**Introduction** 📑](#introduction)
* [**Features** 🌟](#features)
* [**Prerequisites** 🧱](#prerequisites)
* [**Installation instructions** 📓](#installation-instructions)
  * [**Download and compile** ⚙️](#download-and-compile)
  * [**Compile and run test** 🧪](#compile-and-run-test)
* [**Usage** 🖱️](#usage)
* [**To do** ☑️](#to-do)
* [**Related documents** 🗄️](#related-documents)


## Introduction <a id="introduction"></a> 📑 
This library was created with the purpose of providing an easier way to parse command-line options. By default, command-line options are parsed in order, it's to say, they are treated as if they were positional
input parameters. However, they do not provide any way to determine whether the values are found within an appropriate range, nor any way to establish a default value for them.

**C_Arg_Parse** is meant to sort out those problems by providing an API that takes a description of each option and parses them, displaying a detailed description.

## Features <a id="features"></a> 🌟
* Setting associated character, long name, and details to the target option
* Setting minimum, maximum and default values for the target option
* Error handling
* Parsing and displaying the variables on command line

## Prerequisites <a id="prerequisites"></a> 🧱
By now, the application has only been tested in POSIX-compliant Linux distros. In these, many of the dependencies below may already come installed in the OS.
In the following list, the minimum versions required (if any) by the library are listed.

| Dependency                   | Purpose                                 | Minimum version |
| :--------------------------- | :-------------------------------------- |:-------------: |
| [gcc][gcc-link]              | Compile                                 |11.4            |
| [Bash][bash-link]            | Execute Bash/Shell scripts              |4.4             |
| [Make][make-link]            | Execute make file                       |4.1             |
| [Git][git-link]              | Download GitHub dependencies            |2.34.1          |
| [Xmlstarlet][xmlstarlet-link]| Parse [configuration file](config.xml)  |1.6.1           |

[gcc-link]:        https://gcc.gnu.org/
[bash-link]:       https://www.gnu.org/software/bash/
[make-link]:       https://www.gnu.org/software/make/
[git-link]:        https://git-scm.com/
[xmlstarlet-link]: https://xmlstar.sourceforge.net/

Except for Make, Bash and OpenSSL, the latest version of each of the remaining dependencies will be installed automatically if they have not been found beforehand. 

In any case, installing **_Xmlstarlet_** before executing any of the commands below is strongly recommended. Otherwise, it can lead to error since make file
contains some calls to it at the top. If that happens, just repeat the process (Xmlstarlet would have been already installed).

On top of the ones listed above, there are some *JMS* dependencies (libraries that were also made by myself) that are required for both the library and the test executable to be built,
(although these are managed by the library itself, so no need to download them manually). The required version for each of them is specified by the [config.xml](config.xml) file.

| Dependency                                                              | Purpose                                  |
| :---------------------------------------------------------------------- | :--------------------------------------- |
| [C_Common_shell_files](https://github.com/JonMS95/C_Common_shell_files) | Process [configuration file](config.xml) |
| [C_Severity_Log](https://github.com/JonMS95/C_Severity_Log)             | Show logs                                |


## Installation instructions <a id="installation-instructions"></a> 📓
### Download and compile <a id="download-and-compile"></a> ⚙️
1. In order to download the repo, just clone it from GitHub to your choice path by using the [link](https://github.com/JonMS95/C_Arg_Parse) to the project.

```bash
cd /path/to/repos
git clone https://github.com/JonMS95/C_Arg_Parse
```

**Note**: by default, the path to the repository should be found within a directory (found in Desktop) called "scripts" (it's to say, **_~/Desktop/scripts_**). In order to modify this, change the paths specified in [config.xml](config.xml). If dependencies are not found in the specified path, the path itself would be created, then dependencies will be compiled there. 

2. Then navigate to the directory in which the repo has been downloaded, and set execution permissions to every file just in case they have not been sent beforehand.

```bash
cd /path/to/repos/C_Arg_Parse

find . -type f -exec chmod u+x {} +
```

3. For the library to be built (i.e., clean, download dependencies and compile), just type the following:

```bash
make
```

The result of the line above will be a new API directory (which will match the used version). Within it, a *.h* and a *.so* file will be found.
- **/path/to/repos/C_Arg_Parse/API**
  - **vM_m**
    - **Dynamic_libraries**
      - **_libGetOptions.so.M.m_**
    - **Header_files**
      - **_GetOptions_api.h_**

Where **_M_** and **_m_** stand for the major and minor version numbers.
**_GetOptions.h_** could also be found in **_/path/to/repos/C_Arg_Parse/Source_files/GetOptions_api.h_** although it may differ depending on the version.


### Compile and run test <a id="compile-and-run-test"></a> 🧪
For the test executable file to be compiled and executed, use:

```bash
make test
```

Again, the one below is the path to the generated executable file:
- **/path/to/repos/C_Arg_Parse/Tests**
  - **Executable_files**
      - **_main_**
  - Source_files
  - Dependency_files


## Usage <a id="usage"></a> 🖱️
The following are the most relevant functions found within **_header API file_** (_/path/to/repos/C_Arg_Parse/API/vM_m/Header_files/GetOptions_api.h_) or in the [repo file](https://github.com/JonMS95/C_Arg_Parse/blob/main/Source_files/GetOptions_api.h).
But first of all, _OPT_DATA_TYPE_ struct should be defined:

```c
typedef union C_GET_OPTIONS_API
{
    int     integer;
    char    character;
    float   floating;
    double  doubling;
    char*   char_string;

} OPT_DATA_TYPE;
```

The struct definition above is used to store option's minimum, maximum and default values regardless of its type. In fact, it can be seen in the function definition below:

```c
C_GET_OPTIONS_API int SetOptionDefinition(  char            opt_char            ,
                                            char*           opt_long            ,
                                            char*           opt_detail          ,
                                            int             opt_var_type        ,
                                            int             opt_needs_arg       ,
                                            OPT_DATA_TYPE   opt_min_value       ,
                                            OPT_DATA_TYPE   opt_max_value       ,
                                            OPT_DATA_TYPE   opt_default_value   ,
                                            void*           opt_dest_var        );
```

This function sets an option's definition based on the input parameters, which are described below:
* **opt_char**: option character
* **opt_long**: option string
* **opt_detail**: option descritpion/details
* **opt_var_type**: variable type (i.e. integer, character, ...)
* **opt_needs_arg**: specifies whether the option requires an input argument to be passed
* **opt_min_value**: option's minimum value
* **opt_max_value**: option's maximum value
* **opt_default_value**: option's default value
* **opt_dest_var**: pointer to destination variable, it's to say, the address of the variable in which the parsed value is meant to be stored

It returns **0** if everything went OK, **<0** (proving an error code) if something went wrong.

As the SetOptionDefinition function's definition can be too heavy for the developer to use it, several macros have been defined in the API header file.
SetOptionDefinitionInt, for instance, has been defined as follows:

```c
#define SetOptionDefinitionInt( opt_char            ,                               \
                                opt_long            ,                               \
                                opt_detail          ,                               \
                                opt_min_value       ,                               \
                                opt_max_value       ,                               \
                                opt_default_value   ,                               \
                                opt_dest_var        )                               \
                                                                                    \
        SetOptionDefinition(    opt_char                                        ,   \
                                (char*)opt_long                                 ,   \
                                (char*)opt_detail                               ,   \
                                GET_OPT_TYPE_INT                                ,   \
                                GET_OPT_ARG_REQ_REQUIRED                        ,   \
                                (OPT_DATA_TYPE){.integer = opt_min_value}       ,   \
                                (OPT_DATA_TYPE){.integer = opt_max_value}       ,   \
                                (OPT_DATA_TYPE){.integer = opt_default_value}   ,   \
                                opt_dest_var                                    ) 
```

As seen in the example macro's definition, it's nothing but a wrapper of the **_SetOptionDefinition_** function. The same goes for other macros defined in the API header file.

On top of the ones described before, options can also be defined by using an array of **_PUB_OPT_DEFINITION_** struct as an input parameter for **_GetOptDefFromStruct_**.

```c
C_GET_OPTIONS_API int GetOptDefFromStruct(PUB_OPT_DEFINITION* pub_opt_def, int pub_opt_def_size);
```

Where:
* **pub_opt_def**: an array of **_PUB_OPT_DEFINITION_**
* **pub_opt_def_size**: size of the array (**_pub_opt_def_**)

The **_PUB_OPT_DEFINITION_** struct definition is shown below. It does not need further explanation, as it's just a _struct_ comprising all of the input parameters required by **_SetOptionDefinition_**.

```c
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
```

When it comes to parsing input command-line variables, **_ParseOptions_** function should be used.

```c
C_GET_OPTIONS_API int ParseOptions(int argc, char** argv);
```

Which simply takes the argument count and argument strings array as input parameters (it requires them to be passed to the program's main function).
If any error happens, it's associated error code will be returned (**< 0**).

For reference, a proper API usage example has been provided on the [test source file](https://github.com/JonMS95/C_Arg_Parse/blob/main/Tests/Source_files/main.c).
An example of CLI usage is provided in the [**Shell_files/test.sh**](https://github.com/JonMS95/C_Arg_Parse/tree/main/Shell_files) file.


## To do <a id="to-do"></a> ☑️
- [ ] Add XML file parsing


## Related Documents <a id="related-documents"></a> 🗄️
* [LICENSE](LICENSE)
* [CONTRIBUTING.md](Docs/CONTRIBUTING.md)
* [CHANGELOG.md](Docs/CHANGELOG.md)

