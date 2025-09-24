# Cricket Scoreboard

A simple C-based console application for tracking and calculating cricket match scores, player statistics, and determining match winners.

## Description

This program allows users to input detailed match information including batsman and bowler statistics for a complete cricket match. It calculates runs, strike rates, bowling economies, and displays comprehensive scorecards for both innings. The application determines the winner based on the final scores and wickets taken.

## Features

- **Match Setup**: Configure number of players (batsmen/bowlers) and overs per innings
- **Detailed Player Input**: Enter individual batsman and bowler statistics
- **Automatic Calculations**:
  - Batsman runs, strike rates, and out status
  - Bowler economies, runs conceded, and wickets taken
  - Total team scores including extras
- **Scorecard Display**: Formatted output showing batting and bowling cards
- **Match Result**: Automatic winner determination based on runs and wickets
- **Input Validation**: Ensures valid ranges for all inputs

## Requirements

- A C compiler (GCC, Clang, or MSVC)
- Standard C libraries (stdio.h, stdlib.h, string.h, ctype.h)
- Command-line interface for running the program

## Installation and Compilation

1. **Clone or download** the project files to your local machine
2. **Navigate** to the project directory
3. **Compile** the program using your C compiler:

   **Using GCC (Linux/Mac):**
   ```bash
   gcc cricket_score_board.c -o cricket_scoreboard
   ```

   **Using MSVC (Windows):**
   ```cmd
   cl cricket_score_board.c
   ```

   **Using Clang:**
   ```bash
   clang cricket_score_board.c -o cricket_scoreboard
   ```

4. The executable will be created as `cricket_scoreboard.exe` (Windows) or `cricket_scoreboard` (Unix-like systems)

## Usage

1. **Run** the compiled executable:
   ```bash
   ./cricket_scoreboard
   ```

2. **Follow the prompts** to enter:
   - Number of batsmen (1-11)
   - Number of bowlers (1-11)
   - Number of overs for the match

3. **First Innings**:
   - Enter batsman details (name, runs by type, balls faced, out status)
   - Enter bowler details (name, overs bowled, runs given, wickets, extras)

4. **Second Innings**:
   - Repeat the process for the second team

5. **View Results**:
   - The program will display scorecards for both innings
   - Final match result will be shown

## File Structure

```
cricket_scoreboard/
├── cricket_score_board.c    # Main source code file
├── cricket_score_board.exe  # Compiled executable (Windows)
├── match_history.txt        # Sample output data
├── output/                  # Build output directory
│   └── cricket_score_board.exe
└── README.md               # This file
```

## Example Output

```
======== INNINGS 1 SUMMARY ========

Batting Scorecard
Batsman          Runs  Balls  Fours  Sixes  SR     Out
-------------------------------------------------------------
Batsman1         45    30     4      2      150.00 Yes
Batsman2         23    18     2      1      127.78 No

Bowling Scorecard
Bowler           Overs  Runs   Wickets  Extras  Econ
-------------------------------------------------
Bowler1          5.0    28     1        2       5.60
Bowler2          3.2    22     0        1       6.60

Total Runs: 102, Wickets: 1/10, Extras: 0
Target for Second Innings: 103

======== MATCH RESULT ========
Team 2 wins by 5 wickets!
```

## Contributing

This is a simple educational project. Feel free to fork and modify as needed.

## License

This project is provided as-is for educational purposes.
