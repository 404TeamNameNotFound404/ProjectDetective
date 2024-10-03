// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyActor.h"


// Sets default values
ADummyActor::ADummyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADummyActor::BeginPlay()
{
	Super::BeginPlay();
#if USE_LINUX_SPECIFIC_CODE
	UE_LOG(LogTemp, Warning, TEXT("Hello Linux"));
#elif  USE_WINDOW_SPECIFIC_CODE
	UE_LOGLogTemp, Warning, TEXT("Hello Windows");
#else
#endif
}

// Called every frame
void ADummyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

