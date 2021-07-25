// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Actors/ChessRook.h"
#include "ChessGame/ChessGameGameModeBase.h"
#include "ChessGame/Actors/ChessBoardBuilder.h"
#include "ChessGame/Actors/ChessBoardPiece.h"

void AChessRook::SelectedChessPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed)
{
    if (!IsMyTeamsTurn())
    {
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