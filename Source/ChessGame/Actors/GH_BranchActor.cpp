// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GH_BranchActor.h"

// Sets default values
AGH_BranchActor::AGH_BranchActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGH_BranchActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGH_BranchActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

