// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Actors/ChessKnight.h"
#include "ChessGame/ChessGameGameModeBase.h"
#include "ChessGame/Actors/ChessBoardBuilder.h"
#include "ChessGame/Actors/ChessBoardPiece.h"

void AChessKnight::SelectedChessPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed)
{
    if (!IsMyTeamsTurn())
    {
        return;
    }

	GameModeRef->SetActiveChessPiece(this);
    
    TArray<FIntPoint> PossibleMoves;
    CheckMovement(PossibleMoves); 
}

void AChessKnight::CheckMovement(TArray<FIntPoint>& ValidMoves)
{
    FIntPoint StartingPoint {XCoord, YCoord};

    for (int32 Index = 0; Index < AllDirections.Num(); ++Index)
    {
        CheckKnightDirections(ValidMoves, StartingPoint, AllDirections[Index]);
    }
}

void AChessKnight::CheckKnightDirections(TArray<FIntPoint>& ValidMoves, FIntPoint CandidatePoint, FIntPoint Directions)
{
    CheckSingleMovement(ValidMoves, CandidatePoint, Directions);
}
