# Makefile
CC=gcc
CFLAGS=
OBJS=warrior_game.o potion.o signal_handler.o game_state.o
ATTACK_OBJS=attack.o game_state.o
all: hw_game.out attack.out

hw_game.out: $(OBJS)
	gcc -o hw_game.out $(OBJS)

attack.out: $(ATTACK_OBJS)
	$(CC) -o attack.out $(ATTACK_OBJS)

warrior_game.o: warrior_game.c game_state.h signal_handler.h
	$(CC) $(CFLAGS) -c warrior_game.c
attack.o: attack.c game_state.h
	$(CC) $(CFLAGS) -c attack.c
potion.o: potion.c
	$(CC) $(CFLAGS) -c potion.c
signal_handler.o: signal_handler.c signal_handler.h
	$(CC) $(CFLAGS) -c signal_handler.c
game_state.o: game_state.c game_state.h
	$(CC) $(CFLAGS) -c game_state.c

clean:
	rm -f $(OBJS) $(ATTACK_OBJS) hw_game.out attack.out *.o *.txt core
