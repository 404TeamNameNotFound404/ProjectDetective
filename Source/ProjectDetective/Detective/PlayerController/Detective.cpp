// Fill out your copyright notice in the Description page of Project Settings.


#include "Detective.h"

#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Controller/Tags/DetectiveGameplayTags.h"
#include "ProjectDetective/Detective/Controller/DetectiveController.h"


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

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(PlayerWidth);
	GetCapsuleComponent()->SetCapsuleSize(PlayerWidth,WalkHeight);
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
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCapsuleComponent()->SetCapsuleRadius(PlayerWidth);
	GetCapsuleComponent()->SetCapsuleSize(PlayerWidth,WalkHeight);
	Camera->FieldOfView = 90.0f;
	OldCameraLocation = Camera->GetRelativeLocation();
}

// Called every frame
void ADetective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void ADetective::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputDataAsset, TEXT("Forgot to add data asset to Blueprint"));
	ULocalPlayer* LocalPlayer = GetController<ADetectiveController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	checkf(SubSystem, TEXT("Subsystem not found"));
	SubSystem->AddMappingContext(InputDataAsset->DefaultMappingContext, 0);
	UDetectiveEnhancedInput* LastChecked = CastChecked<UDetectiveEnhancedInput>(PlayerInputComponent);
	LastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	LastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_Move, ETriggerEvent::Completed, this, &ThisClass::Input_StopMove);
	LastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	LastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouching);
	LastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_Crouch, ETriggerEvent::Completed, this, &ThisClass::Input_Standing);
}


void ADetective::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2d Direction = InputActionValue.Get<FVector2d>();
	DirectionMovement = FVector::ZeroVector;
	
	const FRotator Rotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	
	if(Direction.Y != 0.f)
	{
		const FVector ForwardDirection = Rotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, Direction.Y );
		DirectionMovement.Y = Direction.Y;
		bIsWalking = true;
	}
	
	if(Direction.X != 0.f)
	{
		const FVector RightDirection = Rotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, Direction.X);
		DirectionMovement.X = Direction.X;
		bIsWalking = true;
	}
	
	bIsWalking = Direction.X != 0.f || Direction.Y != 0.f;
}

void ADetective::Input_StopMove(const FInputActionValue& InputActionValue)
{
	const FVector2d Direction = InputActionValue.Get<FVector2d>();
	
	if(Direction.X == 0.f || Direction.Y == 0.f)
	{
		bIsWalking = false;
	}
}

void ADetective::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2d LookAxisVector = InputActionValue.Get<FVector2d>();

	if(LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X * MouseSensibility);
	}

	if(LookAxisVector.Y != 0.f)
	{
		const float Pitch = LookAxisVector.Y;
		const float ClampedPitch = FMath::Clamp(Pitch, -90.0f, 90.0f);
		AddControllerPitchInput(ClampedPitch * MouseSensibility);
	}
}

void ADetective::Input_Crouching(const FInputActionValue& InputActionValue)
{
	const bool bCrouchPressed = InputActionValue.Get<bool>();
	
	if(bCrouchPressed)
	{
		bCrouching = true;
		CrouchLocation = OldCameraLocation - FVector(0.f, 0.f, CrouchHeight);
		Camera->SetRelativeLocation(CrouchLocation);
		GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchHeight);
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
	}
}

void ADetective::Input_Standing(const FInputActionValue& InputActionValue)
{
	const bool bCrouchPressed = InputActionValue.Get<bool>();

	if(!bCrouchPressed && bCrouching)
	{
		bCrouching = false;
		Camera->SetRelativeLocation(OldCameraLocation);
		GetCapsuleComponent()->SetCapsuleHalfHeight(WalkHeight);
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

