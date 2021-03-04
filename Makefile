all: queue_cond queue_zmq queue_sem

queue_cond:
	-rm queue_conditional.bin
	gcc -g -Wall main_q_cond.c utils.c -o queue_conditional.bin -lpthread

queue_zmq:
	-rm queue_zmq.bin
	gcc -g -Wall main_q_zmq.c utils.c -o queue_zmq.bin -lpthread -lzmq

queue_sem:
	-rm queue_semaphore.bin
	gcc -g -Wall main_q_sema.c utils.c sem.c -o queue_semaphore.bin -lpthread
	
format:
	@echo "Requires clang-format > 9.0"
	@clang-format -i -style=file *.c *.h

clean:
	-rm *.bin
	
time:
	@echo "\n----- SEMAPHORE -----"
	/usr/bin/time -p ./queue_semaphore.bin
	
	@echo "\n----- ZMQ -----"
	/usr/bin/time -p ./queue_zmq.bin
	
	@echo "\n----- MUTEX COND -----"
	/usr/bin/time -p ./queue_conditional.bin

	