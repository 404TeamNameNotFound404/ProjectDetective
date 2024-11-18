// Fill out your copyright notice in the Description page of Project Settings.


#include "EvidenceSystem.h"
#include "../PlayerController/Detective.h"
#include "../Props/SceneActors/SceneActor.h"

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

void EvidenceSystem::ConeCastTrace(UWorld* World, FVector Origin, FVector Direction, float Range, float Radius, ADetective* Detective)
{
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(Radius);
	FCollisionQueryParams Params1;
	TArray<FHitResult> Hits;
	const FVector End = Origin + Direction * Range;
	const float FOV = Detective->GetCamera()->FieldOfView;
	const float ConeAngleRadians = FMath::DegreesToRadians(FOV * Detective->ConeSize);  // The FOV defines the cone's spread
	//Params.AddIgnoredActors(IgnoredActors);
	bool bHit = World->SweepMultiByChannel(Hits, Origin, End, FQuat::Identity, ECC_GameTraceChannel1, SphereShape, Params1);
	DrawDebugLine(World, Origin, End, FColor::Purple, false, 6.f);

	if (bHit)
	{
		for (const FHitResult& Hit : Hits)
		{
			FVector HitDirection = Hit.ImpactPoint - Origin;
			HitDirection.Normalize();
			const float Angle = FMath::Acos(FVector::DotProduct(HitDirection, Direction));
			
			if (Angle <= ConeAngleRadians)
			{
				DrawDebugLine(World, Origin, Hit.ImpactPoint, FColor::Orange, false, 6.f);
				
				if (Hit.GetActor()->ActorHasTag("Evidence"))
				{
					Box = Cast<UBoxComponent>(Hit.GetComponent());
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("Found evidence via conecast"));
					bActorFound = true;
					Who = Hit.GetActor();
					break;
				}
			} 
		}
	}

	if(bActorFound && Who != nullptr)
	{
		const float EvidenceLocation = (Detective->GetCamera()->GetComponentLocation() - Who->GetActorLocation()).Length();
		const float BoxTargetRadius = EvidenceLocation * 0.5f;
		const float BoxAngularSize = FMath::RadiansToDegrees(2 * FMath::Atan2(BoxTargetRadius, EvidenceLocation));
		DistanceMinPercentage = (EvidenceLocation * 75.0f) / 100.f;
		const float DistanceMaxPercentage = (EvidenceLocation * 90.0f) / 100.f;

		if(!AmISeeingEvidence(Detective, Who) || bObstacleFound)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("I'm not seeing any evidence"));
			bObstacleFound = false;
			return;
		}
		
		if (BoxAngularSize <= DistanceMinPercentage + FOV || BoxAngularSize >= DistanceMaxPercentage + FOV)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("Evidence valid"));
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, FString::Printf(TEXT("EvidenceInPhotoPercentage %f"), DistanceMinPercentage));
			bEvidenceFound = true;
		}

		bActorFound = false;
	}
}

void EvidenceSystem::ObstacleConeCastTrace(const UWorld* World, const FVector Origin, const FVector Direction, const float Range, const float Radius,  ADetective* Detective)
{
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(Radius);
	TArray<FHitResult> Hits;
	const FVector End = Origin + Direction * Range;
	const float FOV = Detective->GetCamera()->FieldOfView;
	const float ConeAngleRadians = FMath::DegreesToRadians(FOV * Detective->ConeSize);  // The FOV defines the cone's spread

	for(const auto& Actor : IgnoredActors)
	{
		if(Actor.IsValid())
		{
			Params.AddIgnoredActor(Actor.Get());
		}
	}
	
	bool bHit = World->SweepMultiByChannel(Hits, Origin, End, FQuat::Identity, ECC_Visibility, SphereShape, Params);
	DrawDebugLine(World, Origin, End, FColor::Silver, false, 6.f);
	
	if(bHit)
	{
		for (const FHitResult& Hit : Hits)
		{
			FVector HitDirection = Hit.ImpactPoint - Origin;
			HitDirection.Normalize();
			const float Angle = FMath::Acos(FVector::DotProduct(HitDirection, Direction));
			
			if (Angle <= ConeAngleRadians)
			{
				DrawDebugLine(World, Origin, Hit.ImpactPoint, FColor::Silver, false, 6.f);

				if(Hit.GetActor()->ActorHasTag("Evidence"))
				{
					continue;
				}
				
				SceneActorDetected = Cast<ASceneActor>(Hit.GetActor());
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("Found a wall via conecast"));
				bObstacleFound = true;
				break;
			} 
		}
	}

	if(SceneActorDetected != nullptr && bObstacleFound)
	{
		const float ObstacleLocation = (Detective->GetCamera()->GetComponentLocation() - SceneActorDetected->GetActorLocation()).Length();
		const float BoxTargetRadius = ObstacleLocation * 0.5f;
		const float BoxAngularSize = FMath::RadiansToDegrees(2 * FMath::Atan2(BoxTargetRadius, ObstacleLocation));
		//const float Threshold = (ObstacleLocation * 5.f) / 100.f;

		if(ObstacleLocation < DistanceMinPercentage)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, TEXT("hitting evidence first"));
			bObstacleFound = false;
		}
		
		if(BoxAngularSize > DistanceMinPercentage)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("hitting wall first"));
		}
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