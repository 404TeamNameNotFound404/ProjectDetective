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
 
public:
 static AActor* FindClue(const UWorld* World, ADetective* Detective);
 static bool IsEvidenceValid(ADetective* Detective);
 static FVector HitPoint;
 static AActor* Who;
 static UBoxComponent* Box;
 static bool bActorFound;
};
