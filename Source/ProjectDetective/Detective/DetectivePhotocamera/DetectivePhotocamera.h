// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DetectivePhotocamera.generated.h"

class ADetective;
UCLASS()
class PROJECTDETECTIVE_API ADetectivePhotocamera : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADetectivePhotocamera();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// ZOOM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Photocamera")
	float ZoomStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Photocamera")
	float MaxZoomValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Photocamera")
	float MinZoomValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Photocamera")
	float ZoomHoldInterval;

	UPROPERTY()
	float currentZoomValue;

	UPROPERTY()
	bool bIsZoominIn;

	UPROPERTY()
	bool bIsZoominOut;

	UPROPERTY()
	float ZoomTimer;

	UPROPERTY()
	float ZoomSpeed;

	UPROPERTY()
	float targetZoomValue;

	


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photocamera")
	bool bDcCameraIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photocamera")
	bool bCanTakePhoto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photocamera")
	bool bCanInteract;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photocamera")
	float DCMoveSpeedMultiplier;

	/// <summary>
	///  to save the current zoom fort when u switch back to DC;
	///  DON'T USE IT 
	/// </summary>
	UPROPERTY()	
	float LastFOV; 

	// ZOOM
	void IncreaseZoom(ADetective* Detective);
	void DecreaseZoom(ADetective* Detective);




};
