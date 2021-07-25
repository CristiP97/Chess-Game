// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaticMeshComponent.h"
#include "ChessBoardPiece.h"
#include "ChessGame/Actors/ChessPieceBase.h"
#include "ChessGame/Actors/ChessPieceTypes.h"
#include "ChessGame/Actors/ChessBoardBuilder.h"
#include "ChessGame/ChessGameGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChessBoardPiece::AChessBoardPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ChessBoardPieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chess Board Piece Mesh"));
	RootComponent = ChessBoardPieceMesh;

	
	AvailableSelectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selection Piece Mesh"));
	AvailableSelectionMesh->SetupAttachment(ChessBoardPieceMesh);

	GameModeRef = Cast<AChessGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	
	ChessBoardPieceMesh->OnClicked.AddDynamic(this, &AChessBoardPiece::SelectChessBoardPiece);
}

// Called when the game starts or when spawned
void AChessBoardPiece::BeginPlay()
{
	Super::BeginPlay();

	// Hide the selection mesh
	AvailableSelectionMesh->ToggleVisibility(false);
}

// Called every frame
void AChessBoardPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChessBoardPiece::SpawnChessPiece(ChessPieceTypes NameOfChessPiece)
{
	// Error protection
	if (ChessPieces.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Did not set what kind of chess pieces can be spawned"));
		return;
	}

	int32 Index;
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	Index = GetChessPieceIndex(NameOfChessPiece);

	AChessPieceBase* TempChessPiece = GetWorld()->SpawnActor<AChessPieceBase>(
												ChessPieces[Index],
												SpawnLocation,
												SpawnRotation
												);
	TempChessPiece->SetOwner(this);
	ActiveChessPiece = TempChessPiece;
}

void AChessBoardPiece::SetInitialChessPieceTeam(const int32& Team) const
{
	if (ActiveChessPiece)
	{
		ActiveChessPiece->SetTeam(Team);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to assign a team to a non existent chess piece!"));
	}
}

// Returns index based on the type of chess piece
int AChessBoardPiece::GetChessPieceIndex(ChessPieceTypes NameOfChessPiece) const
{
	switch (NameOfChessPiece)
	{
		case ChessPieceTypes::Pawn:
			return 0;
		case ChessPieceTypes::Bishop:
			return 1;
		case ChessPieceTypes::King:
			return 2;
		case ChessPieceTypes::Knight:
			return 3;
		case ChessPieceTypes::Queen:
			return 4;
		case ChessPieceTypes::Rook:
			return 5;
		default:
			return 0;
	}
}

// TODO: Make so you click through other actors so you do not accidentally selec other chess pieces
// TODO: It's possible to accidentally click another piece while moving
void AChessBoardPiece::SelectChessBoardPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed)
{
	AActor* TempChessPiece =  GameModeRef->GetSelectedChessPiece();
	
	// If we do not have a selected chess piece return
	if (!TempChessPiece)
	{
		UE_LOG(LogTemp, Warning, TEXT("No chess piece selected!"));
		return;
	}

	// If we select the same chess piece, deselect it
	if (TempChessPiece && TempChessPiece == ActiveChessPiece)
	{
		// Reset selected piece
		GameModeRef->ResetActiveChessPiece();
		UE_LOG(LogTemp, Warning, TEXT("Pressed on the same piece so we reset!"));
		return;
	}

	// Stop the player from executing invalid moves
	if (TempChessPiece && !bIsValidMoveSelection)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot move there!"));
		return;
	}

	if (TempChessPiece && TempChessPiece != ActiveChessPiece)
	{
		// Cut ties with the old position
		Cast<AChessBoardPiece>(TempChessPiece->GetOwner())->ResetActiveChessPiece();

		// Delete enemy piece if this was an attack
		if (ActiveChessPiece)
		{
			ActiveChessPiece->Destroy();
			ActiveChessPiece = nullptr;
		}

		// Update logical position
		ActiveChessPiece = Cast<AChessPieceBase>(TempChessPiece);
		FVector ChessPieceLocation = ActiveChessPiece->GetActorLocation();
		ActiveChessPiece->SetChessPieceCoords(XCoord, YCoord);
		ActiveChessPiece->SetOwner(this);

		// Update position in world
		ChessPieceLocation.X = GetActorLocation().X;
		ChessPieceLocation.Y = GetActorLocation().Y;
		// ActiveChessPiece->SetActorLocation(ChessPieceLocation);
		ActiveChessPiece->StartMovement(ChessPieceLocation);

		// Reset selected piece
		GameModeRef->ResetActiveChessPiece();

		// Notify that a move has happened
		GameModeRef->PlayerMoved();

		// Check if the game has ended
		CheckGameEnded();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to click without have an active chess piece selected!"));
	}
}


void AChessBoardPiece::SetInitialChessPieceCoordinates(const int32& _XCoord, const int32& _YCoord)
{
	if (ActiveChessPiece)
	{
		ActiveChessPiece->SetChessPieceCoords(_XCoord, _YCoord);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s doesn't have a chess piece to set initial coords!"), *GetName());
	}

	XCoord = _XCoord;
	YCoord = _YCoord;
}

void AChessBoardPiece::CheckGameEnded() const
{
	if (ActiveChessPiece->GetTeam() == 0 && YCoord == Length - 1)
	{
		GameModeRef->PlayerWon(ActiveChessPiece->GetTeam());
	}

	if (ActiveChessPiece->GetTeam() == 1 && YCoord == 0)
	{
		GameModeRef->PlayerWon(ActiveChessPiece->GetTeam());
	}
}

void AChessBoardPiece::TellChessPieceColor(UMaterialInstance* Material)
{
	if (ActiveChessPiece)
	{
		ActiveChessPiece->SetChessPieceMaterial(Material);
	}
}

void AChessBoardPiece::SetChessBoardDimensions(const int32& _Length, const int32& _Width)
{
	Length = _Length;
	Width = _Width;
}

bool AChessBoardPiece::IsBeyondHalf()
{
	if (GameModeRef->IsWhiteTurn() && YCoord >= Length / 2)
	{
		return true;
	}

	if (!GameModeRef->IsWhiteTurn() && YCoord < Length / 2)
	{
		return true;
	}

	return false;
}

AChessPieceBase* AChessBoardPiece::GetActiveChessPiece() const
{
	return ActiveChessPiece;
}

void AChessBoardPiece::ResetActiveChessPiece()
{
	ActiveChessPiece = nullptr;
}

void AChessBoardPiece::HideAvailableSelection()
{
	AvailableSelectionMesh->ToggleVisibility(false);
}

void AChessBoardPiece::ShowAvailableSelection()
{
	AvailableSelectionMesh->ToggleVisibility(true);
}

void AChessBoardPiece::SetValidMoveSelection(const bool& bIsValid)
{
	bIsValidMoveSelection = bIsValid;
}




