set(LUA_CORE
    ${EXTERNAL_DIR}/lua/lapi.c
    ${EXTERNAL_DIR}/lua/lcode.c
    ${EXTERNAL_DIR}/lua/lctype.c
    ${EXTERNAL_DIR}/lua/ldebug.c
    ${EXTERNAL_DIR}/lua/ldo.c
    ${EXTERNAL_DIR}/lua/ldump.c
    ${EXTERNAL_DIR}/lua/lfunc.c
    ${EXTERNAL_DIR}/lua/lgc.c
    ${EXTERNAL_DIR}/lua/llex.c
    ${EXTERNAL_DIR}/lua/lmem.c
    ${EXTERNAL_DIR}/lua/lobject.c
    ${EXTERNAL_DIR}/lua/lopcodes.c
    ${EXTERNAL_DIR}/lua/lparser.c
    ${EXTERNAL_DIR}/lua/lstate.c
    ${EXTERNAL_DIR}/lua/lstring.c
    ${EXTERNAL_DIR}/lua/ltable.c
    ${EXTERNAL_DIR}/lua/ltm.c
    ${EXTERNAL_DIR}/lua/lundump.c
    ${EXTERNAL_DIR}/lua/lvm.c
    ${EXTERNAL_DIR}/lua/lzio.c
)

set(LUA_LIB
    ${EXTERNAL_DIR}/lua/lauxlib.c
    ${EXTERNAL_DIR}/lua/lbaselib.c
    ${EXTERNAL_DIR}/lua/lcorolib.c
    ${EXTERNAL_DIR}/lua/ldblib.c
    ${EXTERNAL_DIR}/lua/liolib.c
    ${EXTERNAL_DIR}/lua/lmathlib.c
    ${EXTERNAL_DIR}/lua/loadlib.c
    ${EXTERNAL_DIR}/lua/loslib.c
    ${EXTERNAL_DIR}/lua/lstrlib.c
    ${EXTERNAL_DIR}/lua/ltablib.c
    ${EXTERNAL_DIR}/lua/lutf8lib.c
    ${EXTERNAL_DIR}/lua/linit.c
)

add_subdirectory(${EXTERNAL_DIR}/sol2)
add_library(lua STATIC ${LUA_CORE} ${LUA_LIB})

target_include_directories(lua PUBLIC ${EXTERNAL_DIR}/lua)
target_link_libraries(lua PUBLIC sol2::sol2)
