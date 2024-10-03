// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectDetectiveGameMode.h"
#include "ProjectDetectiveCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectDetectiveGameMode::AProjectDetectiveGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
