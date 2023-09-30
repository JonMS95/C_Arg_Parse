src_get_opt			= Source_files/GetOptions.c
src_main			= Tests/Source_files/main.c

obj_main	= Tests/Object_files/main.o

so_opt		= Dynamic_libraries/libGetOptions.so
exe_main	= Tests/Executable_files/main

shell_dirs		= Common_shell_files/directories.sh
shell_sym_links	= Common_shell_files/sym_links.sh
shell_test		= Shell_files/test.sh

p_deps	= config/Dependencies/Header_files
P_deps	= Dependency_files/Header_files
s_deps	= config/Dependencies/Dynamic_libraries
S_deps	= Dependency_files/Dynamic_libraries

p_test_deps	= config/Tests/Dependency_files/Header_files
P_test_deps	= Tests/Dependency_files/Header_files
s_test_deps	= config/Tests/Dependency_files/Dynamic_libraries
S_test_deps	= Tests/Dependency_files/Dynamic_libraries

p_api	= config/API/Header_files
P_api	= API/Header_files
s_api	= config/API/Dynamic_libraries
S_api	= API/Dynamic_libraries

# Retrieve the XML selection
SH_FILES_PATH := $(shell xmlstarlet sel -t -v "//$$(xmlstarlet el -a "config.xml" | grep "config/Common_shell_files.*@.*PathToShellFiles")" "config.xml")

all: exe test

exe: clean ln_sh_files directories deps opt.so api msg

test: test_deps test_main.o test_main test_rm_obj test_exe

ln_sh_files:
	ln -sf $(SH_FILES_PATH) Common_shell_files

directories:
	@./$(shell_dirs)

clean:
	rm -rf Common_shell_files API/Dynamic_libraries/* API/Header_files/*

clean_rm:
	rm -rf API Dependency_files Dynamic_libraries Tests/Dependency_files Tests/Object_files Tests/Executable_files

deps:
	@bash $(shell_sym_links) -p $(p_deps) -P $(P_deps) -s $(s_deps) -S $(S_deps)

opt.so: $(src_get_opt)
	gcc -Wall -g -fPIC -shared $(src_get_opt) -o $(so_opt)

api:
	@bash $(shell_sym_links) -p $(p_api) -P $(P_api) -s $(s_api) -S $(S_api)

msg:
	@echo "**************************************************************************************************"
	@echo "Don't forget to set the path:"
	@echo "export LD_LIBRARY_PATH=~/Desktop/scripts/C/C_Arg_Parse/Dynamic_libraries"
	@echo "**************************************************************************************************"

test_deps:
	@bash $(shell_sym_links) -p $(p_test_deps) -P $(P_test_deps) -s $(s_test_deps) -S $(S_test_deps)

test_main.o:
	gcc -Wall -g -c $(src_main) -o $(obj_main)

test_main:
	gcc -g -Wall $(obj_main) -LTests/Dependency_files/Dynamic_libraries -lGetOptions -lSeverityLog -o $(exe_main)

test_rm_obj:
	rm -rf Tests/Object_files

test_exe:
	@./$(shell_test)
