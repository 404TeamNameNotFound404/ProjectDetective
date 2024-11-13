// Fill out your copyright notice in the Description page of Project Settings.


#include "EvidenceSystem.h"
#include "../PlayerController/Detective.h"

FVector EvidenceSystem::HitPoint;
AActor* EvidenceSystem::Who;
UBoxComponent* EvidenceSystem::Box;
bool EvidenceSystem::bActorFound;

AActor* EvidenceSystem::FindClue(const UWorld* World, ADetective* Detective)
{
	const FVector Start = Detective->GetCamera()->GetComponentLocation();
	const FVector Forward = Detective->GetCamera()->GetForwardVector();
	const FVector End = Start + (Forward * 300.f);
	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.OwnerTag = FName("Evidence");
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(Detective->RayLenght);
	DrawDebugSphere(World, Start, Detective->RayLenght, 1, FColor::Blue, false, 12.f);

	if(World->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Visibility, SphereShape, Params))
	{
		for(FHitResult Hit : Hits)
		{
			if(Hit.GetActor()->ActorHasTag("Evidence"))
			{
				Box = Cast<UBoxComponent>(Hit.GetComponent());
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("found evidence"));
				bActorFound = true;
				return Who = Hit.GetActor();
			}
		}
	}

	return nullptr;
}

bool EvidenceSystem::IsEvidenceValid(ADetective* Detective)
{
	if(Who == nullptr) // for some reason the box is always null so, I deleted it for now (need to revise it)
	{
		return false;
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, TEXT("checking a valid evidence"));
	
	const float Fov = Detective->GetCamera()->FieldOfView;
	const float EvidenceLocation = (Detective->GetCamera()->GetComponentLocation() - Who->GetActorLocation()).Length();
	const float BoxTargetRadius = EvidenceLocation * 0.5f;
	const float BoxAngularSize = FMath::RadiansToDegrees(2 * FMath::Atan2(BoxTargetRadius, EvidenceLocation));
	const float DistanceMinPercentage = (EvidenceLocation * 5.0f) / 100.f;
	const float DistanceMaxPercentage = (EvidenceLocation * 75.0f) / 100.f;

	if(IsEvidenceNotNull())
	{
		return false;
	}
	
	if(BoxAngularSize <= Fov + DistanceMinPercentage ||
		BoxAngularSize >= Fov + DistanceMaxPercentage)
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
