// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessGame/Actors/ChessPieceBase.h"
#include "ChessQueen.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessQueen : public AChessPieceBase
{
	GENERATED_BODY()

protected:
	void SelectedChessPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed) override;

	void CheckMovement(TArray<FIntPoint>& ValidMoves) override;
};
