main:	main.c	shell.c
	gcc -o main main.c shell.c -I.

clean: 
	rm main