all:
	c++ main.cpp -o parser_sub
install:
	install parser_sub /usr/local/bin
clean:
	rm parser_sub
