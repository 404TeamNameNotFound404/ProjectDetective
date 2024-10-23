// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAsset/DetectiveInputDataAsset.h"
#include "DetectiveEnhancedInput.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTDETECTIVE_API UDetectiveEnhancedInput : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(UDetectiveInputDataAsset* InputConfig, const FGameplayTag& GameplayTag , ETriggerEvent
		TriggerEvent, UserObject* Target, CallbackFunc Callback);
};

template <class UserObject, typename CallbackFunc>
inline void UDetectiveEnhancedInput::BindNativeInputAction(UDetectiveInputDataAsset* InputConfig,
	const FGameplayTag& GameplayTag, ETriggerEvent TriggerEvent, UserObject* Target, CallbackFunc Callback)
{
	checkf(InputConfig, TEXT("InputConfig must be a valid UDetectiveInputDataAsset"));

	if(UInputAction* InputAction = InputConfig->FindNativeInputByTag(GameplayTag))
	{
		BindAction(InputAction, TriggerEvent, Target, Callback);
	}
}
