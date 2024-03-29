# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/nulzo/Github/Cringe-Bot/cmake/dpp_external_project-prefix/src/dpp_external_project"
  "/Users/nulzo/Github/Cringe-Bot/cmake/dpp_external_project-prefix/src/dpp_external_project-build"
  "/Users/nulzo/Github/Cringe-Bot/cmake/dpp_external_project-prefix"
  "/Users/nulzo/Github/Cringe-Bot/cmake/dpp_external_project-prefix/tmp"
  "/Users/nulzo/Github/Cringe-Bot/cmake/dpp_external_project-prefix/src/dpp_external_project-stamp"
  "/Users/nulzo/Github/Cringe-Bot/cmake/dpp_external_project-prefix/src"
  "/Users/nulzo/Github/Cringe-Bot/cmake/dpp_external_project-prefix/src/dpp_external_project-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/nulzo/Github/Cringe-Bot/cmake/dpp_external_project-prefix/src/dpp_external_project-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/nulzo/Github/Cringe-Bot/cmake/dpp_external_project-prefix/src/dpp_external_project-stamp${cfgdir}") # cfgdir has leading slash
endif()
