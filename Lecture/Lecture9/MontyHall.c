#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main ()
{
    // Where is the grand Prize?
    srand(time(NULL));
    int prize = rand() % 3; // 0, 1, or 2
    int notprize1 = (prize + 1) % 3; // A door that does not have the prize
    int notprize2 = (prize + 2) % 3; // Another door that does not have the prize
    int doors[3] = {0, 1, 2};

    //Ask contestant to pick a door
    printf("\n");
    printf(" =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= \n");
    printf(" ** Monty Hall Simulator ** \n");
    printf(" =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= \n");
    printf("\n");
    int pick=-1;
    while (pick < 0 || pick > 2) {
        printf("Pick a door (A, B, or C): ");
        char input;
        scanf(" %c", &input);
        if (input == 'A') pick = 0;
        else if (input == 'B') pick = 1;
        else if (input == 'C') pick = 2;
        else printf("Invalid input. Please enter A, B, or C.\n");
    }

    printf("\n You picked door %c.\n", 'A'+pick);

    //Tell the contestant which door Monty opens (not the prize and not the pick)
    int other;
    int other_other;
    printf( "\n Interesting Choice . . .\n");
    if (pick == prize) {
        int ss = rand() % 2; // 0 or 1
        if (ss == 0) {
            other = notprize1;
            other_other = notprize2;
        } else {
            other = notprize2;
            other_other = notprize1;
        }
    }
    else {
        other_other = prize;
        if (pick == notprize1) {
            other = notprize2;
        } else {
            other = notprize1;
        }
    }

    printf("\n I can tell you for sure that the prize is not behind door: %c\n", 'A'+other);

    //Ask if the contestant wants to switch to the other unopened door
    int change=-1;
    char NewPick;
    while (change < 0 || change > 1) {
        printf("\n Stay with Door %c or switch to Door %c. Input Either %c or %c: ", 'A'+pick, 'A'+other_other, 'A'+pick, 'A'+other_other);
        scanf(" %c", &NewPick); // Note the space before %c to skip whitespace
        if (NewPick == 'A'+pick) change = 0;
        else if (NewPick == 'A'+other_other) change = 1;
        else printf("Invalid input. Please enter the remaining door %c or %c.\n", 'A'+pick, 'A'+other_other);
    }

    int final_pick;
    if (change == 0) {
        final_pick = pick;
        printf("\n You stayed with Door %c.\n", 'A'+final_pick);
    } else {
        final_pick = other_other;
        printf("\n You switched to Door %c.\n", 'A'+final_pick);
    }

    //Check answer
    if (final_pick == prize) {
        printf("\n *** WINNER! ***\n");
    } else {
        printf("\n --- LOSER! ---\n \n");
    }
    printf("The prize was behind door %c.\n", 'A'+prize);
    return 0;
}