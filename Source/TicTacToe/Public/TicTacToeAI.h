#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TicTacToeAI.generated.h"

UCLASS()
class TICTACTOE_API ATicTacToeAI : public AActor
{
    GENERATED_BODY()

public:
    ATicTacToeAI();

    // The main board array representing the state (0 = empty, 1 = player, -1 = AI)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TicTacToe")
    TArray<int> Board;

    // Blueprint function to check the board for a win, loss, or draw
    UFUNCTION(BlueprintCallable, Category = "TicTacToe")
    int CheckWin() const;

    // Blueprint function for the player to make a move at the given index
    UFUNCTION(BlueprintCallable, Category = "TicTacToe")
    bool MakeMove(int Index, bool bIsPlayer);

    // Blueprint function to get the best move for the AI
    UFUNCTION(BlueprintCallable, Category = "TicTacToe")
    int GetBestMove();

    // Blueprint function for the AI to make a move
    UFUNCTION(BlueprintCallable, Category = "TicTacToe")
    void AIMove();

    // Blueprint function to spawn the marker for a move at the given index
    UFUNCTION(BlueprintCallable, Category = "TicTacToe")
    void SpawnMarkerAtIndex(int Index, bool bIsPlayer);

protected:
    // Blueprint references for player (X) and AI (O) markers
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TicTacToe")
    TSubclassOf<AActor> BP_TTTX; // Player's X marker

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TicTacToe")
    TSubclassOf<AActor> BP_TTTO; // AI's O marker

    // Helper function to get location based on index
    FVector GetLocationForIndex(int Index);

    // Array of actors representing each grid location
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TicTacToe")
    TArray<AActor*> GridActors;

private:
    // Minimax algorithm for AI decision-making (not exposed to Blueprints)
    int Minimax(TArray<int>& BoardState, bool bIsAITurn);
};