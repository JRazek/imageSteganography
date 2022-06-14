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
    build_args += ['-DNDEBUG']
else
	build_args += ['-g3','-fsanitize=undefined,float-cast-overflow,float-divide-by-zero,bounds-strict,address,', '-Wall']
	link_args += build_args 
endif

range_v3 = subproject('range-v3')

range_v3 = range_v3.get_variable('range_v3')


exec=executable(
    'main',
    'src/main.cpp',
    'src/bitmap.cpp',
    'src/image_formatters.cpp',
    include_directories : incdir,
    cpp_args: build_args,
	link_args: link_args,
    dependencies: [range_v3]
)

