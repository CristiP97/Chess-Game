// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessGame/Actors/ChessPieceBase.h"
#include "ChessKnight.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessKnight : public AChessPieceBase
{
	GENERATED_BODY()

private:
	void CheckKnightDirections(TArray<FIntPoint>& ValidMoves, FIntPoint CandidatePoint, FIntPoint Directions);

protected:
	void SelectedChessPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed) override;

	void CheckMovement(TArray<FIntPoint>& ValidMoves) override;

};
