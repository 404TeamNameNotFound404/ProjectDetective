// Fill out your copyright notice in the Description page of Project Settings.


#include "EvidenceSystem.h"
#include "../PlayerController/Detective.h"

FVector EvidenceSystem::HitPoint;
AActor* EvidenceSystem::Who;
UBoxComponent* EvidenceSystem::Box;
bool EvidenceSystem::bActorFound;
TArray<AActor*> EvidenceSystem::IgnoredActors;
bool EvidenceSystem::bEvidenceFound;
float EvidenceSystem::DistanceMinPercentage;


void EvidenceSystem::ConeCastTrace(UWorld* World, FVector Origin, FVector Direction, float Range, float Radius, ADetective* Detective)
{
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(Radius);
	FCollisionQueryParams Params;
	TArray<FHitResult> Hits;
	const FVector End = Origin + Direction * Range;
	const float FOV = Detective->GetCamera()->FieldOfView;
	const float ConeAngleRadians = FMath::DegreesToRadians(FOV);  // The FOV defines the cone's spread

	bool bHit = World->SweepMultiByChannel(Hits, Origin, End, FQuat::Identity, ECC_Visibility, SphereShape, Params);
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
         
				if (Hit.GetActor()->ActorHasTag("Obstacle"))
				{
					break; // if collides with walls or doors or whatever it instantly cut the detection
				}
                
				if (Hit.GetActor()->ActorHasTag("Evidence"))
				{
					Box = Cast<UBoxComponent>(Hit.GetComponent());
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("Found evidence via conecast"));
					bActorFound = true;
					Who = Hit.GetActor();
				}
			} 
		}
	}

	if(bActorFound && Who != nullptr)
	{
		const float EvidenceLocation = (Detective->GetCamera()->GetComponentLocation() - Who->GetActorLocation()).Length();
		const float BoxTargetRadius = EvidenceLocation * 0.5f;
		const float BoxAngularSize = FMath::RadiansToDegrees(2 * FMath::Atan2(BoxTargetRadius, EvidenceLocation));
		DistanceMinPercentage = (EvidenceLocation * 5.0f) / 100.f;
		const float DistanceMaxPercentage = (EvidenceLocation * 75.0f) / 100.f;

		if(!AmISeeingEvidence(Detective, Who))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("I'm not seeing any evidence"));
			return;
		}
		
		if (BoxAngularSize <= DistanceMinPercentage + FOV || BoxAngularSize >= DistanceMaxPercentage + FOV)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("Evidence valid"));
			bEvidenceFound = true;
		}

		bActorFound = false;
	}
}


bool EvidenceSystem::AmISeeingEvidence(ADetective* Detective, const AActor* Evidence)
{
	const FVector CameraForward = Detective->GetCamera()->GetForwardVector();
	const FVector Distance = Evidence->GetActorLocation() - Detective->GetCamera()->GetComponentLocation();
	const float Seen = static_cast<float>(FVector::DotProduct(CameraForward, Distance));
	return Seen >= DistanceMinPercentage;
}

AActor* EvidenceSystem::FindClue(const UWorld* World, ADetective* Detective)
{
	const FVector Start = Detective->GetCamera()->GetComponentLocation() + FVector(800, 800, 800);
	const FVector Forward = Detective->GetCamera()->GetForwardVector();
	const FVector End = Start + (Forward * 300.f);
	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.OwnerTag = FName("Evidence");
	Params.AddIgnoredActors(IgnoredActors); // Add actor to ignore if detected 
	
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(Detective->RayLenght);
	DrawDebugSphere(World, Start, Detective->RayLenght, 1, FColor::Blue, false, 12.f);

	if(World->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Visibility, SphereShape, Params))
	{
		for(FHitResult Hit : Hits)
		{
			if(Hit.GetActor()->ActorHasTag("Obstacle"))
			{
				break;
			}
			
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
	
	// if(BoxAngularSize <= Fov + DistanceMinPercentage ||
	// 	BoxAngularSize >= Fov + DistanceMaxPercentage)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("Evidence valid"));
	// 	bActorFound = false;
	// 	return true;
	// }

	return false;
}

void EvidenceSystem::AddActorToIgnore(AActor* IgnoredActor)
{
	IgnoredActors.Add(IgnoredActor);
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
