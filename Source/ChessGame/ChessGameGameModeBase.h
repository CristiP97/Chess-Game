// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChessGameGameModeBase.generated.h"

class APlayerControllerBase;

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	void HandleGameStart();
	void HandleGameEnd();

	

	APlayerControllerBase* PlayerControllerRef;
	AActor* ActiveChessPiece = nullptr;

	bool bIsWhiteTurn = true;


public:
	void SetActiveChessPiece(AActor* ChessPiece);
	void ResetActiveChessPiece();
	void PlayerMoved();
	bool IsWhiteTurn();
	void PlayerWon(int32 PlayerTeam);

	AActor* GetSelectedChessPiece();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowWinnerText();

};
