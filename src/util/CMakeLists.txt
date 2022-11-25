SET(DESTINATION ${PROJECT_SOURCE_DIR})
aux_source_directory(. SRCLIST)

add_library(util SHARED ${SRCLIST})
