# =============================================================================
# © You i Labs Inc. 2000-2020. All rights reserved.
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

if(__yi_configure_framework_included2)
    return()
endif()
set(__yi_configure_framework_included2 1)

# =============================================================================
# This function is modified function yi_configure_framework that is located in You.i SDK
# at following location: you.i/[version]/cmake/Modules/apple/YiConfigureFramework.cmake (Original documentation)
# Reason why I modified it, that original function doesn't allow to specify build flavors (release / debug)
# during call to target_link_libraries, that makes it impossible for adding different version of same framework (e.g release vs debug)
# for CppClientLib framework (and any other custom in-house frameworks) debug and release version must be included in xcode project without
# this modification it will duplicate same path for both configurations. CMAKE_BUILD_TYPE can't be used for that since it's being ignored by multi
# configuration generators (XCODE, VS)
function(yi_configure_framework2)
    set(_REQUIRED_ARGS TARGET FRAMEWORK_PATH)

    cmake_parse_arguments(ARGS
        "EMBEDDED"
        "${_REQUIRED_ARGS};PATH_RELEASE;CODE_SIGN_IDENTITY"
        ""
        ${ARGV}
    )

    foreach(_ARG IN LISTS _REQUIRED_ARGS)
        if(NOT ARGS_${_ARG})
            message(FATAL_ERROR "Missing required argument: ${_ARG}")
        endif()
    endforeach()

    if(ARGS_UNUSED_ARGUMENTS)
        message(FATAL_ERROR "Encountered unused arguments: ${ARGS_UNUSED_ARGUMENTS}")
    endif()


    # target_include_directories was removed as not needed... Framework path will be added via 
    # target_link_libraries and that is enough to locate headers ...

    # In order to use same framework but in different paths we need to make this 
    # a single invokation, otherwise calling target_link_libraries twice will add two paths on each configuration ...

    target_link_libraries(${ARGS_TARGET} PRIVATE debug ${ARGS_FRAMEWORK_PATH} optimized ${ARGS_PATH_RELEASE})
    get_filename_component(_FRAMEWORK_NAME ${ARGS_FRAMEWORK_PATH} NAME)

    if(NOT ${ARGS_EMBEDDED})
        message(STATUS "Added non-Embedded Framework: ${_FRAMEWORK_NAME}")
    else()
        message(STATUS "Added Embedded Framework: ${_FRAMEWORK_NAME}")
        if(NOT ARGS_CODE_SIGN_IDENTITY OR "x${ARGS_CODE_SIGN_IDENTITY}" STREQUAL "x")
            message(FATAL_ERROR "The Framework '${ARGS_FRAMEWORK_PATH}' was marked as an EMBEDDED Framework. To code sign the Framework, the CODE_SIGN_IDENTITY argument needs to be given the code signing identity to use.")
        endif()

        # Sanitize un-escaped parenthesis from ARGS_CODE_SIGN_IDENTITY
        # Example:    iPhone Developer (XXXXX) -> iPhone Developer \(XXXXX\)
        # Example 2:  iPhone Developer \(XXXXX\) -> iPhone Developer \(XXXXX\)
        # Example 3:  iPhone Developer(XXXXX) -> iPhone Developer\(XXXXX\)
        string(REGEX REPLACE "([^\\\\])\\\(" "\\1\\\\(" CODE_SIGN_IDENTITY_CORRECTED ${ARGS_CODE_SIGN_IDENTITY})
        string(REGEX REPLACE "([^\\\\])\\\)" "\\1\\\\)" CODE_SIGN_IDENTITY_CORRECTED ${CODE_SIGN_IDENTITY_CORRECTED})

        # Embedded frameworks need to be copied to the regular build folder and code signed when creating regular builds.
        # When creating archives, however, the embedded frameworks need to be copied into Xcode's '${INSTALL_DIR}' path and
        # code signed from that location.
        
        foreach(_DESTINATION_FRAMEWORK_FILEPATH "$<TARGET_FILE_DIR:${ARGS_TARGET}>")

            # Here I need to copy right library (debug | release) to destination folder 
            # otherwise only one that was selected during generation will be copied (always)
            # YI_BUILD_TYPE and CMAKE_BUILD_TYPE can't be used since these variables are being set during generation (to single variant)
            # We need somehow detect build type during each build, so the way I found is to use file path that is containing proper build type
            # Debug-iphoneos, Release-iphoneos or tvos ...

            # since COMMAND directive doesn't support conditional logic 
            # I used bash in order to find string using regex and then execute actual command of copy

            add_custom_command(TARGET ${ARGS_TARGET} PRE_BUILD

                COMMAND /bin/sh -c "if [[ '${_DESTINATION_FRAMEWORK_FILEPATH}' == *'Debug'* ]]\; then ${CMAKE_COMMAND} -E copy_directory ${ARGS_FRAMEWORK_PATH} ${_DESTINATION_FRAMEWORK_FILEPATH}/Frameworks/${_FRAMEWORK_NAME}\; fi\;"
                COMMAND /bin/sh -c "if [[ '${_DESTINATION_FRAMEWORK_FILEPATH}' == *'Release'* ]]\; then ${CMAKE_COMMAND} -E copy_directory ${ARGS_PATH_RELEASE} ${_DESTINATION_FRAMEWORK_FILEPATH}/Frameworks/${_FRAMEWORK_NAME}\; fi\;"
            )

            add_custom_command(TARGET ${ARGS_TARGET} PRE_BUILD
                COMMAND codesign
                    -s "${CODE_SIGN_IDENTITY_CORRECTED}"
                    -fv
                    "${_DESTINATION_FRAMEWORK_FILEPATH}/Frameworks/${_FRAMEWORK_NAME}"
            )
            
        endforeach()
    endif()
endfunction(yi_configure_framework2)
