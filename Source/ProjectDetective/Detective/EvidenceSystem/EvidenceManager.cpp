// Fill out your copyright notice in the Description page of Project Settings.


#include "EvidenceManager.h"
#include "EvidenceSystem.h"

// Sets default values
AEvidenceManager::AEvidenceManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SkippedActors.Empty();
}

// Called when the game starts or when spawned
// Called when the game starts or when spawned
void AEvidenceManager::BeginPlay()
{
	Super::BeginPlay();

	// Log the contents of ActorsToIgnore before doing anything else
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("dim actors to ignore BEFORE: %d"), SkippedActors.Num()));

	EvidenceSystem::IgnoredActors = SkippedActors;

	for (const auto& Actor : SkippedActors)
	{
		if (Actor.IsValid())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
				FString::Printf(TEXT("Actor in ActorsToIgnore: %s"), *Actor->GetName()));
		}
	}
	
}







