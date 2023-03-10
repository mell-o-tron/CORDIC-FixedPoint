all: float fixed

float: ./source/floating_point.c ./source/atan.c
	gcc ./source/floating_point.c ./source/atan.c -o "float" -lm

fixed: ./source/fixed_point.c
	gcc ./source/fixed_point.c -o "fixed"
