cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(GTASA.elf C CXX)
include("${VITASDK}/share/vita.cmake" REQUIRED)
set(VITA_APP_NAME "GTA: SA")
set(VITA_TITLEID  "GTASA0000")
set(VITA_VERSION  "02.20")
set(VITA_MKSFOEX_FLAGS "-d ATTRIBUTE2=12")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wl,-q,--wrap,memcpy,--wrap,memmove,--wrap,memset -D_GNU_SOURCE -Wall -O3 -mfloat-abi=softfp")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -Wno-write-strings")

add_executable(GTASA.elf
  loader/main.c
  loader/config.c
  loader/dialog.c
  loader/fios.c
  loader/so_util.c
  loader/jni_patch.c
  loader/mpg123_patch.c
  loader/openal_patch.c
  loader/opengl_patch.c
  loader/gfx_patch.c
  loader/scripts_patch.c
)

add_dependencies(GTASA.elf companion.bin)

target_link_libraries(GTASA.elf
  -Wl,--whole-archive pthread -Wl,--no-whole-archive
  stdc++
  openal
  vitaGL
  vitashark
  SceShaccCgExt
  m
  mathneon
  mpg123
  taihen_stub
  kubridge_stub
  SceAppMgr_stub
  SceAppUtil_stub
  SceAudio_stub
  SceAudioIn_stub
  SceCtrl_stub
  SceCommonDialog_stub
  SceDisplay_stub
  SceKernelDmacMgr_stub
  SceFios2_stub
  SceGxm_stub
  SceLibcBridge_stub
  SceShaccCg_stub
  SceSysmodule_stub
  ScePower_stub
  SceTouch_stub
  SceVshBridge_stub
)

add_executable(companion
  companion/main.cpp
)

target_link_libraries(companion
  imgui
  vitaGL
  vitashark
  SceShaccCgExt
  m
  mathneon
  taihen_stub
  SceAppMgr_stub
  SceAppUtil_stub
  SceAudio_stub
  SceCtrl_stub
  SceCommonDialog_stub
  SceDisplay_stub
  SceKernelDmacMgr_stub
  SceGxm_stub
  SceShaccCg_stub
  SceSysmodule_stub
  SceTouch_stub
  SceVshBridge_stub
)

vita_create_self(companion.bin companion UNSAFE)

vita_create_self(eboot.bin GTASA.elf UNSAFE)
vita_create_vpk(GTASA.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE ${CMAKE_SOURCE_DIR}/sce_sys/icon0.png sce_sys/icon0.png
       ${CMAKE_SOURCE_DIR}/sce_sys/pic0.png sce_sys/pic0.png  
       ${CMAKE_SOURCE_DIR}/sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
       ${CMAKE_SOURCE_DIR}/sce_sys/livearea/contents/config.png sce_sys/livearea/contents/config.png
       ${CMAKE_SOURCE_DIR}/sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
       ${CMAKE_SOURCE_DIR}/sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/001.png sce_sys/manual/001.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/002.png sce_sys/manual/002.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/003.png sce_sys/manual/003.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/004.png sce_sys/manual/004.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/005.png sce_sys/manual/005.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/006.png sce_sys/manual/006.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/007.png sce_sys/manual/007.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/008.png sce_sys/manual/008.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/009.png sce_sys/manual/009.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/010.png sce_sys/manual/010.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/011.png sce_sys/manual/011.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/012.png sce_sys/manual/012.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/013.png sce_sys/manual/013.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/014.png sce_sys/manual/014.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/015.png sce_sys/manual/015.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/016.png sce_sys/manual/016.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/017.png sce_sys/manual/017.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/018.png sce_sys/manual/018.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/019.png sce_sys/manual/019.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/020.png sce_sys/manual/020.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/021.png sce_sys/manual/021.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/022.png sce_sys/manual/022.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/023.png sce_sys/manual/023.png
	   ${CMAKE_SOURCE_DIR}/sce_sys/manual/024.png sce_sys/manual/024.png
	   ${CMAKE_SOURCE_DIR}/companion/configuration_app.png configuration_app.png
       ${CMAKE_BINARY_DIR}/companion.bin companion.bin
)

add_custom_target(copy
  COMMAND cp eboot.bin D:/app/${VITA_TITLEID}/eboot.bin
  COMMAND cp companion.bin D:/app/${VITA_TITLEID}/companion.bin
  DEPENDS eboot.bin
  DEPENDS companion.bin
)
