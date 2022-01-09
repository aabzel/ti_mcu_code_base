
set(PATH_INC)
set(PATH_SRC)
set(PATH_WORKSPACE  C:/1_job/0_NPP_IT_ELMA/code_base_workspace_esp/ti_mcu_code_base)


list(APPEND PATH_INC ${PATH_WORKSPACE}"/utils/io_utils")
list(APPEND PATH_SRC ${PATH_WORKSPACE}"/utils/debug_info.c")
list(APPEND PATH_SRC ${PATH_WORKSPACE}"/common/common_functions.c")
list(APPEND PATH_SRC ${PATH_WORKSPACE}"/Drivers/cli_drv/table_utils.c")
list(APPEND PATH_SRC ${PATH_WORKSPACE}"/utils/fifo/fifo_char.c")
list(APPEND PATH_SRC ${PATH_WORKSPACE}"/bsp_esp32/sys.c")
list(APPEND PATH_SRC ${PATH_WORKSPACE}"/bsp_esp32/uart/uart_commands.c")
list(APPEND PATH_SRC ${PATH_WORKSPACE}"/utils/io_utils/io_utils.c")
list(APPEND PATH_SRC ${PATH_WORKSPACE}"/utils/io_utils/log.c")
list(APPEND PATH_SRC ${PATH_WORKSPACE}"/utils/io_utils/uart_string_reader.c")
#target_compile_definitions(${COMPONENT_TARGET} PRIVATE "-DHAS_CLI")
#add_compile_definitions(HAS_CLI)

list(APPEND PATH_SRC ${PATH_WORKSPACE}"/common/uart_common.c")

list(APPEND PATH_SRC ${PATH_WORKSPACE}"/utils/utils_string/str_utils.c")
list(APPEND PATH_INC ${PATH_WORKSPACE}"/utils/utils_string")

list(APPEND PATH_SRC ${PATH_WORKSPACE}"/Drivers/cli_drv/cli_manager.c")
list(APPEND PATH_INC ${PATH_WORKSPACE}"/Drivers/cli_drv")

list(APPEND PATH_INC ${PATH_WORKSPACE}"/bsp_esp32")
list(APPEND PATH_INC ${PATH_WORKSPACE}"/bsp_esp32/uart")
list(APPEND PATH_SRC ${PATH_WORKSPACE}"/bsp_esp32/uart/uart_drv.c")


list(APPEND PATH_INC ${PATH_WORKSPACE}"/projects/t_beam_v1_1_example/main")
list(APPEND PATH_INC ${PATH_WORKSPACE}"/boards/tBeamV1_1")
list(APPEND PATH_INC ${PATH_WORKSPACE}"/utils")
list(APPEND PATH_INC ${PATH_WORKSPACE}"/utils/fifo")
list(APPEND PATH_INC ${PATH_WORKSPACE}"/common")
#target_compile_definitions(${COMPONENT_TARGET} PUBLIC "-DHAS_CLI")
idf_component_register(SRCS "hello_world_main.c" ${PATH_SRC}
                       INCLUDE_DIRS ${PATH_INC})

target_compile_options(${COMPONENT_TARGET} PUBLIC -DHAS_MCU -DHAS_CLI -DSUPER_LOOP)