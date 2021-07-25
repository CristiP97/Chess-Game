#include "CoreMinimal.h"
#include "ChessPieceTypes.generated.h"

UENUM()
enum ChessPieceTypes
{
	Bishop UMETA(DisplayName = "Bishop"),
	Pawn UMETA(DisplayName = "Pawn"),
	King UMETA(DisplayName = "King"),
	Knight UMETA(DisplayName = "Knight"),
	Queen UMETA(DisplayName = "Queen"),
	Rook UMETA(DisplayName = "Rook")
};