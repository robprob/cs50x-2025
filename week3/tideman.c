// Original program provided by CS50

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;
bool circuitFound = false;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void findCircuit(int startCandidate, int destinationCandidate);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Functions updated by me

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO

    // Iterate candidates to find a name match
    for (int i = 0, n = candidate_count; i < n; i++)
    {
        // If match, add index of candidate to ranks array at respective index
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    // Return false if candidate name not found
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO

    // Increment count of voters preferring specific candidate over another
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO

    // For each pair, determine if there is a higher preference
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            int countA = preferences[i][j];
            int countB = preferences[j][i];

            if (countA > countB)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO

    // Bubble sort lower strength wins to end
    pair tempPair;
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = 0; j < pair_count - i - 1; j++)
        {
            // Calculate strength of victory for each pair
            int strengthA = preferences[pairs[j].winner][pairs[j].loser] -
                            preferences[pairs[j].loser][pairs[j].winner];
            int strengthB = preferences[pairs[j + 1].winner][pairs[j + 1].loser] -
                            preferences[pairs[j + 1].loser][pairs[j + 1].winner];

            if (strengthA < strengthB)
            {
                // Perform swap using tempPair as helper
                tempPair = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = tempPair;
            }
        }
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO

    // Iterate over pairs ordered in decreasing strength
    for (int i = 0; i < pair_count; i++)
    {
        // Assume no circuit is created, lock pair in adjacency matrix
        locked[pairs[i].winner][pairs[i].loser] = 1;

        // Follow pointers to find any circuits
        findCircuit(pairs[i].winner, pairs[i].loser);

        // If circuit found, undo lock and continue to next pair
        if (circuitFound)
        {
            locked[pairs[i].winner][pairs[i].loser] = 0;
            circuitFound = false;
        }
    }
}

// Follow pointers, attempting to find a circuit back to start candidate
void findCircuit(int startCandidate, int destinationCandidate)
{
    // Follow any path that new destination points to
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[destinationCandidate][i] == 1)
        {
            // If back to start candidate, circuit has been found
            if (i == startCandidate)
            {
                circuitFound = true;
            }
            // Continue recursively searching
            else
            {
                findCircuit(startCandidate, i);
            }
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    // TODO

    // Determine the source of the graph by iterating through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Iterate through other candidates to see if any point to source
        bool pointerFound = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == 1)
            {
                pointerFound = true;
                break;
            }
        }

        if (!(pointerFound))
        {
            // Print winning candidate
            printf("%s\n", candidates[i]);
            break;
        }
    }
}
