#cifuzz:build-template:begin
#add_fuzz_test({{ .FuzzTestName }} {{ .FileName }})
#target_link_libraries({{ .FuzzTestName }} PRIVATE redwg)
#cifuzz:build-template:end
add_fuzz_test(fuzz_dwg_read_file fuzz_dwg_read_file.cpp)
target_link_libraries(fuzz_dwg_read_file PRIVATE redwg)
add_fuzz_test(fuzz_dwg_write_file fuzz_dwg_write_file.cpp)
target_link_libraries(fuzz_dwg_write_file PRIVATE redwg)
