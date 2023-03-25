# makefile规则
# target: dependends...
# 	command(command前面必须有一个tab键)
cacu:add_int.o add_float.o sub_int.o sub_float.o main.o
	gcc -o cacu add/add_int.o add/add_float.o sub/sub_float.o sub/sub_int.o main.o

add_int.o: add/add_int.c add/add.h
	gcc -c -o add/add_int.o add/add_int.c

add_float.o: add/add_float.c add/add.h
	gcc -c -o add/add_float.o add/add_float.c

sub_int.o: sub/sub_int.c sub/sub.h
	gcc -c -o sub/sub_int.o sub/sub_int.c

sub_float.o: sub/sub_float.c sub/sub.h
	gcc -c -o sub/sub_float.o sub/sub_float.c

clean:  #使用make clean命令即可执行下面的命令
	rm -f cacu add/add_int.o add/add_float.o sub/sub_int.o sub/sub_float.o main.o

