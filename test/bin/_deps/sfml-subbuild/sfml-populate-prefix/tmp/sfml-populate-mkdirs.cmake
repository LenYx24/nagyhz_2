# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/leny/dev/uni/sem_2/nagyhz_2/test/bin/_deps/sfml-src"
  "/home/leny/dev/uni/sem_2/nagyhz_2/test/bin/_deps/sfml-build"
  "/home/leny/dev/uni/sem_2/nagyhz_2/test/bin/_deps/sfml-subbuild/sfml-populate-prefix"
  "/home/leny/dev/uni/sem_2/nagyhz_2/test/bin/_deps/sfml-subbuild/sfml-populate-prefix/tmp"
  "/home/leny/dev/uni/sem_2/nagyhz_2/test/bin/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
  "/home/leny/dev/uni/sem_2/nagyhz_2/test/bin/_deps/sfml-subbuild/sfml-populate-prefix/src"
  "/home/leny/dev/uni/sem_2/nagyhz_2/test/bin/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/leny/dev/uni/sem_2/nagyhz_2/test/bin/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/leny/dev/uni/sem_2/nagyhz_2/test/bin/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
