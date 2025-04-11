#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_PLAYERS 11

struct batsman {
    char name[25];
    int runs, balls, ones, twos, threes, fours, sixes, is_out;
    float strike_rate;
} batsmen[MAX_PLAYERS];

struct bowler {
    char name[25];
    int runs_given, wickets_taken, balls_bowled, extras;
    float economy;
} bowlers[MAX_PLAYERS];

int total_extras = 0;
int total_wickets = 0;
int total_runs = 0;
int max_overs;
int num_batsmen, num_bowlers, max_wickets;

void inputMatchDetails() {
    printf("Enter the number of batsmen: ");
    scanf("%d", &num_batsmen);
    max_wickets = num_batsmen - 1;
    
    printf("Enter the number of bowlers: ");
    scanf("%d", &num_bowlers);
    
    printf("Enter the number of overs for the match: ");
    scanf("%d", &max_overs);
}

void inputBatsmanData(int innings, int max_balls) {
    int balls_remaining = max_balls;
    printf("\nInnings %d - Batsmen Details (Max balls: %d)\n", innings, max_balls);
    
    for (int i = 0; i < num_batsmen; i++) {
        printf("Enter name of batsman %d: ", i + 1);
        scanf("%s", batsmen[i].name);

        printf("Enter the number of ones, twos, threes, fours, sixes scored by %s: ", batsmen[i].name);
        scanf("%d %d %d %d %d", &batsmen[i].ones, &batsmen[i].twos, &batsmen[i].threes, 
              &batsmen[i].fours, &batsmen[i].sixes);

        printf("Enter balls played by %s (Max remaining: %d): ", batsmen[i].name, balls_remaining);
        scanf("%d", &batsmen[i].balls);
        
        while (batsmen[i].balls > balls_remaining || batsmen[i].balls < 0) {
            printf("Invalid input! Balls must be between 0 and %d: ", balls_remaining);
            scanf("%d", &batsmen[i].balls);
        }
        balls_remaining -= batsmen[i].balls;

        printf("Did %s get out? (1 for Yes, 0 for No): ", batsmen[i].name);
        scanf("%d", &batsmen[i].is_out);
    }
    
    if (balls_remaining > 0) {
        printf("Warning: %d balls remaining unplayed!\n", balls_remaining);
    }
}

void inputBowlerData(int innings, int batsman_runs) {
    int total_balls = max_overs * 6;
    int balls_remaining = total_balls;
    int runs_remaining = batsman_runs;
    total_extras = 0;
    
    printf("\nInnings %d - Bowlers Details\n", innings);
    printf("Total runs to be distributed among bowlers (excluding extras): %d\n", batsman_runs);
    
    for (int i = 0; i < num_bowlers; i++) {
        printf("Enter name of bowler %d: ", i + 1);
        scanf("%s", bowlers[i].name);

        printf("Enter overs bowled by %s (Max remaining: %d overs, %d balls): ", 
               bowlers[i].name, balls_remaining/6, balls_remaining);
        int overs;
        scanf("%d", &overs);
        bowlers[i].balls_bowled = overs * 6;
        
        while (bowlers[i].balls_bowled > balls_remaining || bowlers[i].balls_bowled < 0) {
            printf("Invalid input! Overs must be between 0 and %d: ", balls_remaining/6);
            scanf("%d", &overs);
            bowlers[i].balls_bowled = overs * 6;
        }
        balls_remaining -= bowlers[i].balls_bowled;
        
        if (i == num_bowlers - 1) {
            bowlers[i].runs_given = runs_remaining;
            printf("Enter wickets taken and extras by %s (Runs given set to %d): ", 
                   bowlers[i].name, bowlers[i].runs_given);
            scanf("%d %d", &bowlers[i].wickets_taken, &bowlers[i].extras);
        } else {
            printf("Enter runs given (Max remaining: %d), wickets taken, extras by %s: ", 
                   runs_remaining, bowlers[i].name);
            scanf("%d %d %d", &bowlers[i].runs_given, &bowlers[i].wickets_taken, &bowlers[i].extras);
            
            while (bowlers[i].runs_given > runs_remaining || bowlers[i].runs_given < 0) {
                printf("Invalid input! Runs must be between 0 and %d: ", runs_remaining);
                scanf("%d", &bowlers[i].runs_given);
            }
            runs_remaining -= bowlers[i].runs_given;
        }
        total_extras += bowlers[i].extras;
    }
    
    if (balls_remaining > 0) {
        printf("Warning: %d balls (%d overs) remaining unbowled!\n", balls_remaining, balls_remaining/6);
    }
    if (runs_remaining > 0 && num_bowlers > 0) {
        printf("Warning: %d runs were not distributed among bowlers!\n", runs_remaining);
    }
}

void calculateStats() {
    total_runs = 0;
    total_wickets = 0;
    
    for (int i = 0; i < num_batsmen; i++) {
        batsmen[i].runs = (batsmen[i].ones * 1) + (batsmen[i].twos * 2) + (batsmen[i].threes * 3) +
                         (batsmen[i].fours * 4) + (batsmen[i].sixes * 6);
        batsmen[i].strike_rate = batsmen[i].balls > 0 ? (batsmen[i].runs * 100.0) / batsmen[i].balls : 0;
        total_runs += batsmen[i].runs;
        if (batsmen[i].is_out) total_wickets++;
    }
    
    for (int i = 0; i < num_bowlers; i++) {
        bowlers[i].economy = bowlers[i].balls_bowled > 0 ? 
                            (float)(bowlers[i].runs_given + bowlers[i].extras) / (bowlers[i].balls_bowled / 6.0) : 0;
    }
}

void displayMatchSummary(int target, int innings) {
    printf("\n======== INNINGS %d SUMMARY ========\n", innings);
    
    printf("\nBatting Scorecard\n");
    printf("Batsman\tRuns\tBalls\tFours\tSixes\tSR\tOut\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < num_batsmen; i++) {
        printf("%-10s %-5d %-5d %-5d %-5d %-10.2f %-8s\n", batsmen[i].name, batsmen[i].runs, batsmen[i].balls,
               batsmen[i].fours, batsmen[i].sixes, batsmen[i].strike_rate, batsmen[i].is_out ? "Yes" : "No");
    }
    
    printf("\nBowling Scorecard\n");
    printf("Bowler\tOvers\tRuns\tWickets\tExtras\tEconomy\n");
    printf("-----------------------------------------\n");
    for (int i = 0; i < num_bowlers; i++) {
        float overs = bowlers[i].balls_bowled / 6.0;
        printf("%-10s %-5.1f %-5d %-5d %-5d %-5.2f\n", bowlers[i].name, overs, 
               bowlers[i].runs_given + bowlers[i].extras, bowlers[i].wickets_taken, 
               bowlers[i].extras, bowlers[i].economy);
    }
    
    printf("\nTotal Runs: %d, Wickets: %d/%d, Extras: %d\n", total_runs + total_extras, 
           total_wickets, max_wickets, total_extras);
    if (innings == 1) {
        printf("Target for Second Innings: %d\n", target);
    }
}

void determineWinner(int runs_team1, int runs_team2) {
    printf("\n======== MATCH RESULT ========\n");
    if (runs_team1 > runs_team2) {
        printf("Team 1 wins by %d runs!\n", runs_team1 - runs_team2);
    } else if (runs_team2 > runs_team1) {
        printf("Team 2 wins by %d wickets!\n", max_wickets - total_wickets);
    } else {
        printf("Match is a tie!\n");
    }
}

int main() {
    inputMatchDetails();
    int max_balls = max_overs * 6;
    
    printf("======== FIRST INNINGS ========\n");
    inputBatsmanData(1, max_balls);
    calculateStats();
    inputBowlerData(1, total_runs);
    displayMatchSummary(total_runs + total_extras + 1, 1);
    int first_innings_runs = total_runs + total_extras;
    
    total_runs = 0;
    total_wickets = 0;
    total_extras = 0;
    
    printf("\n======== SECOND INNINGS ========\n");
    inputBatsmanData(2, max_balls);
    calculateStats();
    inputBowlerData(2, total_runs);
    displayMatchSummary(first_innings_runs + 1, 2);
    determineWinner(first_innings_runs, total_runs + total_extras);

    return 0;
}