
set(PATH_INC)
set(PATH_SRC)
set(PATH_WORKSPACE  C:/1_job/0_NPP_IT_ELMA/code_base_workspace_esp/ti_mcu_code_base)

list(APPEND PATH_INC ${PATH_WORKSPACE}"/bsp_esp32/uart")
list(APPEND PATH_SRC ${PATH_WORKSPACE}"/bsp_esp32/uart/uart_drv.c")

list(APPEND PATH_INC ${PATH_WORKSPACE}"/projects/t_beam_v1_1_example/main")
list(APPEND PATH_INC ${PATH_WORKSPACE}"/boards/tBeamV1_1")
list(APPEND PATH_INC ${PATH_WORKSPACE}"/utils")
list(APPEND PATH_INC ${PATH_WORKSPACE}"/utils/fifo")
list(APPEND PATH_INC ${PATH_WORKSPACE}"/common")
idf_component_register(SRCS "hello_world_main.c" ${PATH_SRC}
                       INCLUDE_DIRS ${PATH_INC})