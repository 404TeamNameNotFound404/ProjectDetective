// Fill out your copyright notice in the Description page of Project Settings.


#include "Evidence.h"


// Sets default values
AEvidence::AEvidence()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	Tags.Add("Evidence");
}

// Called when the game starts or when spawned
void AEvidence::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEvidence::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

