// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/PlayerControllers/PlayerControllerBase.h"

void APlayerControllerBase::SetControllerEnabledState(bool EnabledState)
{
    if (EnabledState)
    {
        GetPawn()->EnableInput(this);
    }
    else
    {
        GetPawn()->DisableInput(this);
    }

    bShowMouseCursor = EnabledState;
}