# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/LenY/dev/uni/sem_2/nagyhz_2/build/_deps/sfml-src"
  "C:/Users/LenY/dev/uni/sem_2/nagyhz_2/build/_deps/sfml-build"
  "C:/Users/LenY/dev/uni/sem_2/nagyhz_2/build/_deps/sfml-subbuild/sfml-populate-prefix"
  "C:/Users/LenY/dev/uni/sem_2/nagyhz_2/build/_deps/sfml-subbuild/sfml-populate-prefix/tmp"
  "C:/Users/LenY/dev/uni/sem_2/nagyhz_2/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
  "C:/Users/LenY/dev/uni/sem_2/nagyhz_2/build/_deps/sfml-subbuild/sfml-populate-prefix/src"
  "C:/Users/LenY/dev/uni/sem_2/nagyhz_2/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/LenY/dev/uni/sem_2/nagyhz_2/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/LenY/dev/uni/sem_2/nagyhz_2/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
