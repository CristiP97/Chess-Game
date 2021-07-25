// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessGame/Actors/ChessPieceBase.h"
#include "ChessPawn.generated.h"

struct FIntPoint;

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessPawn : public AChessPieceBase
{
	GENERATED_BODY()
	
private:
	void ForwardMovement(TArray<FIntPoint>& ValidMoves, FIntPoint CandidatePoint, int32 JumpForward);
	void LeftAttack(TArray<FIntPoint>& ValidMoves, FIntPoint CandidatePoint);
	void RightAttack(TArray<FIntPoint>& ValidMoves, FIntPoint CandidatePoint);

public:
	AChessPawn();

protected:
	virtual void BeginPlay() override;

	void SelectedChessPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed) override;

	void CheckMovement(TArray<FIntPoint>& ValidMoves);

};
