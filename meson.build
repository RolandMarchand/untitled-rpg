project('project-name', 'c', version: '0.1', license: 'BSD0')

cmake = import('cmake')

# Raylib
raylib_proj = cmake.subproject('raylib')
raylib_dep = raylib_proj.dependency('raylib')

# Math
cc = meson.get_compiler('c')
math_dep = cc.find_library('m', required : false)

# Define the executable and its sources
executable('main',
           sources: ['src/main.c'],
           dependencies: [raylib_dep, math_dep]
)
