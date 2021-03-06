%module example_libexample
%include <od/glue/mod.cpp.swig>

%{

#undef SWIGLUA

#include <example/AR.h>

#define SWIGLUA

%}

%include <example/AR.h>
