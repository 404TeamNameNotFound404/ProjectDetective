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
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Speed = 50.0f;
	MouseSensibility = 0.2f;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
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
	GetCharacterMovement()->MaxWalkSpeed = Speed;
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
	LastChecked->BindNativeInputAction(InputDataAsset, DetectiveTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
}


void ADetective::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2d Direction = InputActionValue.Get<FVector2d>();
	
	const FRotator Rotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if(Direction.Y != 0.f)
	{
		const FVector ForwardDirection = Rotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, Direction.Y );
	}

	if(Direction.X != 0.f)
	{
		const FVector RightDirection = Rotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, Direction.X);
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
