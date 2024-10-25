// Fill out your copyright notice in the Description page of Project Settings.


#include "../PlayerController/Detective.h"
#include "DetectivePhotocamera.h"


// Sets default values
ADetectivePhotocamera::ADetectivePhotocamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// ZOOM
	ZoomStep = 0.1f;
	MinZoomValue = 1.0f;
	MaxZoomValue = 2.0f;
	currentZoomValue = 1.0f;
	ZoomHoldInterval = 0.1f;
	ZoomSpeed = 12.f;
	bIsZoominIn = false;
	bIsZoominOut = false;

	// MOVEMENT
	DCMoveSpeedMultiplier = 0.5f;

	bDcCameraIsActive = false;


}

// Called when the game starts or when spawned
void ADetectivePhotocamera::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADetectivePhotocamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADetectivePhotocamera::IncreaseZoom(ADetective* Detective)
{
	if (bDcCameraIsActive)
	{
		
		targetZoomValue = FMath::Clamp(currentZoomValue + ZoomStep, MinZoomValue, MaxZoomValue);
		
		currentZoomValue = FMath::FInterpTo(currentZoomValue, targetZoomValue, GetWorld()->GetDeltaSeconds(), ZoomSpeed);

		// Update FOV
		float NewFOV = 90.0f * (1.0f / currentZoomValue);
		Detective->GetCamera()->SetFieldOfView(NewFOV);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Zoom In - Current Zoom Value: %f"), currentZoomValue));
		}
	}

}

void ADetectivePhotocamera::DecreaseZoom(ADetective* Detective)
{
	if (bDcCameraIsActive)
	{
		targetZoomValue = FMath::Clamp(currentZoomValue - ZoomStep, MinZoomValue, MaxZoomValue);
		
		currentZoomValue = FMath::FInterpTo(currentZoomValue, targetZoomValue, GetWorld()->GetDeltaSeconds(), ZoomSpeed);

		// Update FOV
		float NewFOV = 90.0f * (1.0f / currentZoomValue);
		Detective->GetCamera()->SetFieldOfView(NewFOV); 

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Zoom Out - Current Zoom Value: %f"), currentZoomValue));
		}
	}

}

