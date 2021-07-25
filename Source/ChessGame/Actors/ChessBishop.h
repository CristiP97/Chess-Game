// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessGame/Actors/ChessPieceBase.h"
#include "ChessBishop.generated.h"

struct FIntPoint;

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessBishop : public AChessPieceBase
{
	GENERATED_BODY()

private:
	void CheckDiagonal(TArray<FIntPoint>& ValidMoves, FIntPoint CandidatePoint, FIntPoint Directions);

protected:
	void SelectedChessPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed) override;

	void CheckMovement(TArray<FIntPoint>& ValidMoves);
	
};
