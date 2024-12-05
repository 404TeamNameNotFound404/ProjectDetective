// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FileHelpers.h"
#include "ImageUtils.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"
#include "Misc/FileHelper.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "Engine/GameViewportClient.h"
#include "CoreMinimal.h"

class EvidenceSystem;

class PROJECTDETECTIVE_API InventorySystem
{
public:


	static void StartScreenshotProcess();

	static void UpdateScreenshotTimer(float DeltaTime);

	static void CheckScreenshotProcess();


private:

	static void CheckEvidence();
	static UTexture2D* LoadScreenshotFromFile(FString& ScreenshotFile);
	



	static TArray<UTexture2D*> EvidencePhotos;
	static TArray<UTexture2D*> CrimeScenePhotos;
	static UTexture2D* Screenshot;

	static FString CurrentScreenshotPath;
	static FString ScreenshotPath;
	static FString Path;

	static int32 ScreenshotIndex;
	static int32 CurrentScreenshotIndex;

	static bool bIsProcessingScreenshot;
	static bool bIsEvidence;

	static float ScreenshotTimer;
	static float ScreenshotDuration;

};
