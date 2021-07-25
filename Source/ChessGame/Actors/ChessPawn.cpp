// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Actors/ChessPawn.h"
#include "ChessGame/Actors/ChessBoardBuilder.h"
#include "ChessGame/Actors/ChessBoardPiece.h"
#include "ChessGame/ChessGameGameModeBase.h"
#include "Math/IntPoint.h"

AChessPawn::AChessPawn()
{

}

void AChessPawn::BeginPlay() {
    Super::BeginPlay();
}

void AChessPawn::SelectedChessPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed)
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

void AChessPawn::CheckMovement(TArray<FIntPoint>& ValidMoves)
{
    int32 MoveForward = 1;
    int32 JumpForward = 2;

    if (ChessPieceTeam == 1)
    {
        MoveForward = -1;
        JumpForward = -2;
    }

    FIntPoint CandidatePoint {XCoord, YCoord + MoveForward};
    ForwardMovement(ValidMoves, CandidatePoint, JumpForward);

    //Check attack position right
    CandidatePoint.X = XCoord + MoveForward;
    CandidatePoint.Y = YCoord + MoveForward;
    RightAttack(ValidMoves, CandidatePoint);

    // Check attack position left
    CandidatePoint.X = XCoord - MoveForward;
    CandidatePoint.Y = YCoord + MoveForward;
    LeftAttack(ValidMoves, CandidatePoint);
}

void AChessPawn::ForwardMovement(TArray<FIntPoint>& ValidMoves, FIntPoint CandidatePoint, int32 JumpForward)
{
    AChessBoardPiece* CandidateChessBoardPiece;
    CandidateChessBoardPiece = ChessBoardBuilderRef->GetChessBoardPieceAtCoords(CandidatePoint);

    // Forward Movement
    if (CandidateChessBoardPiece && !CandidateChessBoardPiece->GetActiveChessPiece())
    {
        ValidMoves.Emplace(CandidatePoint);
        ValidChessBoardPieces.Emplace(CandidateChessBoardPiece);
        CandidateChessBoardPiece->ShowAvailableSelection();
        CandidateChessBoardPiece->SetValidMoveSelection(true);

        CandidatePoint.Y = YCoord + JumpForward;
        CandidateChessBoardPiece = ChessBoardBuilderRef->GetChessBoardPieceAtCoords(CandidatePoint);

        if (CandidateChessBoardPiece && CandidateChessBoardPiece->IsBeyondHalf())
        {
            return;
        }

        if (CandidateChessBoardPiece && !CandidateChessBoardPiece->GetActiveChessPiece())
        {
            ValidMoves.Emplace(CandidatePoint);
            ValidChessBoardPieces.Emplace(CandidateChessBoardPiece);
            CandidateChessBoardPiece->ShowAvailableSelection();
            CandidateChessBoardPiece->SetValidMoveSelection(true);
        }
    } 
}

void AChessPawn::RightAttack(TArray<FIntPoint>& ValidMoves, FIntPoint CandidatePoint)
{
    AChessBoardPiece* CandidateChessBoardPiece;
    CandidateChessBoardPiece = ChessBoardBuilderRef->GetChessBoardPieceAtCoords(CandidatePoint);
    if (CandidateChessBoardPiece)
    {
        AChessPieceBase* CurrentChessPiece = CandidateChessBoardPiece->GetActiveChessPiece();

        if (CurrentChessPiece && CurrentChessPiece->GetTeam() != ChessPieceTeam)
        {
            ValidMoves.Emplace(CandidatePoint);
            ValidChessBoardPieces.Emplace(CandidateChessBoardPiece);
            CandidateChessBoardPiece->ShowAvailableSelection();
            CandidateChessBoardPiece->SetValidMoveSelection(true);
        }
    }
}

void AChessPawn::LeftAttack(TArray<FIntPoint>& ValidMoves, FIntPoint CandidatePoint)
{
    AChessBoardPiece* CandidateChessBoardPiece;
    CandidateChessBoardPiece = ChessBoardBuilderRef->GetChessBoardPieceAtCoords(CandidatePoint);

    if (CandidateChessBoardPiece)
    {
        AChessPieceBase* CurrentChessPiece = CandidateChessBoardPiece->GetActiveChessPiece();

        if (CurrentChessPiece && CurrentChessPiece->GetTeam() != ChessPieceTeam)
        {
            ValidMoves.Emplace(CandidatePoint);
            ValidChessBoardPieces.Emplace(CandidateChessBoardPiece);
            CandidateChessBoardPiece->ShowAvailableSelection();
            CandidateChessBoardPiece->SetValidMoveSelection(true);
        }
    }
}

