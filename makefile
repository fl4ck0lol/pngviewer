default:
	gcc -o main main.c 
ray:
	gcc -o main main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11