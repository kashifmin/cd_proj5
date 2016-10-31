all:
	cc -o main parse.c
main:
	parse.c tokens.c
clean:
	rm -rf main
