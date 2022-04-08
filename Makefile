compile:
	g++ src/main.c -Wall -Wextra -o bin/program

mem:
	g++ src/main.c -o bin/test
	valgrind --tool=memcheck --leak-check=yes ./bin/test data/example-4.5.ini uncomfortable-belated-decent-hospitable-percentage.wan-gullible-ashamed-outstanding-easy-going-reality -s
