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
 static AActor* ActorDetected;
 static bool AmISeeingEvidence(ADetective* Detective, const AActor* Evidence);
 static AActor* MathWho;
 static float HitEvidenceWallDistance;
 static AActor* HitEvidenceWallActor;
 static bool bEvidenceFirst;

 
public:
 static bool IsEvidenceValid(ADetective* Detective); // TODO set origin and direction of the function as reference to avoid copy objects 
 static void ConeCastTraceWithoutSweep(UWorld* World, FVector Origin, FVector Direction, float Range, float ConeAngle, ADetective* Detective);
 static FCollisionQueryParams Params;
 static FVector HitPoint;
 static AActor* Who;
 static UBoxComponent* Box;
 static TArray<TSoftObjectPtr<AActor>> IgnoredActors;
 static TArray<UStaticMesh> IgnoredSceneAssets;
 static bool bEvidenceFound;
 static bool bObstacleFound;
 
};
