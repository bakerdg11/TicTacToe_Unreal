#include "TicTacToeAI.h"
#include "Engine/World.h"




// Constructor
ATicTacToeAI::ATicTacToeAI() 
{
    // Initialize the board with empty cells
    Board.Init(0, 9); // 0 for empty, 1 for player (X), -1 for AI (O)
}



// This sets the 9 translucent box positions into the array. 
FVector ATicTacToeAI::GetLocationForIndex(int Index)
{
    if (GridActors.IsValidIndex(Index))
    {
        return GridActors[Index]->GetActorLocation();
    }

    UE_LOG(LogTemp, Warning, TEXT("Invalid index for grid actor location!"));
    return FVector::ZeroVector; // Return a default location if index is invalid
}



// Check if there's a winner
int ATicTacToeAI::CheckWin() const 
{
    const int WinningCombos[8][3] = 
    {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // columns
        {0, 4, 8}, {2, 4, 6}             // diagonals
    };

    for (const auto& Combo : WinningCombos) 
    {
        if (Board[Combo[0]] != 0 &&
            Board[Combo[0]] == Board[Combo[1]] &&
            Board[Combo[1]] == Board[Combo[2]]) 
        {
            return Board[Combo[0]]; // Return the winner (1 for player, -1 for AI)
        }
    }



    // Check for draw
    for (int Cell : Board) 
    {
        if (Cell == 0) return 0; // Game is still ongoing
    }
    return 2; // Return 2 for a draw
}



// Minimax algorithm (private helper function)
int ATicTacToeAI::Minimax(TArray<int>& BoardState, bool bIsAITurn) 
{
    int Score = CheckWin();
    if (Score == 1) return -10; // Player wins
    if (Score == -1) return 10; // AI wins
    if (Score == 2) return 0; // Draw

    int BestScore = bIsAITurn ? -1000 : 1000;
    for (int i = 0; i < 9; i++) 
    {
        if (BoardState[i] == 0) 
        { // Empty cell
            BoardState[i] = bIsAITurn ? -1 : 1; // AI or Player move
            int MoveScore = Minimax(BoardState, !bIsAITurn);
            BoardState[i] = 0; // Undo move
            BestScore = bIsAITurn ? FMath::Max(BestScore, MoveScore) : FMath::Min(BestScore, MoveScore);
        }
    }
    return BestScore;
}



// Get the best move for the AI
int ATicTacToeAI::GetBestMove() 
{
    int BestScore = -1000;
    int BestMove = -1;

    for (int i = 0; i < 9; i++) 
    {
        if (Board[i] == 0) { // Empty cell
            Board[i] = -1; // AI's move
            int MoveScore = Minimax(Board, false); // Evaluate player’s response
            Board[i] = 0; // Undo move

            if (MoveScore > BestScore) 
            {
                BestScore = MoveScore;
                BestMove = i;
            }
        }
    }
    return BestMove;
}



// Player makes a move
bool ATicTacToeAI::MakeMove(int Index, bool bIsPlayer) 
{
    if (Index < 0 || Index >= Board.Num() || Board[Index] != 0) 
    {
        return false; // Invalid move
    }

    Board[Index] = bIsPlayer ? 1 : -1;
    return true;
}



// AI makes its move
void ATicTacToeAI::AIMove() 
{
    int BestMoveIndex = GetBestMove();
    if (BestMoveIndex != -1) 
    {
        Board[BestMoveIndex] = -1;
        // Spawn or update AI's marker (O) at BestMoveIndex location
        SpawnMarkerAtIndex(BestMoveIndex, false);
    }
}



// Function to spawn the player's or AI's marker on the board
void ATicTacToeAI::SpawnMarkerAtIndex(int Index, bool bIsPlayer) 
{
    FVector Location = GetLocationForIndex(Index); // Get the position based on index

    // Check if the Blueprint references are valid before spawning
    if (BP_TTTX && BP_TTTO) 
    {
        AActor* Marker = GetWorld()->SpawnActor<AActor>(bIsPlayer ? BP_TTTX : BP_TTTO, Location, FRotator::ZeroRotator);
    }
    else 
    {
        UE_LOG(LogTemp, Warning, TEXT("BP_TTTX or BP_TTTO not set in the ATicTacToeAI instance."));
    }
}

