// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SceneActor.generated.h"

UCLASS()
class PROJECTDETECTIVE_API ASceneActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASceneActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Scene Actor", meta=(AllowPrivateAccess = "true"))
	FName AssignableTag;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
