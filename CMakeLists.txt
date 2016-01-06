cmake_minimum_required(VERSION 2.4)

set(project_name WaaCaa)
project(${project_name})

include_directories(.)

aux_source_directory(entry src_entry)
aux_source_directory(model src_model)
aux_source_directory(network src_network)
aux_source_directory(rendering src_rendering)
aux_source_directory(settings src_settings)
aux_source_directory(utils src_utils)
aux_source_directory(window src_window)

add_definitions(-DUNICODE -D_UNICODE)


add_executable(${project_name} WIN32 ${src_entry} ${src_model} ${src_network}
 ${src_rendering} ${src_settings} ${src_utils} ${src_window} resources/Resource.rc)

