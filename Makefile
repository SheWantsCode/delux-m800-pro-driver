TARGET = delux_driver

$(TARGET) : ./obj/main.o ./obj/driver.o
	gcc ./obj/main.o ./obj/driver.o -o $(TARGET) -lusb-1.0

./obj/main.o : ./src/main.c
	gcc -c ./src/main.c -o ./obj/main.o

./obj/driver.o : ./src/driver.c
	gcc -c ./src/driver.c -o ./obj/driver.o
