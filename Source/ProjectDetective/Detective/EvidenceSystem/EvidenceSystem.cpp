// Fill out your copyright notice in the Description page of Project Settings.


#include "EvidenceSystem.h"
#include "../PlayerController/Detective.h"
#include "../Props/SceneActors/SceneActor.h"
#include "Engine/OverlapResult.h"

FVector EvidenceSystem::HitPoint;
AActor* EvidenceSystem::Who;
UBoxComponent* EvidenceSystem::Box;
bool EvidenceSystem::bActorFound;
TArray<TSoftObjectPtr<AActor>> EvidenceSystem::IgnoredActors;
bool EvidenceSystem::bEvidenceFound;
float EvidenceSystem::DistanceMinPercentage;
ASceneActor* EvidenceSystem::SceneActorDetected;
bool EvidenceSystem::bObstacleFound;
TArray<UStaticMesh> EvidenceSystem::IgnoredSceneAssets;
FCollisionQueryParams EvidenceSystem::Params;
float EvidenceSystem::HitEvidenceWallDistance;
AActor* EvidenceSystem::HitEvidenceWallActor;
AActor* EvidenceSystem::ActorDetected;
AActor* EvidenceSystem::MathWho;
bool EvidenceSystem::bEvidenceFirst;



void EvidenceSystem::ConeCastTraceWithoutSweep(UWorld* World, FVector Origin, FVector Direction, float Range, float ConeAngle, ADetective* Detective)
{
    TArray<FHitResult> HitResults;
    const int32 NumTraces = 64; 
    const float AngleIncrement = ConeAngle / NumTraces;
    TMap<AActor*, float> VisibilityScores; 
    int32 TotalHits = 0;

    for (const auto& Actor : IgnoredActors)
    {
        if (Actor.IsValid())
        {
            Params.AddIgnoredActor(Actor.Get());
        }
    }

    for (int32 i = -NumTraces / 2; i <= NumTraces / 2; ++i)
    {
        
        FRotator Rotation = FRotator(0, i * AngleIncrement, 0);
        FVector TraceDirection = Rotation.RotateVector(Direction);

        
        FHitResult HitResult;
        FVector End = Origin + TraceDirection * Range;
        bool bHit = World->LineTraceSingleByChannel(HitResult, Origin, End, ECC_Visibility, Params);

        if (bHit)
        {
            HitResults.Add(HitResult);
            TotalHits++;
            DrawDebugLine(World, Origin, End, FColor::Purple, false, 6.f);

            AActor* HitActor = HitResult.GetActor();
            if (HitActor)
            {
                float& Score = VisibilityScores.FindOrAdd(HitActor);
                Score += 1.0f / TotalHits; 
                
                if (HitActor->ActorHasTag("Evidence"))
                {
                    DrawDebugPoint(World, HitResult.ImpactPoint, 50.f, FColor::Turquoise, false, 10.0f);
                }
                else
                {
                    DrawDebugPoint(World, HitResult.ImpactPoint, 50.f, FColor::Red, false, 10.0f);
                }
            }
        }
    }

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Total Hits: %d"), TotalHits));

    // Determine the most visible evidence
    AActor* MostVisibleEvidence = nullptr;
    float HighestScore = 0.0f;
    for (auto& Elem : VisibilityScores)
    {
        AActor* Actor = Elem.Key;
        float Score = Elem.Value;
        if (Actor->ActorHasTag("Evidence") && Score > HighestScore)
        {
            HighestScore = Score;
            MostVisibleEvidence = Actor;
        }
    }

    if (MostVisibleEvidence && HighestScore >= 0.75f)
    {
        const float EvidenceLocation = (Detective->GetCamera()->GetComponentLocation() - MostVisibleEvidence->GetActorLocation()).Size();
        const float BoxTargetRadius = EvidenceLocation * 0.5f;
        const float BoxAngularSize = FMath::RadiansToDegrees(2 * FMath::Atan2(BoxTargetRadius, EvidenceLocation));
        DistanceMinPercentage = (EvidenceLocation * 75.0f) / 100.f;
        const float DistanceMaxPercentage = (EvidenceLocation * 90.0f) / 100.f;

        if (!AmISeeingEvidence(Detective, MostVisibleEvidence))
        {
            GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::White, TEXT("I'm not seeing any evidence"));
            return;
        }

        // Validate evidence considering its visibility score and angular size
        if ((BoxAngularSize <= DistanceMinPercentage + Detective->GetCamera()->FieldOfView))
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("Evidence valid"));
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, FString::Printf(TEXT("EvidenceInPhotoPercentage %f"), DistanceMinPercentage));
            bEvidenceFound = true;
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Evidence not valid"));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No evidence found"));
    }
}




#pragma region MinorDetections

bool EvidenceSystem::AmISeeingEvidence(ADetective* Detective, const AActor* Evidence)
{
	const FVector CameraForward = Detective->GetCamera()->GetForwardVector();
	const FVector Distance = Evidence->GetActorLocation() - Detective->GetCamera()->GetComponentLocation();
	const float Seen = static_cast<float>(FVector::DotProduct(CameraForward, Distance));
	return Seen >= DistanceMinPercentage;
}

bool EvidenceSystem::IsEvidenceValid(ADetective* Detective)
{
	if(Who == nullptr) // for some reason the box is always null so, I deleted it for now (need to revise it)
	{
		return false;
	}
	
	const float Fov = Detective->GetCamera()->FieldOfView;
	const float EvidenceLocation = (Detective->GetCamera()->GetComponentLocation() - Who->GetActorLocation()).Length();
	const float BoxTargetRadius = EvidenceLocation * 0.5f;
	const float BoxAngularSize = FMath::RadiansToDegrees(2 * FMath::Atan2(BoxTargetRadius, EvidenceLocation));
	DistanceMinPercentage = (EvidenceLocation * 5.0f) / 100.f;
	const float DistanceMaxPercentage = (EvidenceLocation * 75.0f) / 100.f;
	
	if(!IsEvidenceNotNull())
	{
		return false;
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, TEXT("checking a valid evidence"));

	if (DistanceMinPercentage <= BoxAngularSize && BoxAngularSize <= DistanceMaxPercentage)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("Evidence valid"));
		bActorFound = false;
		return true;
	}

	return false;
}


bool EvidenceSystem::IsEvidenceNotNull()
{
	if(Who == nullptr)
	{
		return false;
	}

	if(Who->ActorHasTag("Evidence"))
	{
		return true;
	}

	return false;
}
#pragma endregion 