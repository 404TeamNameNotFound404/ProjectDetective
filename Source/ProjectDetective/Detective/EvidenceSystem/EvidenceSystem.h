// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshActor.h"

/**
 * 
 */
class ADetective;
class ASceneActor;

class PROJECTDETECTIVE_API EvidenceSystem
{

private:
 static bool IsEvidenceNotNull();
 static bool bActorFound;
 static float DistanceMinPercentage;
 static ASceneActor* SceneActorDetected;
 static bool AmISeeingEvidence(ADetective* Detective, const AActor* Evidence);
 
public:
 static bool IsEvidenceValid(ADetective* Detective);
 static void ConeCastTrace(UWorld* World, FVector Origin, FVector Direction, float Range, float Radius, ADetective* Detective);
 static void ObstacleConeCastTrace(const UWorld* World, const FVector Origin, const FVector Direction, const float Range, const float Radius,  ADetective* Detective);
 static FCollisionQueryParams Params;
 static FVector HitPoint;
 static AActor* Who;
 static UBoxComponent* Box;
 static TArray<TSoftObjectPtr<AActor>> IgnoredActors;
 static TArray<UStaticMesh> IgnoredSceneAssets;
 static bool bEvidenceFound;
 static bool bObstacleFound;
 
};
