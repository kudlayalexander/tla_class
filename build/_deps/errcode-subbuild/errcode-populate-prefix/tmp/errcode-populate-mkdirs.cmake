# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/couldlie/vscode_projs/tla2024/build/_deps/errcode-src"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/errcode-build"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/errcode-subbuild/errcode-populate-prefix"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/errcode-subbuild/errcode-populate-prefix/tmp"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/errcode-subbuild/errcode-populate-prefix/src/errcode-populate-stamp"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/errcode-subbuild/errcode-populate-prefix/src"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/errcode-subbuild/errcode-populate-prefix/src/errcode-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/couldlie/vscode_projs/tla2024/build/_deps/errcode-subbuild/errcode-populate-prefix/src/errcode-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/couldlie/vscode_projs/tla2024/build/_deps/errcode-subbuild/errcode-populate-prefix/src/errcode-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
