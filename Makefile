main.exe : main.c loader.c
	gcc main.c loader.c -o main.exe