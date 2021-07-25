// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Actors/ChessRook.h"
#include "ChessGame/ChessGameGameModeBase.h"
#include "ChessGame/Actors/ChessBoardBuilder.h"
#include "ChessGame/Actors/ChessBoardPiece.h"

void AChessRook::SelectedChessPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed)
{
    Super::SelectedChessPiece(touchedComponent, buttonPressed);

    if (GameModeRef->IsWhiteTurn() && ChessPieceTeam == 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("Can't select black pieces on first player turn"));
        return;
    }

    if (!GameModeRef->IsWhiteTurn() && ChessPieceTeam == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Can't select white pieces on second player turn"));
        return;
    }

	GameModeRef->SetActiveChessPiece(this);
    
    TArray<FIntPoint> PossibleMoves;
    CheckMovement(PossibleMoves); 
}

void AChessRook::CheckMovement(TArray<FIntPoint>& ValidMoves)
{
    FIntPoint StartingPoint {XCoord, YCoord};

    for (int32 Index = 0; Index < AllDirections.Num(); ++Index)
    {
        CheckPerpendicularDirections(ValidMoves, StartingPoint, AllDirections[Index]);
    }
}

void AChessRook::CheckPerpendicularDirections(TArray<FIntPoint>& ValidMoves, FIntPoint CandidatePoint, FIntPoint Directions)
{
    CheckRepeatedMovement(ValidMoves, CandidatePoint, Directions);
}