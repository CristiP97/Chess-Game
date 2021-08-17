// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GH_TestActor.h"

// Sets default values
AGH_TestActor::AGH_TestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGH_TestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGH_TestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

