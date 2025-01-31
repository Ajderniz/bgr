/* PROJECT SOLO #0201 / MUGWUMP */

#include <ctype.h> /* toupper() */
#include <math.h> /* sqrt() */
#include <stdio.h>
#include <stdlib.h> /* srand(), rand() */
#include <string.h> /* strcmp(), strchr() */
#include <time.h> /* time() */

#define NUM_PLAYERS 4
#define MAX_TURNS 10
#define GRID_SIZE 10
#define ANSWER_LENGTH 4

typedef enum {
	FALSE = 0,
	TRUE = 1
} Bool;

typedef struct {
	int x;
	int y;
} Vector2;

/* 'player' is a structure of the form (x, y, found?) */
typedef struct {
	Vector2 pos;
	Bool found;
} Player;

void
ask_user(const char *question, char *answer)
{
	int i;
	int c;

	printf("%s", question);
	fgets(answer, ANSWER_LENGTH, stdin);
	answer[ANSWER_LENGTH - 1] = '\0';
	for (i = 0; i < ANSWER_LENGTH; i++) {
		answer[i] = toupper(answer[i]);
	}
	if (NULL == strchr(answer, '\n')) {
		while ((c = getchar()) != '\n' && c != EOF)
			;
	}
}

int
main(void)
{
	int i;
	/* 'players' is a set of four 'player's */
	Player players[NUM_PLAYERS];
	int current_turn;
	Bool all_players_found;
	char answer[ANSWER_LENGTH];
	Vector2 guess;
	int x_diff, y_diff;
	float distance;

	srand(time(NULL));

	puts("THIS IS A GAME OF HIDE AND SEEK.\n");

	printf("THE OBJECT OF THE GAME IS TO FIND THE %d PLAYERS\n",
			NUM_PLAYERS);
	printf("WHO ARE HIDDEN ON A %d BY %d GRID.\n", GRID_SIZE, GRID_SIZE);

	puts("HOMEBASE WILL BE THE POSITION AT (0,0) AND ANY GUESS");
	puts("YOU MAKE SHOULD CONTAIN TWO NUMBERS. THE FIRST GIVES");
	puts("THE UNIT DISTANCE RIGHT OF THE HOMEBASE AND THE SECOND");
	puts("IS THE UNIT DISTANCE ABOVE HOMEBASE.\n");

	printf("YOU WILL HAVE %d ATTEMPTS TO LOCATE THESE PLAYERS\n",
			MAX_TURNS);
	puts("AND WILL BE TOLD HOW CLOSE YOUR GUESS IS");
	puts("TO EACH PLAYER.\n");

	printf("IF AFTER %d TRIES YOU ARE UNABLE TO CARRY OUT THIS TASK\n",
			MAX_TURNS);
	puts("YOU MAY CONTINUE TO BE 'IT', BUT THE PLAYERS WILL");
	puts("BE PERMITTED TO MOVE TO NEW LOCATIONS.\n");

	ask_user("\nARE YOU READY TO BEGIN?", answer);
	if (0 != strcmp(answer, "YES")) {
		return 0;
	}
	/* START */
	while (TRUE) {
		/* fill 'players' with random positions (x, y) */
		for (i = 0; i < NUM_PLAYERS; i++) {
			players[i].pos.x = rand() % GRID_SIZE;
			players[i].pos.y = rand() % GRID_SIZE;
			players[i].found = FALSE;
		}
		/* while current turn is under GRID_SIZE */
		/* AND not all players have been found: */
		current_turn = 1;
		do {
			/* ask user for a 'guess' in the form (x, y) */
			printf("\n\nTURN NUMBER %d, ", current_turn);
			ask_user("WHAT IS YOUR GUESS?", answer);
			if (FALSE == isdigit(answer[0]) ||
					answer[1] != ',' ||
					FALSE == isdigit(answer[2])) {
				puts("THE FORMAT IS (X,Y). TRY AGAIN.");
				continue;
			}
			guess.x = answer[0] - '0';
			guess.y = answer[2] - '0';
			for (i = 0; i < NUM_PLAYERS; i++) {
				/* if the 'guess' matches a 'player': */
				if ((guess.x == players[i].pos.x &&
					guess.y == players[i].pos.y) ||
					TRUE == players[i].found) {
					/* mark this 'player' as found */
					printf("YOU HAVE FOUND PLAYER %d\n",
							i + 1);
					players[i].found = TRUE;
					continue;
				}
				/* say how far away from the 'guess' this */
				/* 'player' is using the pythagorean theroem: */
				/* (a^2 + b^2 = c^2) */
				x_diff = (players[i].pos.x > guess.x) ? 
					players[i].pos.x - guess.x :
					guess.x - players[i].pos.x;
				y_diff = (players[i].pos.y > guess.y) ? 
					players[i].pos.y - guess.y :
					guess.y - players[i].pos.y;
				distance = sqrt((x_diff * x_diff) +
						(y_diff * y_diff));
				printf("YOUR DISTANCE FROM PLAYER %d IS %2.1f "
					"UNIT(S).\n", i + 1, distance);
			}
			current_turn++;
			all_players_found = TRUE;
			for (i = 0; i < NUM_PLAYERS; i++) {
				if (FALSE == players[i].found) {
					all_players_found = FALSE;
					break;
				}
			}
			if (FALSE == all_players_found) {
				continue;
			}
			break;
		} while (current_turn <= MAX_TURNS);
		/* outside of the main loop */
		/* if not all 'player's in 'players' were found: */
		if (FALSE == all_players_found) {
			/*say "fail" */
			/* ask user if they want to know the location of the */
			/* 'players' */
			printf("\nYOU DIDN'T FIND ALL THE PLAYERS IN %d "
					"TRIES.\n",
					MAX_TURNS);
			puts("DO YOU WANT TO KNOW WHERE THE PLAYERS YOU "
					"DIDN'T");
			ask_user("FIND WERE HIDDEN?", answer);
			/* if yes */
			/* say the x and y values of each 'player' */
			if (0 == strcmp(answer, "YES")) {
				for (i = 0; i < NUM_PLAYERS; i++) {
					if (TRUE == players[i].found) {
						printf("YOU FOUND PLAYER %d.\n",
								i + 1);
						continue;
					}
					printf("PLAYER %d HID AT: (%d,%d).\n",
							i + 1,
							players[i].pos.x,
							players[i].pos.y);
				}
			}
		} else {
			printf("\nYOU HAVE FOUND ALL THE PLAYERS IN %d "
					"TURNS.\n",
					current_turn);
		}
		/* ask user if they want to replay */
		ask_user("\nDO YOU WANT TO PLAY AGAIN?", answer);
		/* if yes, go back to START */
		if (strcmp(answer, "YES") != 0) {
			puts("THEN PLEASE LOGOUT.");
			break;
		}
	}
	/* END */
	return 0;
}
