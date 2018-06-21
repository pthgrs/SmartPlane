plant.out :  main.o  sensor.o motor.o server.o
	gcc -lpthread -lwiringPi -lbluetooth $^ -o $@
main.o : main.c smartPlant.h
	gcc -c -o main.o main.c
sensor.o :sensor.c sensor.h
	gcc -c -o sensor.o sensor.c

motor.o :motor.c motor.h
	gcc -c -o motor.o motor.c
server.o : server.c server.h
	gcc -c -o server.o server.c
clean :
	rm *.o
	rm *.out

