// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessBoardPiece.generated.h"

class UStaticMeshComponent;
class AChessPieceBase;
class AChessGameGameModeBase;
enum ChessPieceTypes;

UCLASS()
class CHESSGAME_API AChessBoardPiece : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ChessBoardPieceMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AvailableSelectionMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Chess Piece");
	TArray<TSubclassOf<AChessPieceBase>> ChessPieces;

	UPROPERTY(VisibleInstanceOnly, Category = "Chess Board Position")
	int32 XCoord = 0;
	UPROPERTY(VisibleInstanceOnly, Category = "Chess Board Position")
	int32 YCoord = 0;

	AChessPieceBase* ActiveChessPiece;
	AChessGameGameModeBase * GameModeRef;

	int32 Length = 0;
	int32 Width = 0;

	bool bIsValidMoveSelection = false;

	int32 GetChessPieceIndex(ChessPieceTypes NameOfChessPiece) const;
	void CheckGameEnded() const;

	UFUNCTION()
	void SelectChessBoardPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed);

public:	
	// Sets default values for this actor's properties
	AChessBoardPiece();
	void SpawnChessPiece(ChessPieceTypes NameOfChessPiece);
	void SetInitialChessPieceTeam(const int32& Team) const;
	void SetInitialChessPieceCoordinates(const int32& XCoord, const int32& YCoord);
	AChessPieceBase* GetActiveChessPiece() const;
	void ResetActiveChessPiece();

	// Hides the static mesh that indicates the available moves to the player
	void HideAvailableSelection();
	// Shows the static mesh that indicates the available moves to the player
	void ShowAvailableSelection();

	void SetValidMoveSelection(const bool& bIsValid);
	void TellChessPieceColor(UMaterialInstance* Material);
	void SetChessBoardDimensions(const int32& _Length, const int32& _Width);
	bool IsBeyondHalf();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
