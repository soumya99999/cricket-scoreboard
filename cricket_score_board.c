#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PLAYERS 11
#define NAME_LEN 64

typedef struct {
    char name[NAME_LEN];
    int runs;
    int balls;
    int ones, twos, threes, fours, sixes;
    int is_out;
    double strike_rate;
} Batsman;

typedef struct {
    char name[NAME_LEN];
    int runs_given;
    int wickets_taken;
    int balls_bowled; /* total balls (overs*6 + extra balls) */
    int extras;
    double economy;
} Bowler;

typedef struct {
    Batsman batsmen[MAX_PLAYERS];
    Bowler bowlers[MAX_PLAYERS];
    int num_batsmen;
    int num_bowlers;
    int total_runs;   /* sum of batsmen runs */
    int total_extras; /* sum of bowler extras */
    int total_wickets;
} Innings;

/* ---------- Helper Input Utilities ---------- */

static void readLine(char *buf, size_t n) {
    if (!fgets(buf, (int)n, stdin)) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len && buf[len - 1] == '\n') buf[len - 1] = '\0';
}

/* read integer with prompt; enforce min..max if max >= min */
static int readIntRange(const char *prompt, int min, int max) {
    char line[128];
    int val;
    while (1) {
        printf("%s", prompt);
        readLine(line, sizeof(line));
        if (sscanf(line, "%d", &val) != 1) {
            printf("Invalid integer. Try again.\n");
            continue;
        }
        if (max >= min && (val < min || val > max)) {
            if (min == max)
                printf("Value must be %d. Try again.\n", min);
            else
                printf("Value must be between %d and %d. Try again.\n", min, max);
            continue;
        }
        return val;
    }
}

/* ---------- Core Functions ---------- */

static void inputMatchDetails(int *num_batsmen, int *num_bowlers, int *max_overs) {
    *num_batsmen = readIntRange("Enter the number of batsmen (1-11): ", 1, MAX_PLAYERS);
    *num_bowlers = readIntRange("Enter the number of bowlers (1-11): ", 1, MAX_PLAYERS);
    *max_overs   = readIntRange("Enter the number of overs for the match (>=1): ", 1, 100);
}

static void inputBatsmanData(Innings *inn, int max_balls, int innings_no) {
    int balls_remaining = max_balls;
    printf("\nInnings %d - Batsmen Details (Total balls available: %d)\n", innings_no, max_balls);

    for (int i = 0; i < inn->num_batsmen; ++i) {
        printf("\nBatsman %d name: ", i + 1);
        readLine(inn->batsmen[i].name, NAME_LEN);
        if (inn->batsmen[i].name[0] == '\0') snprintf(inn->batsmen[i].name, NAME_LEN, "Batsman%d", i+1);

        inn->batsmen[i].ones = readIntRange("  Ones: ", 0, 1000000);
        inn->batsmen[i].twos = readIntRange("  Twos: ", 0, 1000000);
        inn->batsmen[i].threes = readIntRange("  Threes: ", 0, 1000000);
        inn->batsmen[i].fours = readIntRange("  Fours: ", 0, 1000000);
        inn->batsmen[i].sixes = readIntRange("  Sixes: ", 0, 1000000);

        char prompt[128];
        snprintf(prompt, sizeof(prompt), "  Balls faced by %s (0 to %d): ", inn->batsmen[i].name, balls_remaining);
        inn->batsmen[i].balls = readIntRange(prompt, 0, balls_remaining);
        balls_remaining -= inn->batsmen[i].balls;

        inn->batsmen[i].is_out = readIntRange("  Is out? (1 for Yes, 0 for No): ", 0, 1);
    }

    if (balls_remaining > 0) {
        printf("Warning: %d balls remaining unplayed in this innings.\n", balls_remaining);
    }
}

/* Read overs and balls as two integers and convert to total balls bowled for a bowler */
static int readOversToBalls(const char *bowler_name, int max_balls_remaining) {
    while (1) {
        int overs = readIntRange("  Overs bowled (integer part): ", 0, max_balls_remaining / 6);
        int extra_balls = readIntRange("  Extra balls this over (0-5): ", 0, 5);
        int tot = overs * 6 + extra_balls;
        if (tot <= max_balls_remaining) return tot;
        printf("  That exceeds remaining balls (%d). Try again.\n", max_balls_remaining);
    }
}

static void inputBowlerData(Innings *inn, int max_overs, int innings_no) {
    int total_balls = max_overs * 6;
    int balls_remaining = total_balls;
    int runs_remaining = inn->total_runs; /* runs to be distributed among bowlers (excluding extras) */
    inn->total_extras = 0;

    printf("\nInnings %d - Bowlers Details\n", innings_no);
    printf("Total batsmen runs (excluding extras) to distribute to bowlers: %d\n", inn->total_runs);

    for (int i = 0; i < inn->num_bowlers; ++i) {
        printf("\nBowler %d name: ", i + 1);
        readLine(inn->bowlers[i].name, NAME_LEN);
        if (inn->bowlers[i].name[0] == '\0') snprintf(inn->bowlers[i].name, NAME_LEN, "Bowler%d", i+1);

        printf("Enter overs and balls for %s (remaining balls: %d)\n", inn->bowlers[i].name, balls_remaining);
        inn->bowlers[i].balls_bowled = readOversToBalls(inn->bowlers[i].name, balls_remaining);
        balls_remaining -= inn->bowlers[i].balls_bowled;

        if (i == inn->num_bowlers - 1) {
            /* Last bowler takes remaining runs */
            inn->bowlers[i].runs_given = runs_remaining;
            char prompt[128];
            snprintf(prompt, sizeof(prompt), "  Wickets taken by %s: ", inn->bowlers[i].name);
            inn->bowlers[i].wickets_taken = readIntRange(prompt, 0, inn->num_batsmen);
            snprintf(prompt, sizeof(prompt), "  Extras conceded by %s: ", inn->bowlers[i].name);
            inn->bowlers[i].extras = readIntRange(prompt, 0, 1000000);
        } else {
            char prompt[128];
            snprintf(prompt, sizeof(prompt), "  Runs given by %s (0 to %d): ", inn->bowlers[i].name, runs_remaining);
            inn->bowlers[i].runs_given = readIntRange(prompt, 0, runs_remaining);
            snprintf(prompt, sizeof(prompt), "  Wickets taken by %s: ", inn->bowlers[i].name);
            inn->bowlers[i].wickets_taken = readIntRange(prompt, 0, inn->num_batsmen);
            snprintf(prompt, sizeof(prompt), "  Extras conceded by %s: ", inn->bowlers[i].name);
            inn->bowlers[i].extras = readIntRange(prompt, 0, 1000000);

            runs_remaining -= inn->bowlers[i].runs_given;
        }

        inn->total_extras += inn->bowlers[i].extras;
    }

    if (balls_remaining > 0) {
        int rem_overs = balls_remaining / 6;
        int rem_balls = balls_remaining % 6;
        printf("Warning: %d balls (%d overs and %d balls) remaining unbowled!\n", balls_remaining, rem_overs, rem_balls);
    }
    if (runs_remaining > 0 && inn->num_bowlers > 0) {
        printf("Warning: %d runs were not distributed among bowlers!\n", runs_remaining);
    }
}

/* compute totals, strike rates and economies */
static void calculateStats(Innings *inn) {
    inn->total_runs = 0;
    inn->total_wickets = 0;

    for (int i = 0; i < inn->num_batsmen; ++i) {
        Batsman *b = &inn->batsmen[i];
        b->runs = b->ones + (b->twos * 2) + (b->threes * 3) + (b->fours * 4) + (b->sixes * 6);
        b->strike_rate = (b->balls > 0) ? ((double)b->runs * 100.0 / (double)b->balls) : 0.0;
        inn->total_runs += b->runs;
        if (b->is_out) inn->total_wickets++;
    }

    for (int i = 0; i < inn->num_bowlers; ++i) {
        Bowler *bw = &inn->bowlers[i];
        if (bw->balls_bowled > 0) {
            double overs = (double)bw->balls_bowled / 6.0;
            bw->economy = ((double)bw->runs_given + (double)bw->extras) / overs;
        } else {
            bw->economy = 0.0;
        }
    }
}

/* display overs in O.B format (e.g., 4.2 means 4 overs and 2 balls) */
static void printOversFromBalls(int balls) {
    int overs = balls / 6;
    int rem = balls % 6;
    printf("%d.%d", overs, rem);
}

static void displayMatchSummary(const Innings *inn, int max_wickets, int max_overs, int innings_no, int target_for_next) {
    printf("\n======== INNINGS %d SUMMARY ========\n", innings_no);

    printf("\nBatting Scorecard\n");
    printf("Batsman\t\tRuns\tBalls\tFours\tSixes\tSR\tOut\n");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < inn->num_batsmen; ++i) {
        const Batsman *b = &inn->batsmen[i];
        printf("%-15s %-5d %-5d %-5d %-5d %-7.2f %-4s\n",
               b->name, b->runs, b->balls, b->fours, b->sixes, b->strike_rate, b->is_out ? "Yes" : "No");
    }

    printf("\nBowling Scorecard\n");
    printf("Bowler\t\tOvers\tRuns\tWickets\tExtras\tEcon\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < inn->num_bowlers; ++i) {
        const Bowler *bw = &inn->bowlers[i];
        printf("%-15s ", bw->name);
        printOversFromBalls(bw->balls_bowled);
        printf("\t%-5d %-7d %-6d %-5.2f\n",
               bw->runs_given + bw->extras, bw->wickets_taken, bw->extras, bw->economy);
    }

    int total_innings_runs = inn->total_runs + inn->total_extras;
    printf("\nTotal Runs: %d, Wickets: %d/%d, Extras: %d\n",
           total_innings_runs, inn->total_wickets, max_wickets, inn->total_extras);

    if (innings_no == 1) {
        printf("Target for Second Innings: %d\n", target_for_next);
    }
}

static void determineWinner(int runs_team1, int runs_team2, int second_innings_wickets_taken, int max_wickets) {
    printf("\n======== MATCH RESULT ========\n");
    if (runs_team1 > runs_team2) {
        printf("Team 1 wins by %d runs!\n", runs_team1 - runs_team2);
    } else if (runs_team2 > runs_team1) {
        /* remaining wickets = max_wickets - wickets lost by the chasing team */
        int wickets_in_hand = max_wickets - second_innings_wickets_taken;
        if (wickets_in_hand < 0) wickets_in_hand = 0;
        printf("Team 2 wins by %d wicket%s!\n", wickets_in_hand, (wickets_in_hand == 1) ? "" : "s");
    } else {
        printf("Match is a tie!\n");
    }
}

/* ---------- Main ---------- */

int main(void) {
    Innings innings1 = {0}, innings2 = {0};
    int max_overs = 0;

    inputMatchDetails(&innings1.num_batsmen, &innings1.num_bowlers, &max_overs);
    if (innings1.num_batsmen > MAX_PLAYERS) innings1.num_batsmen = MAX_PLAYERS;
    if (innings1.num_bowlers > MAX_PLAYERS) innings1.num_bowlers = MAX_PLAYERS;

    int max_wickets = innings1.num_batsmen - 1;
    int max_balls = max_overs * 6;

    /* FIRST INNINGS */
    printf("\n======== FIRST INNINGS ========\n");
    inputBatsmanData(&innings1, max_balls, 1);
    calculateStats(&innings1);
    inputBowlerData(&innings1, max_overs, 1);
    calculateStats(&innings1); /* recalc economy after bowlers input */
    int first_innings_total = innings1.total_runs + innings1.total_extras;
    displayMatchSummary(&innings1, max_wickets, max_overs, 1, first_innings_total + 1);

    /* SECOND INNINGS - set players counts same as first innings by default */
    innings2.num_batsmen = innings1.num_batsmen;
    innings2.num_bowlers = innings1.num_bowlers;

    printf("\n======== SECOND INNINGS ========\n");
    inputBatsmanData(&innings2, max_balls, 2);
    calculateStats(&innings2);
    inputBowlerData(&innings2, max_overs, 2);
    calculateStats(&innings2); /* recalc economy after bowlers input */

    int second_innings_total = innings2.total_runs + innings2.total_extras;
    displayMatchSummary(&innings2, max_wickets, max_overs, 2, first_innings_total + 1);

    /* Determine winner: note that innings2.total_wickets is wickets lost by team 2 */
    determineWinner(first_innings_total, second_innings_total, innings2.total_wickets, max_wickets);

    return 0;
}
