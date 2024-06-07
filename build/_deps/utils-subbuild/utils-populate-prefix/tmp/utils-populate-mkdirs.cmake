# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/couldlie/vscode_projs/tla2024/build/_deps/utils-src"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/utils-build"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/utils-subbuild/utils-populate-prefix"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/utils-subbuild/utils-populate-prefix/tmp"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/utils-subbuild/utils-populate-prefix/src/utils-populate-stamp"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/utils-subbuild/utils-populate-prefix/src"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/utils-subbuild/utils-populate-prefix/src/utils-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/couldlie/vscode_projs/tla2024/build/_deps/utils-subbuild/utils-populate-prefix/src/utils-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/couldlie/vscode_projs/tla2024/build/_deps/utils-subbuild/utils-populate-prefix/src/utils-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
