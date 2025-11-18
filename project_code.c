#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> 

// --- Game Configuration Structure ---
typedef struct {
    int max_number;      // The upper limit for the secret number
    int max_guesses;     // Maximum attempts allowed
    int secret_number;   // The number the user must guess
    int attempts_taken;  // The actual number of guesses used
    bool win_status;     // Did the player win?
} GameSettings;

// --- Function Prototypes ---
// Functions now accept a pointer to the GameSettings structure
void initialize_game(GameSettings *settings);
int get_user_guess(const GameSettings *settings); // const pointer: reads but doesn't change data
void check_guess(GameSettings *settings, int guess); 
void print_game_info(const GameSettings *settings);
void game_over_message(const GameSettings *settings);

// --------------------------

/**
 * @brief Prints the structured information about the current game configuration.
 */
void print_game_info(const GameSettings *settings) {
    printf("--------------------------------------------------\n");
    printf("🎮 GAME CONFIGURATION\n");
    printf("--------------------------------------------------\n");
    printf("Maximum Guessable Number: %d\n", settings->max_number);
    printf("Total Attempts Allowed:   %d\n", settings->max_guesses);
    printf("--------------------------------------------------\n");
}


/**
 * @brief Sets up the game parameters and generates the secret number.
 */
void initialize_game(GameSettings *settings) {
    // Set initial configuration and status
    settings->max_number = 100;
    settings->max_guesses = 10;
    settings->attempts_taken = 0;
    settings->win_status = false;

    // Seed the random number generator
    srand(time(NULL));

    // Generate a random number between 1 and max_number
    settings->secret_number = (rand() % settings->max_number) + 1;

    // --- Welcome Messages ---
    printf("==================================================\n");
    printf("     Welcome to the C Number Guessing Game!       \n");
    printf("==================================================\n");
    
    print_game_info(settings); 
    
    printf("I have picked a secret number between 1 and %d.\n", settings->max_number);
    printf("You have %d attempts to guess it.\n\n", settings->max_guesses);
}

/**
 * @brief Prompts the user for a guess and handles input validation.
 * @return The valid integer guess entered by the user.
 */
int get_user_guess(const GameSettings *settings) {
    int guess;
    
    while (1) {
        printf("Attempt %d/%d. Enter your guess: ", 
               settings->attempts_taken + 1, // Accessing data via pointer
               settings->max_guesses);
        
        if (scanf("%d", &guess) != 1) {
            printf("\n[ERROR] Invalid input. Please enter a whole number.\n");
            
            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF); 
            continue; 
        }
        
        // Basic range check
        if (guess < 1 || guess > settings->max_number) {
            printf("\n[ERROR] Guess must be between 1 and %d.\n", settings->max_number);
            continue; 
        }
        
        return guess; 
    }
}

/**
 * @brief Checks the user's guess against the secret number and updates game status.
 */
void check_guess(GameSettings *settings, int guess) {
    settings->attempts_taken++; // Update attempts via pointer
    
    if (guess == settings->secret_number) { // Accessing data via pointer
        settings->win_status = true;
    } else if (guess < settings->secret_number) {
        printf("--- Too Low! Try again. ---\n\n");
    } else { // guess > secret_number
        printf("--- Too High! Try again. ---\n\n");
    }
}

/**
 * @brief Prints the final game outcome message.
 */
void game_over_message(const GameSettings *settings) {
    if (settings->win_status) {
        printf("\n*** CONGRATULATIONS! ***\n");
        printf("You guessed the number %d in %d attempts!\n", 
               settings->secret_number, 
               settings->attempts_taken);
    } else {
        printf("\n--- GAME OVER ---\n");
        printf("You ran out of guesses. The secret number was %d.\n", settings->secret_number);
    }
}

// --------------------------

/**
 * @brief Main function: controls the primary game loop.
 */
int main() {
    // 1. Declare the structure variable
    GameSettings game_data;

    // 2. Pass the ADDRESS (&) of the structure to initialize the game
    initialize_game(&game_data);

    // --- Main Game Loop ---
    // Loop continues if the player hasn't won AND has attempts remaining
    while (!game_data.win_status && game_data.attempts_taken < game_data.max_guesses) {
        
        int current_guess = get_user_guess(&game_data);
        
        // Pass the ADDRESS (&) of the structure to update its internal state
        check_guess(&game_data, current_guess);
    }
    
    // Output final result
    game_over_message(&game_data);

    return 0;
}