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
  build_args += ['-g3', '-Wall']

  if cpp.get_id() == 'gcc'
    build_args += ['-fsanitize=undefined,float-cast-overflow,float-divide-by-zero,bounds-strict,address']
  endif

  link_args += build_args 
endif


exec=executable(
    'image_steganography',
    'src/main.cpp',
    'src/header_utils.cpp',
    'src/utils.cpp',
    'src/ui.cpp',
    include_directories : incdir,
    cpp_args: build_args,
	link_args: link_args,
    dependencies: []
)

