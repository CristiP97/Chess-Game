// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaticMeshComponent.h"
#include "ChessGame/Actors/ChessPieceBase.h"
#include "ChessGame/ChessGameGameModeBase.h"
#include "ChessGame/Actors/ChessBoardBuilder.h"
#include "ChessGame/Actors/ChessBoardPiece.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChessPieceBase::AChessPieceBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChessPieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chess Piece Mesh"));
	RootComponent = ChessPieceMesh;
	
    ChessPieceMesh->OnClicked.AddDynamic(this, &AChessPieceBase::SelectedChessPiece);

	GameModeRef = Cast<AChessGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called when the game starts or when spawned
void AChessPieceBase::BeginPlay()
{
	Super::BeginPlay();

	ChessBoardBuilderRef = GetChessBoardBuilder();
	
	ApplyInitialOffset();
}


// Called every frame
void AChessPieceBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bTargetReached)
	{
		Move(DeltaTime);
	}

	if (bWinnerChessPiece)
	{
		RotateWinnerPiece(DeltaTime);
	}
}

void AChessPieceBase::SetTeam(int32 Team)
{
	if (ChessPieceTeam == -1)
	{
		ChessPieceTeam = Team;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to assign a team to a chess piece that already has a team!"));
	}
}

void AChessPieceBase::ApplyInitialOffset()
{
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z += InitialPlacementZOffset;
	SetActorLocation(CurrentLocation);
}

void AChessPieceBase::SelectedChessPiece(UPrimitiveComponent* touchedComponent, FKey buttonPressed)
{

}

void AChessPieceBase::SetChessPieceCoords(int32 _XCoord, int32 _YCoord)
{
	XCoord = _XCoord;
	YCoord = _YCoord;
}

AChessBoardBuilder* AChessPieceBase::GetChessBoardBuilder()
{
	TArray<AActor*> ChessBoardBuilderActor;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChessBoardBuilder::StaticClass(), ChessBoardBuilderActor);

	// Should have only one builder in the world
	if (ChessBoardBuilderActor.Num() > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Multiple chess board builders in the world!"))
		return nullptr;
	}
	else if (ChessBoardBuilderActor.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No chess board builders in the world!"))
		return nullptr;
	}

	return Cast<AChessBoardBuilder>(ChessBoardBuilderActor[0]);
}

int32 AChessPieceBase::GetTeam()
{
	return ChessPieceTeam;
}

void AChessPieceBase::ClearAvailableSelection()
{
	for (AChessBoardPiece* CurrentBoardPiece : ValidChessBoardPieces)
	{
		CurrentBoardPiece->HideAvailableSelection();
		CurrentBoardPiece->SetValidMoveSelection(false);
	}

	ValidChessBoardPieces.Empty();
}

void AChessPieceBase::SetChessPieceMaterial(UMaterialInstance* Material)
{
	ChessPieceMesh->SetMaterial(MaterialCounter++, Material);
}

void AChessPieceBase::StartMovement(FVector TargetLocation)
{
	if (!bIsMoving)
	{
		bIsMoving = true;
		NewTargetLocation = TargetLocation;
		bTargetReached = false;
	}
}

void AChessPieceBase::Move(float DeltaTime)
{
	FVector Location = GetActorLocation();
	Location = FMath::Lerp(Location, NewTargetLocation, ChessPieceMoveSpeed * DeltaTime);
	SetActorLocation(Location);

	if (FVector::Distance(Location, NewTargetLocation) < 0.1f)
	{
		bTargetReached = true;
		bIsMoving = false;
	}
}

void AChessPieceBase::SetWinnerChessPiece()
{
	bWinnerChessPiece = true;
}

void AChessPieceBase::RotateWinnerPiece(float DeltaTime)
{
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Add(0.f, DeltaTime * ChessRotateSpeed, 0.f);
	SetActorRotation(CurrentRotation);

	if (!bWinnerPosition)
	{
		if (!bIsMoving)
		{
			bWinnerPosition = true;
			FVector TargetLocation = GetActorLocation();
			TargetLocation.Z += InitialPlacementZOffset;
			StartMovement(TargetLocation);
		}
	}
}

void AChessPieceBase::CheckRepeatedMovement(TArray<FIntPoint>& ValidMoves, FIntPoint CandidatePoint, FIntPoint Directions)
{
	AChessBoardPiece* CandidateChessBoardPiece;
    
    do
    {
        CandidatePoint.X += Directions.X;
        CandidatePoint.Y += Directions.Y;

        CandidateChessBoardPiece = ChessBoardBuilderRef->GetChessBoardPieceAtCoords(CandidatePoint);

        if (CandidateChessBoardPiece)
        {
            AChessPieceBase* ChessPiece = CandidateChessBoardPiece->GetActiveChessPiece();

            if (!ChessPiece || (ChessPiece->GetTeam() != ChessPieceTeam))
            {
                ValidMoves.Emplace(CandidatePoint);
                ValidChessBoardPieces.Emplace(CandidateChessBoardPiece);
                CandidateChessBoardPiece->ShowAvailableSelection();
                CandidateChessBoardPiece->SetValidMoveSelection(true);
            }
            
            if (ChessPiece)
            {
                break;
            }
        }
    } 
    while (CandidateChessBoardPiece);
}

void AChessPieceBase::CheckSingleMovement(TArray<FIntPoint>& ValidMoves, FIntPoint CandidatePoint, FIntPoint Directions)
{
	AChessBoardPiece* CandidateChessBoardPiece;
    
    CandidatePoint.X += Directions.X;
    CandidatePoint.Y += Directions.Y;

    CandidateChessBoardPiece = ChessBoardBuilderRef->GetChessBoardPieceAtCoords(CandidatePoint);

    if (CandidateChessBoardPiece)
    {
        AChessPieceBase* ChessPiece = CandidateChessBoardPiece->GetActiveChessPiece();

        if (!ChessPiece || (ChessPiece->GetTeam() != ChessPieceTeam))
        {
            ValidMoves.Emplace(CandidatePoint);
            ValidChessBoardPieces.Emplace(CandidateChessBoardPiece);
            CandidateChessBoardPiece->ShowAvailableSelection();
            CandidateChessBoardPiece->SetValidMoveSelection(true);
        }
    }
}

bool AChessPieceBase::IsMyTeamsTurn()
{
	if (GameModeRef->IsWhiteTurn() && ChessPieceTeam == 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("Can't select black pieces on first player turn"));
        return false;
    }

    if (!GameModeRef->IsWhiteTurn() && ChessPieceTeam == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Can't select white pieces on second player turn"));
        return false;
    }

	return true;
}

void AChessPieceBase::CheckMovement(TArray<FIntPoint>& ValidMoves)
{
	
}