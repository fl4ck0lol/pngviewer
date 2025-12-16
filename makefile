default:
	gcc -o main main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
run:
	gcc -o main main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	./main $(IMG)