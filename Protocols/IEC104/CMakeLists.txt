set(SUBDIR_NAME IEC104)

aux_source_directory(. SRC_HEAD_FILES)

add_library(${SUBDIR_NAME}
${SRC_HEAD_FILES}
)