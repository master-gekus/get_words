cmake_minimum_required(VERSION 3.5)

project(get_words LANGUAGES CXX)

add_executable(${PROJECT_NAME}
  main.cpp
  )

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  )

configure_file(words.txt words.txt COPYONLY)
