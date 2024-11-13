// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

/**
 * 
 */
class ADetective;
class PROJECTDETECTIVE_API EvidenceSystem
{

private:
 static bool IsEvidenceNotNull();
 static TArray<AActor*> IgnoredActors;
 static bool bActorFound;
 static float DistanceMinPercentage;
 static bool AmISeeingEvidence(ADetective* Detective, const AActor* Evidence);
 
public:
 static AActor* FindClue(const UWorld* World, ADetective* Detective);
 static bool IsEvidenceValid(ADetective* Detective);
 static void AddActorToIgnore(AActor* IgnoredActor);
 static void ConeCastTrace(UWorld* World, FVector Origin, FVector Direction, float Range, float Radius, ADetective* Detective);
 static FVector HitPoint;
 static AActor* Who;
 static UBoxComponent* Box;
 static bool bEvidenceFound;
};
