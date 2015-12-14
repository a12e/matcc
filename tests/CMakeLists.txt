get_filename_component(testsh test.sh ABSOLUTE)
set(TEST ${testsh} $<TARGET_FILE:matcc>)

################################################################################

# 11_print_usage
add_test(NAME 11_print_usage COMMAND $<TARGET_FILE:matcc> -h /dev/null)

# 12_main_func
get_filename_component(source 1_invocation/12_main_func.mcc ABSOLUTE)
add_test(NAME 12_main_func COMMAND ${TEST} ${source})

# 13_comments
get_filename_component(source 1_invocation/13_comments.mcc ABSOLUTE)
add_test(NAME 13_comments COMMAND ${TEST} ${source})

################################################################################

# 21_print_string
get_filename_component(source 2_print/21_print_string.mcc ABSOLUTE)
add_test(NAME 21_print_string COMMAND ${TEST} ${source})

# 22_print_int
get_filename_component(source 2_print/22_print_int.mcc ABSOLUTE)
add_test(NAME 22_print_int COMMAND ${TEST} ${source})

# 23_print_float
get_filename_component(source 2_print/23_print_float.mcc ABSOLUTE)
add_test(NAME 23_print_float COMMAND ${TEST} ${source})

# 24_print_matrix
get_filename_component(source 2_print/24_print_matrix.mcc ABSOLUTE)
add_test(NAME 24_print_matrix COMMAND ${TEST} ${source})

################################################################################

# 31_integer_declaration
get_filename_component(source 3_integer/31_integer_declaration.mcc ABSOLUTE)
add_test(NAME 31_integer_declaration COMMAND ${TEST} ${source})

# 32_integer_assignation
get_filename_component(source 3_integer/32_integer_assignation.mcc ABSOLUTE)
add_test(NAME 32_integer_assignation COMMAND ${TEST} ${source})

# 33_integer_arithmetic
get_filename_component(source 3_integer/33_integer_arithmetic.mcc ABSOLUTE)
add_test(NAME 33_integer_arithmetic COMMAND ${TEST} ${source})

################################################################################

