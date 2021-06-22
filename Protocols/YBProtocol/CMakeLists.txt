set(SUBDIR_NAME YBProtocol)

aux_source_directory(. SRC_HEAD_FILES)

add_library(${SUBDIR_NAME}
${SRC_HEAD_FILES}
)

