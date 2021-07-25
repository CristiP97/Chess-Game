// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Actors/ChessBishop.h"
#include "ChessGame/ChessGameGameModeBase.h"
#include "ChessGame/Actors/ChessBoardBuilder.h"
#include "ChessGame/Actors/ChessBoardPiece.h"

void AChessBishop::SelectedChessPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed)
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
    AChessBoardPiece* CandidateChessBoardPiece;
    
    do
    {
        CandidatePoint.X += Directions.X;
        CandidatePoint.Y += Directions.Y;

        CandidateChessBoardPiece = ChessBoardBuilderRef->GetChessBoardPieceAtCoords(CandidatePoint);

        if (CandidateChessBoardPiece)
        {
            AChessPieceBase* ChessPiece = CandidateChessBoardPiece->GetActiveChessPiece();

            if (!ChessPiece || (ChessPiece->GetTeam() != ChessPieceTeam))
            {
                ValidMoves.Emplace(CandidatePoint);
                ValidChessBoardPieces.Emplace(CandidateChessBoardPiece);
                CandidateChessBoardPiece->ShowAvailableSelection();
                CandidateChessBoardPiece->SetValidMoveSelection(true);
            }
            
            if (ChessPiece)
            {
                break;
            }
        }
    } 
    while (CandidateChessBoardPiece);
}