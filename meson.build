project(
    'image_steganography', 'c', 'cpp',
    version: '0.1.0',
    default_options : ['c_std=c18', 'cpp_std=c++2a']
)

cpp = meson.get_compiler('cpp')

incdir = include_directories(
    'include',
    'src',
)

build_args=[]
link_args=[]


if get_option('build')=='release'
    build_args += ['-O3', '-Wall', '-DNDEBUG', '-g0', '-s']
else
	build_args += ['-g3','-fsanitize=undefined,float-cast-overflow,float-divide-by-zero,bounds-strict,address,', '-Wall']
	link_args += build_args 
endif

range_v3 = subproject('range-v3')

range_v3 = range_v3.get_variable('range_v3')


exec=executable(
    'image_steganography',
    'src/main.cpp',
    'src/header_utils.cpp',
    'src/utils.cpp',
    'src/ui.cpp',
    include_directories : incdir,
    cpp_args: build_args,
	link_args: link_args,
    dependencies: [range_v3]
)

