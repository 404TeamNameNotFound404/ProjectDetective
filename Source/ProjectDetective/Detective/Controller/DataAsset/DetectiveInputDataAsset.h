// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "DetectiveInputDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDetectiveInputConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (category = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;
};

UCLASS()
class PROJECTDETECTIVE_API UDetectiveInputDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDetectiveInputConfig> InputConfigs;

	UInputAction* FindNativeInputByTag(const FGameplayTag& InputTag) const;
};
