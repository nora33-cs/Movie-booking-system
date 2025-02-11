// movie_ticket_booking_system.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_MOVIES 5
#define MAX_TICKETS 10
#define MAX_ROWS 5
#define MAX_COLS 5
#define MAX_SHOWTIMES 3

struct Casting {
    char director[100];
    char actors[500];
};

struct Movie {
    char name[100];
    float rating;
    int price;
    int availableseats;
    struct Casting casting;
    int seats[MAX_SHOWTIMES][MAX_ROWS][MAX_COLS];  // 3D array for seats
};

struct Ticket {
    char movieName[100];
    int showtime;
    char row;
    int col;
    int price;
};

struct Movie movies[MAX_MOVIES] = {
        {"Ruby Red", 3.9, 250, 500, {"Jannis Niewöhner", "Maria Ehrich, Josefine Preuß"}},
        {"Vampire Diaries", 5.0, 250, 600, {"Ian Somerholder", "Paul Wesley, Nina Dobrev"}},
        {"The Originals", 4.8, 250, 650, {"Joseph Morgan", "Phoebe Tonkin, Daniel Gillies"}},
        {"Stranger Things", 4.9, 250, 300, {"Finn Wolfhard", "Millie Brown, Winona Ryder"}},
        {"Titanic", 4.7, 250, 700, {"Leo DiCaprio", "Kate Winslet"}}
};

struct Ticket tickets[MAX_TICKETS];
int numTickets = 0;

// Function to set text color
void setTextColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void displayMovies() {
    setTextColor(14); // Yellow
    printf("\n===================================== Movie Listing =====================================\n\n");
    printf("%-20s%-10s%-10s%-20s%-50s\n", "Movie Name", "Rating", "Price", "Director", "Cast");
    printf("--------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < MAX_MOVIES; i++) {
        printf("%-20s%-10.1f%-10d%-20s%-50s\n", movies[i].name, movies[i].rating, movies[i].price, movies[i].casting.director, movies[i].casting.actors);
    }
    printf("--------------------------------------------------------------------------------------------------\n");
    setTextColor(7); // Default color
}

void displaySeatingChart(int movieIndex, int showtimeIndex) {
    setTextColor(10); // Green
    printf("Seating Chart for %s (Showtime %d)\n", movies[movieIndex].name, showtimeIndex + 1);
    printf("  ");
    for (int col = 0; col < MAX_COLS; col++) {
        printf("%d ", col + 1);
    }
    printf("\n");

    for (int row = 0; row < MAX_ROWS; row++) {
        printf("%c ", 'A' + row);
        for (int col = 0; col < MAX_COLS; col++) {
            printf("%d ", movies[movieIndex].seats[showtimeIndex][row][col]);
        }
        printf("\n");
    }
    setTextColor(7); // Default color
}

void displayTicketDetails(struct Ticket ticket) {
    setTextColor(11); // Cyan
    printf("\nTicket Details:\n");
    printf("Movie: %s\n", ticket.movieName);
    printf("Showtime: %d\n", ticket.showtime + 1);
    printf("Seat: %c%d\n", ticket.row, ticket.col + 1);
    printf("Price: %d\n", ticket.price);
    setTextColor(7); // Default color
}

void bookTicket() {
    char movieName[100];
    int movieIndex = -1;
    int showtimeIndex = -1;
    char rowChar;
    int row, col;

    displayMovies();

    setTextColor(11); // Cyan
    printf("Enter the name of the movie you want to book: ");
    fgets(movieName, sizeof(movieName), stdin);
    movieName[strcspn(movieName, "\n")] = 0; // Remove newline character

    for (int i = 0; i < MAX_MOVIES; i++) {
        if (strcmp(movies[i].name, movieName) == 0) {
            movieIndex = i;
            break;
        }
    }

    if (movieIndex == -1) {
        printf("Invalid movie name.\n");
        setTextColor(7); // Default color
        return;
    }

    printf("Select showtime (1-%d): ", MAX_SHOWTIMES);
    scanf("%d", &showtimeIndex);
    showtimeIndex--;

    if (showtimeIndex < 0 || showtimeIndex >= MAX_SHOWTIMES) {
        printf("Invalid showtime.\n");
        setTextColor(7); // Default color
        return;
    }

    displaySeatingChart(movieIndex, showtimeIndex);

    printf("Enter the row (A-E): ");
    scanf(" %c", &rowChar);
    row = rowChar - 'A';

    printf("Enter the column (1-5): ");
    scanf("%d", &col);
    col--;

    if (row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS || movies[movieIndex].seats[showtimeIndex][row][col] != 0) {
        printf("Invalid seat selection or seat already booked.\n");
        setTextColor(7); // Default color
        return;
    }

    movies[movieIndex].seats[showtimeIndex][row][col] = 1;

    struct Ticket ticket;
    strcpy(ticket.movieName, movies[movieIndex].name);
    ticket.showtime = showtimeIndex;
    ticket.row = rowChar;
    ticket.col = col;
    ticket.price = movies[movieIndex].price;
    tickets[numTickets++] = ticket;

    displayTicketDetails(ticket);
}

void cancelTicket() {
    char movieName[100];
    int showtimeIndex = -1;
    char rowChar;
    int row, col;

    if (numTickets == 0) {
        printf("No tickets to cancel.\n");
        return;
    }

    setTextColor(11); // Cyan
    printf("Enter the name of the movie you want to cancel the ticket for: ");
    fgets(movieName, sizeof(movieName), stdin);
    movieName[strcspn(movieName, "\n")] = 0; // Remove newline character

    printf("Select showtime (1-%d): ", MAX_SHOWTIMES);
    scanf("%d", &showtimeIndex);
    showtimeIndex--;

    printf("Enter the row (A-E): ");
    scanf(" %c", &rowChar);
    row = rowChar - 'A';

    printf("Enter the column (1-5): ");
    scanf("%d", &col);
    col--;

    for (int i = 0; i < numTickets; i++) {
        if (strcmp(tickets[i].movieName, movieName) == 0 && tickets[i].showtime == showtimeIndex &&
            tickets[i].row == rowChar && tickets[i].col == col) {

            int movieIndex = 0;
            movies[movieIndex].seats[showtimeIndex][row][col] = 0;

            // Remove the ticket from the array
            for (int j = i; j < numTickets - 1; j++) {
                tickets[j] = tickets[j + 1];
            }
            numTickets--;

            printf("Ticket for %s at seat %c%d canceled successfully.\n", movieName, rowChar, col + 1);
            setTextColor(7); // Default color
            return;
        }
    }

    printf("No matching ticket found to cancel.\n");
    setTextColor(7); // Default color
}

int main() {
    char option;

    while (1) {
        setTextColor(13); // Magenta
        printf("\nMenu:\n");
        printf("1. Display Movies\n");
        printf("2. Book a Ticket\n");
        printf("3. Cancel a Ticket\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf(" %c", &option);
        getchar(); // Consume newline character

        switch (option) {
            case '1':
                displayMovies();
                break;
            case '2':
                bookTicket();
                break;
            case '3':
                cancelTicket();
                break;
            case '4':
                exit(0);
            default:
                setTextColor(12); // Red
                printf("Invalid option.\n");
                setTextColor(7); // Default color
        }
    }

    return 0;
}