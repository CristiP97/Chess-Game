// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Actors/ChessQueen.h"
#include "ChessGame/ChessGameGameModeBase.h"

void AChessQueen::SelectedChessPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed)
{
    if (!IsMyTeamsTurn())
    {
        return;
    }

	GameModeRef->SetActiveChessPiece(this);
    
    TArray<FIntPoint> PossibleMoves;
    CheckMovement(PossibleMoves); 
}

void AChessQueen::CheckMovement(TArray<FIntPoint>& ValidMoves)
{
    FIntPoint CandidatePoint {XCoord, YCoord};

    for (int32 Index = 0; Index < AllDirections.Num(); ++Index)
    {
        CheckRepeatedMovement(ValidMoves, CandidatePoint, AllDirections[Index]);
    }
}