
if(NOT "D:/Github/StonksBot/build/cpr-download/cpr-download-prefix/src/cpr-download-stamp/cpr-download-gitinfo.txt" IS_NEWER_THAN "D:/Github/StonksBot/build/cpr-download/cpr-download-prefix/src/cpr-download-stamp/cpr-download-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: 'D:/Github/StonksBot/build/cpr-download/cpr-download-prefix/src/cpr-download-stamp/cpr-download-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "D:/Github/StonksBot/external/sleepy-discord/deps/cpr"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: 'D:/Github/StonksBot/external/sleepy-discord/deps/cpr'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "D:/Git/cmd/git.exe"  clone --no-checkout --config "advice.detachedHead=false" "https://github.com/whoshuu/cpr.git" "cpr"
    WORKING_DIRECTORY "D:/Github/StonksBot/external/sleepy-discord/deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/whoshuu/cpr.git'")
endif()

execute_process(
  COMMAND "D:/Git/cmd/git.exe"  checkout e627e47 --
  WORKING_DIRECTORY "D:/Github/StonksBot/external/sleepy-discord/deps/cpr"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'e627e47'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "D:/Git/cmd/git.exe"  submodule update --recursive --init 
    WORKING_DIRECTORY "D:/Github/StonksBot/external/sleepy-discord/deps/cpr"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: 'D:/Github/StonksBot/external/sleepy-discord/deps/cpr'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "D:/Github/StonksBot/build/cpr-download/cpr-download-prefix/src/cpr-download-stamp/cpr-download-gitinfo.txt"
    "D:/Github/StonksBot/build/cpr-download/cpr-download-prefix/src/cpr-download-stamp/cpr-download-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: 'D:/Github/StonksBot/build/cpr-download/cpr-download-prefix/src/cpr-download-stamp/cpr-download-gitclone-lastrun.txt'")
endif()

