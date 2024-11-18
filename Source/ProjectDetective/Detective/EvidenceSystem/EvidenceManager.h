// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "EvidenceManager.generated.h"

UCLASS(Blueprintable)
class PROJECTDETECTIVE_API AEvidenceManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEvidenceManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Evidence System")
	TArray<TSoftObjectPtr<AActor>> SkippedActors;
	//TArray<TSoftObjectPtr<AActor>> ActorsToIgnore;
	
};
