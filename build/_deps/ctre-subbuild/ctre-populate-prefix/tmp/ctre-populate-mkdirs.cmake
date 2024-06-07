# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/couldlie/vscode_projs/tla2024/build/_deps/ctre-src"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/ctre-build"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/ctre-subbuild/ctre-populate-prefix"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/ctre-subbuild/ctre-populate-prefix/tmp"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/ctre-subbuild/ctre-populate-prefix/src/ctre-populate-stamp"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/ctre-subbuild/ctre-populate-prefix/src"
  "/home/couldlie/vscode_projs/tla2024/build/_deps/ctre-subbuild/ctre-populate-prefix/src/ctre-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/couldlie/vscode_projs/tla2024/build/_deps/ctre-subbuild/ctre-populate-prefix/src/ctre-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/couldlie/vscode_projs/tla2024/build/_deps/ctre-subbuild/ctre-populate-prefix/src/ctre-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
