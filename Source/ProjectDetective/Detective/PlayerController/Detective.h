// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectDetective/Detective/BaseClass/DBaseClass.h"
#include "../Controller/DetectiveEnhancedInput.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Detective.generated.h"

UCLASS(Blueprintable)
class PROJECTDETECTIVE_API ADetective : public ADBaseClass
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADetective();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Movement")
	float MouseSensibility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float WalkHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float PlayerWidth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float WalkBobbing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float CrouchHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float CrouchSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float CrouchBobbing;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsWalking;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Body", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Body" ,meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Body", meta = (AllowPrivateAccess = "true"))
	UDetectiveInputDataAsset* InputDataAsset;

	UPROPERTY()
	FVector OldCameraLocation;
	
	UPROPERTY()
	FVector CrouchLocation;

	UPROPERTY()
	FVector DirectionMovement;
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Crouching(const FInputActionValue& InputActionValue);
	void Input_Standing(const FInputActionValue& InputActionValue);
	
	void SimulateHeadBobbing();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
