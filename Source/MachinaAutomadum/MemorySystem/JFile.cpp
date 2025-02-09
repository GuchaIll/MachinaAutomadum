// Fill out your copyright notice in the Description page of Project Settings.


#include "JFile.h"
#include "JFolder.h"
#include "Engine/Texture2D.h"
#include "Sound/SoundWave.h"
#include "UObject/ConstructorHelpers.h"

UJFile::UJFile()
{
    Type = EFileType::FT_Other;
    Name = TEXT("New File");
    LoadStatus = EFileLoadStatus::FileNotFound;
    ParentFolder = nullptr;
    FilePath = TEXT("");
    Content = TEXT("");

    SolveFilePath();
    //LoadFile(FilePath);
}

bool UJFile::LoadFile(const FString& Path)
{
    if (FPaths::FileExists(Path))
    {
        FilePath = Path;
        FFileHelper::LoadFileToString(Content, *Path);
        Name = FPaths::GetCleanFilename(Path);
        LoadStatus = EFileLoadStatus::Ok;
        return true;
    }
    else
    {
        LoadStatus = EFileLoadStatus::FileNotFound;
        return false;
    }
}

bool UJFile::SaveFile(const FString& Path)
{
    if (FPaths::FileExists(Path))
    {
        FilePath = Path;
        FFileHelper::SaveStringToFile(Content, *Path);
        Name = FPaths::GetCleanFilename(Path);
        LoadStatus = EFileLoadStatus::Ok;
        return true;
    }
    else
    {
        LoadStatus = EFileLoadStatus::FileNotFound;
        return false;
    }
}

void UJFile::SolveFilePath()
{
    int i = 0;
    UJFolder* CurrFolder = ParentFolder;
    while(i < 100 && CurrFolder->GetName() != TEXT("Root"))
    {
       FilePath = CurrFolder->GetPath() + TEXT("/") + FilePath;
       CurrFolder = CurrFolder->GetParentFolder();
        i++;

    }
    FString DiskRootPath = FPaths::ProjectDir();
    FilePath = DiskRootPath + TEXT("Content/Root/") + FilePath;
    if(GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FilePath);
    }
}

void UJFile::HandleFileType()
{
    FString Extension = FPaths::GetExtension(FilePath).ToLower();

    
    if (Extension == "uasset")
    {
        UObject* LoadedAsset = StaticLoadObject(UObject::StaticClass(), nullptr, *FilePath);
        if (LoadedAsset)
        {
            //if (LoadedAsset->IsA(UTexture2D::StaticClass()))
            //{
                //Type = EFileType::FT_Image;
            //}
            //else if (LoadedAsset->IsA(UMediaSource::StaticClass()))
           // {
            //    Type = EFileType::FT_Media;
            // Add more asset types as needed
            //}
            //else if(LoadAsset->IsA(USoundWave::StaticClass()))
           // {
              //  Type = EFileType::FT_Sound;
           // }
           //else
           // {
              // Type = EFileType::FT_Asset;
            //}
    }
}
}
