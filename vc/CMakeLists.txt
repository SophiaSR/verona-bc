add_executable(vc
  passes/anf.cc
  passes/application.cc
  passes/flatten.cc
  passes/operators.cc
  passes/parser.cc
  passes/sugar.cc
  passes/structure.cc
  lang.cc
  main.cc
)

if(MSVC)
  target_compile_options(vbcc PRIVATE /W4 /WX /wd5030 /bigobj)
else()
  target_compile_options(vbcc PRIVATE
    -Wall -Wextra -Werror -Wshadow -Wno-zero-length-array)
endif()

target_link_libraries(vc
  vbc::vbcc
  CLI11::CLI11
)

# Copy the stdlib to the install directory
install(DIRECTORY std DESTINATION vc)

install(TARGETS vc RUNTIME DESTINATION vc)
