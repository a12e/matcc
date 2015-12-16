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

# 41_float_declaration
get_filename_component(source 4_float/41_float_declaration.mcc ABSOLUTE)
add_test(NAME 41_float_declaration COMMAND ${TEST} ${source})

# 42_float_assignation
get_filename_component(source 4_float/42_float_assignation.mcc ABSOLUTE)
add_test(NAME 42_float_assignation COMMAND ${TEST} ${source})

# 43_float_arithmetic
get_filename_component(source 4_float/43_float_arithmetic.mcc ABSOLUTE)
add_test(NAME 43_float_arithmetic COMMAND ${TEST} ${source})

################################################################################

# 61_if
get_filename_component(source 6_control/61_if.mcc ABSOLUTE)
add_test(NAME 61_if COMMAND ${TEST} ${source})

# 62_if_else
get_filename_component(source 6_control/62_if_else.mcc ABSOLUTE)
add_test(NAME 62_if_else COMMAND ${TEST} ${source})

# 63_for
get_filename_component(source 6_control/63_for.mcc ABSOLUTE)
add_test(NAME 63_for COMMAND ${TEST} ${source})

# 64_while
get_filename_component(source 6_control/64_while.mcc ABSOLUTE)
add_test(NAME 64_while COMMAND ${TEST} ${source})
