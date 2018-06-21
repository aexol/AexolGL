function(add_whole_archive out in whole_lib)
    set(${out} ${in} -Wl,--whole-archive ${whole_lib} -Wl,--no-whole-archive PARENT_SCOPE)
endfunction()
