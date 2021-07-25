// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Actors/ChessBoardBuilder.h"
#include "ChessGame/Actors/ChessBoardPiece.h"
#include "ChessGame/Actors/ChessPieceTypes.h"
#include "Math/IntPoint.h"

// Sets default values
AChessBoardBuilder::AChessBoardBuilder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AChessBoardBuilder::BeginPlay()
{
	Super::BeginPlay();

	GenerateChessTable();
}

// Called every frame
void AChessBoardBuilder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChessBoardBuilder::GenerateChessTable()
{
	FRotator SpawnRotation {0.f, 0.f, 0.f};
	bool bWhiteChessBoardPiecePlaced = true;
	int32 Index = 0;
	int32 NumberOfChessBoardMeshes = ChessBoardPieces.Num();
	int32 NumberOfChessBoardMaterials = ChessPieceMaterials.Num();

	if (!AreProvidedMaterialsAndMeshesOk(NumberOfChessBoardMeshes, NumberOfChessBoardMaterials))
	{
		return;
	}

	for (int32 LengthIndex = 0; LengthIndex < Length; ++LengthIndex)
	{
		Index = LengthIndex % NumberOfChessBoardMeshes;

		for (int32 WidthIndex = 0; WidthIndex < Width; ++WidthIndex)
		{
				AChessBoardPiece* TempChessBoardPiece;
				
				// Spawn the chess board piece
				TempChessBoardPiece = GetWorld()->SpawnActor<AChessBoardPiece>(
													ChessBoardPieces[Index],
													SpawnLocation,
													SpawnRotation
													);

				TempChessBoardPiece->SetOwner(this);
				TempChessBoardPiece->SetChessBoardDimensions(Length, Width);

				SpawnChessPiece(TempChessBoardPiece, WidthIndex, LengthIndex);
				SetChessPieceProperties(TempChessBoardPiece, WidthIndex, LengthIndex);

				// Create dictionary entries
				ChessBoardDict.Emplace(FIntPoint {WidthIndex, LengthIndex}, TempChessBoardPiece);

				// Update information for next chess board piece
				SpawnLocation.Y += YDistanceBetweenChessBoardPieces;
				Index += 1;
				Index %= NumberOfChessBoardMeshes;
		}

		SpawnLocation.Y = 0;
		SpawnLocation.X += XDistanceBetweenChessBoardPieces; 
	}
}

// Returns the chess board piece that coresponds to the search point
AChessBoardPiece* AChessBoardBuilder::GetChessBoardPieceAtCoords(FIntPoint SearchPoint)
{
	AChessBoardPiece** SearchedChessBoardPiece = ChessBoardDict.Find(SearchPoint);

	if (SearchedChessBoardPiece)
	{
		return *SearchedChessBoardPiece;
	}

	return nullptr;
}

bool AChessBoardBuilder::AreProvidedMaterialsAndMeshesOk(const int32& NumberOfMeshes, const int32& NumberOfMaterials)
{
	// Check to see if the materials provided are equal to the standard one
	if (NumberOfMaterials != StandardNumberOfMaterials)
	{
		UE_LOG(
				LogTemp, 
			   	Error,
			   	TEXT("Provided %d materials! Expected %d materials for chess pieces in %d teams"),
			   	NumberOfMaterials,
			   	StandardNumberOfMaterials,
				StandardNumberOfMaterials
			   );
		return false;
	}

	// Do additional checks to if we want this to be similar to a real chessboard
	if (bRealChessBoard)
	{
		if (NumberOfMeshes != StandardNumberOfMeshes)
		{
			UE_LOG(LogTemp, Error, TEXT("Provided meshes or materials do not fit a real chessboard!"));
			return false;
		}

		Length = StandardLength;
		Width = StandardWidth;
	}

	return true;
}

void AChessBoardBuilder::SpawnChessPiece(AChessBoardPiece* CurrentChessBoardPiece, const int32& XCoord, const int32& YCoord) const
{
	// Decide what type of chess piece to spawn depending on the position
	if (YCoord == 0 || YCoord == Length - 1)
	{
		if (XCoord == 1 || XCoord == Width - 2)
		{
			CurrentChessBoardPiece->SpawnChessPiece(ChessPieceTypes::Bishop);
			return;
		}

		if (XCoord == 0 || XCoord == Width - 1)
		{
			CurrentChessBoardPiece->SpawnChessPiece(ChessPieceTypes::Rook);
			return;
		}
		
		CurrentChessBoardPiece->SpawnChessPiece(ChessPieceTypes::Pawn);
		// TODO: Additional chess piece spawning depending on the XPosition on the table
		// TODO: EX. Rook, Queen, Knight etc.
		return;
	}

	if (YCoord == 1 || YCoord == Length - 2)
	{
		CurrentChessBoardPiece->SpawnChessPiece(ChessPieceTypes::Pawn);
		return;
	}
}

void AChessBoardBuilder::SetChessPieceProperties(AChessBoardPiece* CurrentChessBoardPiece, const int32& XCoord, const int32& YCoord) const
{
	// Assign to which team the chess piece attains
	if (YCoord == 0 || YCoord == 1)
	{
		// TODO: Magic number
		CurrentChessBoardPiece->SetInitialChessPieceTeam(0);
		CurrentChessBoardPiece->TellChessPieceColor(ChessPieceMaterials[0]);
	}
	else if (YCoord == Length - 2 || YCoord == Length - 1)
	{
		CurrentChessBoardPiece->SetInitialChessPieceTeam(1);
		CurrentChessBoardPiece->TellChessPieceColor(ChessPieceMaterials[1]);
	}

	// Set their coordinates
	CurrentChessBoardPiece->SetInitialChessPieceCoordinates(XCoord, YCoord);
}

int32 AChessBoardBuilder::GetChessBoardLength()
{
	return Length;
}

int32 AChessBoardBuilder::GetChessBoardWidth()
{
	return Width;
}