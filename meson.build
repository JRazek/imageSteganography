project(
    'image_steganography', 'c', 'cpp',
    default_options : ['c_std=c18', 'cpp_std=c++2a']
)

cpp = meson.get_compiler('cpp')

incdir = include_directories(
    'include',
    'src',
)

executable(
    'main',
    'src/main.cpp',
    include_directories : incdir,
    dependencies: []
)