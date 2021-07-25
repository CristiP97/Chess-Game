// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessPieceBase.generated.h"

class UStaticMeshComponent;
class AChessGameGameModeBase;
class AChessBoardBuilder;
class AChessBoardPiece;

UCLASS()
class CHESSGAME_API AChessPieceBase : public AActor
{
	GENERATED_BODY()

private:
	void ApplyInitialOffset();
	AChessBoardBuilder* GetChessBoardBuilder();

	int32 MaterialCounter = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ChessPieceMesh;

	UPROPERTY(VisibleAnyWhere, Category = "Chess Board Position")
	bool bSetInitialCoords = false;


public:	
	// Sets default values for this actor's properties
	AChessPieceBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetTeam(int32 Team);
	void SetChessPieceCoords(int32 A, int32 B);
	int32 GetTeam();
	void ClearAvailableSelection();
	void SetChessPieceMaterial(UMaterialInstance* Material);
	void StartMovement(FVector TargetLocation);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void SelectedChessPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed);

	virtual void Move(float DeltaTime);


	UPROPERTY(EditAnyWhere, Category = "SpawnInfo")
	float InitialPlacementZOffset = 50.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	AChessGameGameModeBase* GameModeRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	AChessBoardBuilder* ChessBoardBuilderRef;

	UPROPERTY(VisibleAnywhere, Category = "Chess Piece Info")
	int32 ChessPieceTeam = -1;
	
	UPROPERTY(VisibleAnyWhere, Category = "Chess Board Position")
	int32 XCoord = 0;
	
	UPROPERTY(VisibleAnyWhere, Category = "Chess Board Position")
	int32 YCoord = 0;

	UPROPERTY(EditAnyWhere, Category = "Move Speed")
	float ChessPieceMoveSpeed = 5.f;

	bool bIsMoving = false;
	bool bTargetReached = true;
	FVector NewTargetLocation;

    TArray<AChessBoardPiece*> ValidChessBoardPieces;

};