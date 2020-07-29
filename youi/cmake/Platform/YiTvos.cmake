# =============================================================================
# © You i Labs Inc. 2000-2019. All rights reserved.
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

if(__yi_custom_platform_included)
    return()
endif()
set(__yi_custom_platform_included 1)

include(${YouiEngine_DIR}/cmake/Platform/YiTvos.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Platform/YiConfigureFramework2.cmake)

macro(yi_configure_platform)
    cmake_parse_arguments(_ARGS "" "PROJECT_TARGET" "" ${ARGN})

    if(NOT _ARGS_PROJECT_TARGET)
        message(FATAL_ERROR "'yi_configure_platform' requires the PROJECT_TARGET argument be set")
    endif()

    _yi_configure_platform(PROJECT_TARGET ${_ARGS_PROJECT_TARGET})

    include(Modules/apple/YiConfigureFramework)
    yi_configure_framework(TARGET ${_ARGS_PROJECT_TARGET}
        FRAMEWORK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${YI_PLATFORM_LOWER}/Frameworks/NewRelicAgentTVOS.framework"
    )

    # Upload dSYMs to New NewRelic
    if(YI_UPLOAD_DSYM)
      add_custom_command(TARGET ${_ARGS_PROJECT_TARGET}
          POST_BUILD
          COMMAND /usr/bin/zip --recurse-paths --quiet "${CMAKE_CURRENT_SOURCE_DIR}/tvOS_dSYM.zip" "${YI_BASE_OUTPUT_DIRECTORY}.dSYM"
          COMMAND curl -F dsym=@"${CMAKE_CURRENT_SOURCE_DIR}/tvOS_dSYM.zip" -H "X-APP-LICENSE-KEY: AA839df3238dbfabfa2325033f3eee665ef3837b98" https://mobile-symbol-upload.newrelic.com/symbol
      )
    endif()

    include(Modules/apple/YiFindFrameworkHelper)
    yi_find_framework_helper(FRAMEWORK "SystemConfiguration")
    yi_find_framework_helper(FRAMEWORK "libc++.tbd")
    yi_find_framework_helper(FRAMEWORK "libz.tbd")
    yi_find_framework_helper(FRAMEWORK "libsqlite3.tbd")
    yi_find_framework_helper(FRAMEWORK "AVKit")

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        "-w"
        "-framework CoreLocation"
        "-framework VideoSubscriberAccount"
        "-framework AVKit"
    )

   target_link_libraries(${PROJECT_NAME} PRIVATE -lsqlite3)

   set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE[variant=Release] "YES")
endmacro(yi_configure_platform)
