// Copyright Epic Games, Inc. All Rights Reserved.


#include "ChessGameGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "ChessGame/PlayerControllers/PlayerControllerBase.h"
#include "ChessGame/Actors/ChessPieceBase.h"

void AChessGameGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void AChessGameGameModeBase::HandleGameStart()
{
    PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

    if (PlayerControllerRef)
    {
        PlayerControllerRef->SetControllerEnabledState(true);
    }
}

void AChessGameGameModeBase::HandleGameEnd()
{

}

void AChessGameGameModeBase::SetActiveChessPiece(AActor* ChessPiece)
{
    if (ActiveChessPiece && ActiveChessPiece == ChessPiece)
    {
        Cast<AChessPieceBase>(ActiveChessPiece)->ClearAvailableSelection();
        return;
    }
    
    if (ActiveChessPiece)
    {
        Cast<AChessPieceBase>(ActiveChessPiece)->ClearAvailableSelection();
    }

    ActiveChessPiece = ChessPiece;
}

void AChessGameGameModeBase::ResetActiveChessPiece()
{
    if (ActiveChessPiece)
    {
        Cast<AChessPieceBase>(ActiveChessPiece)->ClearAvailableSelection();
    }

    ActiveChessPiece = nullptr;
    UE_LOG(LogTemp, Warning, TEXT("Reset the active chess piece!"));
}

AActor* AChessGameGameModeBase::GetSelectedChessPiece()
{
    return ActiveChessPiece;
}

void AChessGameGameModeBase::PlayerMoved()
{
    bIsWhiteTurn = !bIsWhiteTurn;

    if (bIsWhiteTurn)
    {
        UE_LOG(LogTemp, Warning, TEXT("Turn of player with white chess pieces!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Turn of player with black chess pieces!"));
    }
}

bool AChessGameGameModeBase::IsWhiteTurn()
{
    return bIsWhiteTurn;
}

void AChessGameGameModeBase::PlayerWon(int32 TeamPlayer)
{
    if (TeamPlayer == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player with white pieces won!"));
    }
    else if (TeamPlayer == 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player with black pieces won!"));
    }

    PlayerControllerRef->SetControllerEnabledState(false);
}