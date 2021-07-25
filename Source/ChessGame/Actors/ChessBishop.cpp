// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Actors/ChessBishop.h"
#include "ChessGame/ChessGameGameModeBase.h"
#include "ChessGame/Actors/ChessBoardBuilder.h"
#include "ChessGame/Actors/ChessBoardPiece.h"

void AChessBishop::SelectedChessPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed)
{
    if (!IsMyTeamsTurn())
    {
        return;
    }

	GameModeRef->SetActiveChessPiece(this);
    
    TArray<FIntPoint> PossibleMoves;
    CheckMovement(PossibleMoves); 
}

void AChessBishop::CheckMovement(TArray<FIntPoint>& ValidMoves)
{
    FIntPoint CandidatePoint {XCoord, YCoord};

    for (int32 Index = 0; Index < AllDirections.Num(); ++Index)
    {
        CheckDiagonal(ValidMoves, CandidatePoint, AllDirections[Index]);
    }
}

void AChessBishop::CheckDiagonal(TArray<FIntPoint>& ValidMoves, FIntPoint CandidatePoint, FIntPoint Directions)
{
    CheckRepeatedMovement(ValidMoves, CandidatePoint, Directions);
}