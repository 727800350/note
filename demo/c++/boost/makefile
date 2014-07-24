cc=clang++
all: array

run:
	./a.out --apples 2 --oranges=40
array:
	$(cc) -g -Wall array.cpp

optional:
	$(cc) -g -Wall optional.cpp

db-wrapper_any:
	$(cc) -g -Wall db-wrapper_any.cpp

db-wrapper_variant:
	$(cc) -g -Wall db-wrapper_variant.cpp

variant:
	$(cc) -g -Wall variant.cpp

container-any:
	 $(cc) -g -Wall -c container-any.cpp
	 $(cc) -g -Wall -o a.out container-any.o -L/usr/local/lib -lboost_program_options

program_options:
	 $(cc) -g -Wall -c program_options.cpp
	 $(cc) -g -Wall -o a.out program_options.o -L/usr/local/lib -lboost_program_options

test:
	$(cc) -g -Wall test.cpp

clean:
	rm a.out
