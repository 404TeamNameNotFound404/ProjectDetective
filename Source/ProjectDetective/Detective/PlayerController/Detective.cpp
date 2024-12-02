// Fill out your copyright notice in the Description page of Project Settings.


#include "Detective.h"

#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Controller/Tags/DetectiveGameplayTags.h"
#include "ProjectDetective/Detective/Controller/DetectiveController.h"
#include "../EvidenceSystem/EvidenceSystem.h"


// Sets default values
ADetective::ADetective()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	WalkSpeed = 500.0f;
	MouseSensibility = 0.2f;
	WalkHeight = 200.0f;
	PlayerWidth = 90.0f;
	WalkBobbing = 0.0f;
	CrouchHeight = 100.0f;
	CrouchSpeed = 250.0f;
	CrouchBobbing = 0.0f;
	bCrouching = false;
	bIsWalking = false;
	CrouchToggle = 0;
	CrouchLerpSpeed = 6.0f;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(PlayerWidth);
	GetCapsuleComponent()->SetCapsuleSize(PlayerWidth, WalkHeight);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 55.0f, 0.0f);
	CameraBoom->bUsePawnControlRotation = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 190.0f, 0.0f);
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;


	
}

// Called when the game starts or when spawned
void ADetective::BeginPlay()
{
	Super::BeginPlay();
	DefaultWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCapsuleComponent()->SetCapsuleRadius(PlayerWidth);
	GetCapsuleComponent()->SetCapsuleSize(PlayerWidth, WalkHeight);
	Camera->FieldOfView = 90.0f;
	OldCameraLocation = Camera->GetRelativeLocation();

	DetectivePhotocamera->LastFOV = Camera->FieldOfView;
}

// Called every frame
void ADetective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DirectionMovement.X == 0.0f || DirectionMovement.Y == 0.0f)
	{
		bIsWalking = false;
	}

	if(!bCrouching)
	{
		const FVector CurrentLocation = Camera->GetRelativeLocation();
		const FVector TargetLocation = OldCameraLocation; 
		const FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), CrouchLerpSpeed);

		Camera->SetRelativeLocation(NewLocation);
		
		if (NewLocation.Equals(TargetLocation, 1.0f)) 
		{
			GetCapsuleComponent()->SetCapsuleHalfHeight(WalkHeight);
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
}



// INPUT  & MOVEMENT
#pragma region INPUT & MOVEMENT
void ADetective::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputDataAsset, TEXT("Forgot to add data asset to Blueprint"));
	ULocalPlayer* LocalPlayer = GetController<ADetectiveController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	checkf(SubSystem, TEXT("Subsystem not found"));
	SubSystem->AddMappingContext(InputDataAsset->DefaultMappingContext, 0);
	UDetectiveEnhancedInput* LastChecked = CastChecked<UDetectiveEnhancedInput>(PlayerInputComponent);
	LastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	LastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	LastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouching);
	//TODO DELETE THIS FUNCTIONLastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_Crouch, ETriggerEvent::Completed, this, &ThisClass::Input_Standing);
	LastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_PhotocameraZoomIn, ETriggerEvent::Triggered, this, &ThisClass::Input_PhotocameraZoomIn);
	LastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_PhotocameraZoomOut, ETriggerEvent::Triggered, this, &ThisClass::Input_PhotocameraZoomOut);
	LastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_PhotocameraActive, ETriggerEvent::Triggered, this, &ThisClass::Input_PhotocameraActive);
	LastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_TakePhoto, ETriggerEvent::Triggered, this, &ThisClass::Input_TakePhoto);


}

void ADetective::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2d Direction = InputActionValue.Get<FVector2d>();
	DirectionMovement = FVector::ZeroVector;

	const FRotator Rotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (Direction.Y != 0.f)
	{
		const FVector ForwardDirection = Rotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, Direction.Y);
		DirectionMovement.Y = Direction.Y;
		bIsWalking = true;
	}

	if (Direction.X != 0.f)
	{
		const FVector RightDirection = Rotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, Direction.X);
		DirectionMovement.X = Direction.X;
		bIsWalking = true;
	}

}

void ADetective::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2d LookAxisVector = InputActionValue.Get<FVector2d>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X * MouseSensibility);
	}

	if (LookAxisVector.Y != 0.f)
	{
		const float Pitch = -LookAxisVector.Y;
		const float ClampedPitch = FMath::Clamp(Pitch, -90.0f, 90.0f);
		AddControllerPitchInput(ClampedPitch * MouseSensibility);
	}
}

void ADetective::Input_Crouching(const FInputActionValue& InputActionValue)
{
	const bool bCrouchPressed = InputActionValue.Get<bool>();
	
	if (bCrouchPressed)
	{
		if (CrouchToggle == 0)
		{
			bCrouching = true;
			CrouchLocation = OldCameraLocation - FVector(0.f, 0.f, CrouchHeight);
			Camera->SetRelativeLocation(CrouchLocation);
			GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchHeight);
			GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
			CrouchToggle = 1;
		}

		else if(bCrouching  && CrouchToggle == 1)
		{
			bCrouching = false;
			//Camera->SetRelativeLocation(OldCameraLocation);
			GetCapsuleComponent()->SetCapsuleHalfHeight(WalkHeight);
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			CrouchToggle = 0;
		}
	}
}


//TODO DELETE THIS FUNCTION
void ADetective::Input_Standing(const FInputActionValue& InputActionValue)
{
	const bool bCrouchPressed = InputActionValue.Get<bool>();
	
	if (!bCrouchPressed && bCrouching && CrouchToggle == 1)
	{
		bCrouching = false;
		// const float OriginalZ = FMath::FInterpTo(CrouchLocation.Z, OldCameraLocation.Z, GetWorld()->GetDeltaSeconds(), 6.0f);
		// const float OriginalY = FMath::FInterpTo(CrouchLocation.Y, OldCameraLocation.Y, GetWorld()->GetDeltaSeconds(), 6.0f);
		// const float OriginalX = FMath::FInterpTo(CrouchLocation.X, OldCameraLocation.X, GetWorld()->GetDeltaSeconds(), 6.0f);
		// const FVector OriginalLocation = {OriginalX, OriginalY, OriginalZ};
		Camera->SetRelativeLocation(OldCameraLocation);
		GetCapsuleComponent()->SetCapsuleHalfHeight(WalkHeight);
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Emerald, FString::Printf(TEXT("Crouch Toggle in standing: %d"), CrouchToggle));
		CrouchToggle = 0;
	}
}

//INPUT  DC CAMERA 
void ADetective::Input_PhotocameraZoomIn(const FInputActionValue& InputActionValue)
{
	bool bKeyPressed = InputActionValue.Get<bool>();

	if (bKeyPressed && DetectivePhotocamera->bDcCameraIsActive)
	{
		DetectivePhotocamera->IncreaseZoom(this);
	}
}

void ADetective::Input_PhotocameraZoomOut(const FInputActionValue& InputActionValue)
{
	bool bKeyPressed = InputActionValue.Get<bool>();


	if (bKeyPressed && DetectivePhotocamera->bDcCameraIsActive)
	{
		DetectivePhotocamera->DecreaseZoom(this);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("ZOOMING OUT"));
		}
	}
}

void ADetective::Input_PhotocameraActive(const FInputActionValue& InputActionValue)
{
	bool bKeyPressed = InputActionValue.Get<bool>();

	if (bKeyPressed && CameraCounter == 0)
	{
		DetectivePhotocamera->bDcCameraIsActive = true;
		CameraCounter += 1;
		Camera->SetFieldOfView(DetectivePhotocamera->LastFOV); // Restore the last known FOV when reactivating the camera
		SetWalkSpeed(WalkSpeed * DetectivePhotocamera->DCMoveSpeedMultiplier);
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("CAMERA IS ACTIVE"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
				FString::Printf(TEXT("CAMERA IS ACTIVE - Default Speed: %.1f, Modified Speed: %.1f"),
					DefaultWalkSpeed, WalkSpeed * DetectivePhotocamera->DCMoveSpeedMultiplier));
		}
	}
	else if (bKeyPressed && CameraCounter == 1)
	{
		DetectivePhotocamera->bDcCameraIsActive = false;
		CameraCounter = 0;
		SetWalkSpeed(DefaultWalkSpeed);
	
		DetectivePhotocamera->LastFOV = Camera->FieldOfView; // Store the current FOV before resetting to the default value

		// Reset FOV when the camera is deactivated
		Camera->SetFieldOfView(90.0f);

		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("CAMERA IS DISABLE"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
				FString::Printf(TEXT("CAMERA IS DISABLE - Speed: %.1f"), DefaultWalkSpeed));
		}
	}
}

void ADetective::Input_TakePhoto(const FInputActionValue& InputActionValue)
{
	bool bKeyPressed = InputActionValue.Get<bool>();

	DetectivePhotocamera->bCanTakePhoto = false;

	if(bKeyPressed && DetectivePhotocamera->bDcCameraIsActive)
	{
		DetectivePhotocamera->bCanTakePhoto = true;
		FScreenshotRequest::RequestScreenshot(false);
		
		EvidenceSystem::ConeCastTraceWithoutSweep(GetWorld(), Camera->GetComponentLocation(), Camera->GetForwardVector(), 1000.f, RayLenght, this);
		
		if(EvidenceSystem::bEvidenceFound)
		{
			//TODO SAVE IT TO INVENTORY AS CLUE EVIDENCE OTHERWISE CRIME SCENE
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Turquoise, TEXT("Evidence saved to inventory"));
			EvidenceSystem::bEvidenceFound = false;
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
				FString::Printf(TEXT("SCREENSHOT")));
		}

	}
}

void ADetective::SetWalkSpeed(float NewSpeed)
{
	WalkSpeed = NewSpeed;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

UCameraComponent* ADetective::GetCamera()
{
	return Camera;
}

#pragma endregion
