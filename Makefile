.PHONY: all clean test

CC = gcc -Wall -Werror -Wextra -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage
SOURSES = 3d-viewer/parser.c
OBJECT = *.o
LINUXFLAGS = -lsubunit -lrt -lpthread -lm
LIBS= -lcheck
OS := $(shell uname -s)

all: install open

install:
	mkdir build
	cd 3d-viewer && qmake && make && make clean && rm Makefile
	mv 3d-viewer/3d-viewer.app build
	rm 3d-viewer/.qmake.stash

open:
	cd build/3d-viewer.app/Contents/MacOS && ./3d-viewer

uninstall:
	-rm -rf build

dvi:
	open readme.md

dist:
	-rm -rf Archive_3dViewer_v1.0/
	mkdir Archive_3dViewer_v1.0/
	mkdir Archive_3dViewer_v1.0/src
	mv ./build/3d-viewer.app Archive_3dViewer_v1.0/src/
	tar cvzf Archive_3dViewer_v1.0.tgz Archive_3dViewer_v1.0/
	rm -rf Archive_3dViewer_v1.0/
	rm -rf build

parcer.o:
	gcc -g -Wall -Wextra -Werror -std=c11 -c 3d-viewer/parser.c

tests: clean parcer.o
	gcc -g -Wall -Wextra -Werror -std=c11 parser.o tests.c -lcheck -lm -lpthread -o test
	./test

gcov_report: parcer.o
	@gcc $(GCOVFLAGS) tests.c $(LIBS) $(SOURSES) -o s21_gcov_report
	@./s21_gcov_report
	@lcov -t "s21_gcov_report" -o s21_gcov_report.info -c -d .
	@genhtml -o report s21_gcov_report.info
	@open ./report/index-sort-f.html
	@rm -rf ./*.gcno ./*.gcda ./s21_gcov_report.*

clean:
	@-rm -rf *.o
	@-rm -rf test
	@-rm -rf  *.dSYM
	@-rm -rf s21_gcov_report
	@-rm -rf *.gcda
	@-rm -rf *.gcno
	@-rm -rf report
	@-rm -rf Archive_3dViewer_v1.0.tgz
	@-rm -rf build

check_style:
	clang-format -n */*.c */*.h */*.cpp
