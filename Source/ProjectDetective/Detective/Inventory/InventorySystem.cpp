// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem.h"
#include "../EvidenceSystem/EvidenceSystem.h"

TArray<UTexture2D*> InventorySystem::EvidencePhotos;
TArray<UTexture2D*> InventorySystem::CrimeScenePhotos;
FString InventorySystem::CurrentScreenshotPath;
bool InventorySystem::bIsProcessingScreenshot;
int InventorySystem::ScreenshotIndex;
int InventorySystem::CurrentScreenshotIndex;
UTexture2D* InventorySystem::Screenshot;
FString InventorySystem::Path;
FString InventorySystem::ScreenshotPath;
float InventorySystem::ScreenshotTimer;
float InventorySystem::ScreenshotDuration;




//void InventorySystem::TakeScreenshot()
//{
//
//    //Path = FPaths::ProjectSavedDir() + TEXT("Screenshots/") + FPlatformProperties::IniPlatformName() + TEXT("Editor/") + TEXT("Screenshot_") + FString::Printf(TEXT("%d.png"), ScreenshotIndex);
//
//    FScreenshotRequest::RequestScreenshot(Path,false, true);
//
//    //ScreenshotIndex++;
//
//    // Get the default screenshots directory
//    Path = FPaths::ProjectSavedDir() + TEXT("Screenshots/") + FPlatformProperties::IniPlatformName() + TEXT("Editor/");
//    //ScreenshotPath = Path + TEXT("ScreenShot00000.png"); // Default screenshot name format
//    ScreenshotPath = Path + FString::Printf(TEXT("Screenshot_%05d.png"), ScreenshotIndex);  // IL PROBLEMA STA QUI CERCA DI CAPIRE 
//
//    // Wait for a brief moment to ensure the screenshot is saved
//    FPlatformProcess::Sleep(0.5f);
//
//    // Convert FString to const TCHAR*
//    const TCHAR* ScreenshotPathTChar = *ScreenshotPath;
//    const TCHAR* ScreenshotDirTChar = *Path;
//
//    // Check if the default screenshot file exists
//    if (IFileManager::Get().FileExists(ScreenshotPathTChar))
//    {
//        // Define the new name
//        FString NewName = Path + FString::Printf(TEXT("%d.png"), CurrentScreenshotIndex);
//        const TCHAR* NewNameTChar = *NewName;
//
//        // Rename the file
//        IFileManager::Get().Move(NewNameTChar, ScreenshotPathTChar);
//
//        // Log the renaming
//        UE_LOG(LogTemp, Warning, TEXT("Screenshot renamed to: %s"), NewNameTChar);
//
//        CurrentScreenshotIndex++;
//
//        ScreenshotIndex = CurrentScreenshotIndex;
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("No screenshot found to rename"));
//    }
//   
//    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Screenshot DONE !!."));
//    
//    HandleScreenshot();
//}
//
//void InventorySystem::HandleScreenshot(int32 Width, int32 Height, const TArray<FColor>& Bitmap)
//{
//
//	if (bIsProcessingScreenshot)
//	{
//		return;
//	}
//
//	bIsProcessingScreenshot = true;
//
//	ScreenshotIndex = 1;
//	FString ScreenshotPath = FPaths::ProjectSavedDir() + TEXT("Screenshots/Screenshot_") + FString::Printf(TEXT("%d.png"), ScreenshotIndex);
//
//	// Ora carica la texture direttamente dal file salvato
//	UTexture2D* ScreenshotTexture = FileToTexture2d(ScreenshotPath);
//
//	ScreenshotIndex++;
//
//	// Segnala che il processo è terminato
//	bIsProcessingScreenshot = false;
//
//	CheckEvidence(ScreenshotTexture);
//
//}
//
//UTexture2D* InventorySystem::FileToTexture2d(const FString File)
//{
//	FString FullPath = FPaths::ConvertRelativePathToFull(File);
//
//	UTexture2D* texture = nullptr;
//	if (FullPath.Len() > 0)
//	{
//		FullPath.ReplaceInline(TEXT("\\"), TEXT("/"), ESearchCase::CaseSensitive);
//		FullPath.ReplaceInline(TEXT("//"), TEXT("/"), ESearchCase::CaseSensitive);
//		FPlatformMisc::NormalizePath(FullPath);
//
//		texture = FImageUtils::ImportFileAsTexture2D(FullPath); // Importa il file come texture
//	}
//
//	return texture;
//}
//
//void InventorySystem::CheckEvidence(const UTexture2D* ScreenshotTexture)
//{
//	if(EvidenceSystem::bEvidenceFound)
//	{
//		EvidencePhotos.Add(ScreenshotTexture);
//	}
//	else
//	{
//		CrimeScenePhotos.Add(ScreenshotTexture);
//	}
//
//
//}

//void InventorySystem::TakeScreenshot()
//{
//    // Imposta il percorso predefinito per la directory degli screenshot
//    Path = FPaths::ProjectSavedDir() + TEXT("Screenshots/") + FPlatformProperties::IniPlatformName() + TEXT("Editor/");
//
//    // Richiede lo screenshot (non specifichiamo il nome, lasciamo che Unreal lo generi)
//    FScreenshotRequest::RequestScreenshot(false);
//
//    // Aspetta un breve momento per assicurarsi che lo screenshot venga salvato
//    FPlatformProcess::Sleep(0.5f);
//
//    // Ottieni il file più recente dalla directory degli screenshot
//    TArray<FString> Files;
//    IFileManager::Get().FindFiles(Files, *Path, TEXT("*.png"));
//
//    if (Files.Num() > 0)
//    {
//        // Trova il file più recente (dovrebbe essere l'ultimo aggiunto)
//        Files.Sort([](const FString& A, const FString& B) {
//            return IFileManager::Get().GetTimeStamp(*A) > IFileManager::Get().GetTimeStamp(*B);
//            });
//
//        FString LatestScreenshot = Path + Files[0];
//
//        // Rinomina il file al nuovo formato
//        FString NewName = Path + FString::Printf(TEXT("Screenshot_%d.png"), CurrentScreenshotIndex);
//        if (IFileManager::Get().Move(*NewName, *LatestScreenshot))
//        {
//            UE_LOG(LogTemp, Warning, TEXT("Screenshot renamed to: %s"), *NewName);
//
//            // Aggiorna gli indici
//            CurrentScreenshotIndex++;
//            ScreenshotIndex = CurrentScreenshotIndex;
//        }
//        else
//        {
//            UE_LOG(LogTemp, Error, TEXT("Failed to rename screenshot: %s"), *LatestScreenshot);
//        }
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("No screenshots found in directory: %s"), *Path);
//    }
//
//    // Messaggio di debug
//    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Screenshot DONE !!."));
//
//    HandleScreenshot();
//}

void InventorySystem::StartScreenshotProcess()
{
    if (bIsProcessingScreenshot)
    {
        UE_LOG(LogTemp, Warning, TEXT("Screenshot process already in progress."));
        return;
    }
 
    bIsProcessingScreenshot = true;

    // Init timer for the check of the file 
    ScreenshotTimer = 0.0f; 
    ScreenshotDuration = 0.5f;

    FScreenshotRequest::RequestScreenshot(false);

    

}




void InventorySystem::CheckScreenshotProcess()
{
    if (!bIsProcessingScreenshot)
    {
        return;
    }

    // Path directory
    Path = FPaths::ProjectSavedDir() + TEXT("Screenshots/") + FPlatformProperties::IniPlatformName() + TEXT("Editor/");

    // Check if there are screenshots
    TArray<FString> Files;
    IFileManager::Get().FindFiles(Files, *Path, TEXT("*.png"));

    if (Files.Num() > 0)
    {
        // Find and Rename 
        Files.Sort([](const FString& A, const FString& B) {
            FString FullPathA = Path + A;
            FString FullPathB = Path + B;
            return IFileManager::Get().GetTimeStamp(*FullPathA) > IFileManager::Get().GetTimeStamp(*FullPathB);
            });

        FString LatestScreenshot = Path + Files[0];
        FString NewName = Path + FString::Printf(TEXT("Screenshot_%d.png"), CurrentScreenshotIndex);

        if (IFileManager::Get().Move(*NewName, *LatestScreenshot))
        {
            UE_LOG(LogTemp, Warning, TEXT("Screenshot renamed to: %s"), *NewName);
            CurrentScreenshotIndex++;
            ScreenshotIndex = CurrentScreenshotIndex;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to rename screenshot: %s"), *LatestScreenshot);
        }

        //// Complete the process
        //bIsProcessingScreenshot = false;

        Screenshot = LoadScreenshotFromFile(NewName);

        if (Screenshot)
        {
            // Check if the screenshot is an evidence
            CheckEvidence();
        }
        else
        {
            // Show an error message if loading failed
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error to upload the screenshot."));
        }

    }
    
        // Complete the process
        bIsProcessingScreenshot = false;
    
}


void InventorySystem::UpdateScreenshotTimer(float DeltaTime)
{
    if (!bIsProcessingScreenshot)
    {
        return; // no process 
    }

    // Incrementa il timer
    ScreenshotTimer += DeltaTime;

    if (ScreenshotTimer >= ScreenshotDuration)
    {
        
        CheckScreenshotProcess();

        // Reset timer
        bIsProcessingScreenshot = false;
        ScreenshotTimer = 0.0f;
    }
}

//void InventorySystem::HandleScreenshot()
//{
//    if (bIsProcessingScreenshot)
//    {
//        
//        return;
//    }
//
//    
//    bIsProcessingScreenshot = true;
//    
//    //FString ScreenshotPath = FPaths::ProjectSavedDir() + TEXT("Screenshots/WindowsEditor/Screenshot_") + FString::Printf(TEXT("%d.png"), ScreenshotIndex);
//    //FString ScreenshotPath = FPaths::ProjectSavedDir() + TEXT("Screenshots/") + FPlatformProperties::IniPlatformName() + TEXT("Editor/") + TEXT("/Screenshot_") + FString::Printf(TEXT("%d.png"), ScreenshotIndex);
//  
//    if (FPaths::FileExists(ScreenshotPath))
//    {    
//        UE_LOG(LogTemp, Warning, TEXT("Screenshot Path don't exists: %s"), *ScreenshotPath);
//        bIsProcessingScreenshot = false;    
//        return;
//    }
//    
//
//    Screenshot = LoadScreenshotFromFile(ScreenshotPath);
//
//    if (Screenshot)
//    {
//        // Controlla se l'immagine è una Evidence
//        CheckEvidence();
//    }
//    else
//    {
//        
//        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error to upload the screenshot."));
//        
//    }
//
//    bIsProcessingScreenshot = false;
//}



//UTexture2D* InventorySystem::FileToTexture2d(FString File)
//{
//    FString FullPath = FPaths::ConvertRelativePathToFull(File);
//    
//    UTexture2D* texture = nullptr;
//    if (FullPath.Len() > 0)
//    {
//    	FullPath.ReplaceInline(TEXT("\\"), TEXT("/"), ESearchCase::CaseSensitive);
//    	FullPath.ReplaceInline(TEXT("//"), TEXT("/"), ESearchCase::CaseSensitive);
//    	FPlatformMisc::NormalizePath(FullPath);
//    
//    	texture = FImageUtils::ImportFileAsTexture2D(FullPath); // Importa il file come texture
//    }
//    
//    return texture;
//}

void InventorySystem::CheckEvidence()
{
    if (!Screenshot)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Screenshot is null."));
        return;
    }

    // Debug message to check if EvidenceFound is true
    if (EvidenceSystem::bEvidenceFound)
    {
        EvidencePhotos.Add(Screenshot);
        EvidenceSystem::bEvidenceFound = false;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Screenshot added to EvidencePhotos array."));
    }
    else
    {
        CrimeScenePhotos.Add(Screenshot);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No evidence found. Screenshot added to Crime Scene."));
    }

    //Debug message to check the array size
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("EvidencePhotos array size: %d"), EvidencePhotos.Num()));

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("CrimeScenePhotos array size: %d"), CrimeScenePhotos.Num()));
}

UTexture2D* InventorySystem::LoadScreenshotFromFile(FString& ScreenshotFile)
{

    // Check if the file exists before attempting to load it
    if (!IFileManager::Get().FileExists(*ScreenshotFile))
    {
        UE_LOG(LogTemp, Error, TEXT("File not found: %s"), *ScreenshotFile);
        return nullptr;
    }

    UE_LOG(LogTemp, Warning, TEXT("Trying to load screenshot from path: %s"), *ScreenshotFile);

    // Load the image file as a texture
    UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(ScreenshotFile); //Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *ScreenshotFile));
    if (Texture)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Load texture Complete "));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load texture from path: %s"), *ScreenshotFile);
    }
    return Texture;
}

	



