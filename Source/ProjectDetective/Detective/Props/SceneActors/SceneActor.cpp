// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneActor.h"
#include "../../EvidenceSystem/EvidenceSystem.h"

// Sets default values
ASceneActor::ASceneActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASceneActor::BeginPlay()
{
	Super::BeginPlay();
	EvidenceSystem::AddActorToIgnore(this);
}

// Called every frame
void ASceneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

