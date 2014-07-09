cc=clang++
all: container-any

run:
	./a.out --apples 2 --oranges=40

container-any:
	 $(cc) -g -Wall -c container-any.cpp
	 $(cc) -g -Wall -o a.out container-any.o -L/usr/local/lib -lboost_program_options

program_options:
	 $(cc) -g -Wall -c program_options.cpp
	 $(cc) -g -Wall -o a.out program_options.o -L/usr/local/lib -lboost_program_options

test:
	$(cc) -g -Wall test.cpp

clean:
	rm *.o a.out
