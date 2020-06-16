macro(mahi_util_example target)
    # create executable
    add_executable(${target} "ex_${target}.cpp")
    # set dependencies
    target_link_libraries(${target} mahi::util)
    # add install rule
    install(TARGETS ${target}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    set_target_properties(${target} PROPERTIES FOLDER "Examples")
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)
endmacro(mahi_util_example)

mahi_util_example(print)
mahi_util_example(csv)
mahi_util_example(time)
mahi_util_example(log)
mahi_util_example(ring_buffer)
mahi_util_example(options)
mahi_util_example(concurrency)
mahi_util_example(spsc)
mahi_util_example(json)
mahi_util_example(filter)
mahi_util_example(math)
mahi_util_example(stats)
mahi_util_example(ctrl_c_handling)
mahi_util_example(type_erasure)
mahi_util_example(keyboard)