// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessBoardBuilder.generated.h"

class AChessBoardPiece;
struct FIntPoint;

UCLASS()
class CHESSGAME_API AChessBoardBuilder : public AActor
{
	GENERATED_BODY()

private:
	// Holds the meshes for the chess board pieces
	// Should only be 2 for real chessboard
	UPROPERTY(EditDefaultsOnly, Category = "Chess Board Mesh");
	TArray<TSubclassOf<AChessBoardPiece>> ChessBoardPieces;
	UPROPERTY(VisibleDefaultsOnly, Category = "Chess Board Mesh");
	int32 StandardNumberOfMeshes = 2;

	// Holds materials for the chess pieces place on the chess board piece
	// These are the white and black materials for the Pawn, Rook, Queen, etc.
	// Should only be 2 for real chessboard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess Board Materials", meta = (AllowPrivateAccess = "true"))
	TArray<UMaterialInstance*> ChessPieceMaterials;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Chess Board Materials", meta = (AllowPrivateAccess = "true"))
	int32 StandardNumberOfMaterials = 2;

	// Distance between chess board pieces on the X axis
	UPROPERTY(EditAnywhere, Category = "Chess Board Build Info")
	float XDistanceBetweenChessBoardPieces = 100.f;
	// Distance between chess board pieces on the Y axis
	UPROPERTY(EditAnywhere, Category = "Chess Board Build Info")
	float YDistanceBetweenChessBoardPieces = 100.f;
	
	// Origin in the world from where to start building the chess board
	UPROPERTY(EditAnywhere, Category = "Chess Board Build Info")
	FVector SpawnLocation {0.f, 0.f, 0.f};

	// Chessboard size
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chess Board Build Info", meta = (AllowPrivateAccess = "true"))
	int32 Length = 8;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chess Board Build Info", meta = (AllowPrivateAccess = "true"))
	int32 Width = 8;

	// Used for making checks to build a standard, real chess board
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Chess Board Build Info", meta = (AllowPrivateAccess = "true"))
	bool bRealChessBoard = true;

	//Standard chessboard size
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Chess Board Build Info", meta = (AllowPrivateAccess = "true"))
	int32 StandardLength = 8;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Chess Board Build Info", meta = (AllowPrivateAccess = "true"))
	int32 StandardWidth = 8;

	// Store the chess board pieces with their coordinates in the world
	TMap<FIntPoint, AChessBoardPiece*> ChessBoardDict;
	
	
	void GenerateChessTable();
	bool AreProvidedMaterialsAndMeshesOk(const int32& NumberOfMeshes, const int32& NumberOfMaterials);
	void SpawnChessPiece(AChessBoardPiece* CurrentChessBoardPiece, const int32& XCoord, const int32& YCoord) const;
	void SetChessPieceProperties(AChessBoardPiece* CurrentChessBoardPiece, const int32& XCoord, const int32& YCoord) const;

public:	
	// Sets default values for this actor's properties
	AChessBoardBuilder();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Returns the AChessBoardPiece corresponding to the coordinates received
	AChessBoardPiece* GetChessBoardPieceAtCoords(FIntPoint SearchPoint);
	int32 GetChessBoardLength();
	int32 GetChessBoardWidth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
