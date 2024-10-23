// Fill out your copyright notice in the Description page of Project Settings.


#include "FileExplorerManager.h"
#include "JFolder.h"
#include "JFile.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"



// Sets default values
AFileExplorerManager::AFileExplorerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	DiskRootPath = FPaths::ProjectDir() + TEXT("Content/Root");
	RootFolder = NewObject<UJFolder>();
	RootFolder->SetName("Root");
	RootFolder->SetPath("/");

}

// Called when the game starts or when spawned
void AFileExplorerManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFileExplorerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFileExplorerManager::SyncwithRootFolder(const FString &RootPath)
{
	if(!FPaths::DirectoryExists(RootPath))
	{
		 if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Root path does not exist"));
        }
		UE_LOG(LogTemp, Error, TEXT("Root path does not exist"));
		return;
	}
	 if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("Root path found"));
        }
	RecursivelySyncFolder(RootFolder, RootPath, 0);
}



UJFolder *AFileExplorerManager::GetFolderByPath(const FString &Path)
{
    TArray<FString> ParsedPath = ParsePath(Path);

	UJFolder* CurrentFolder = RootFolder;

	for(const FString& FolderName : ParsedPath)
	{
		CurrentFolder = CurrentFolder->FindFolder(FolderName);
		if(!CurrentFolder)
		{
			return nullptr;
		}
	}

	return CurrentFolder;
}

UJFile *AFileExplorerManager::GetFileByPath(const FString &FileName)
{
	TArray<FString> ParsedPath = ParsePath(FileName);
	UJFolder* CurrentFolder = RootFolder;

	for(int32 i = 0; i < ParsedPath.Num() - 1; i++)
	{
		CurrentFolder = CurrentFolder->FindFolder(ParsedPath[i]);
		if(!CurrentFolder)
		{
			return nullptr;
		}
	}

	return CurrentFolder->FindFile(ParsedPath.Last());
}

bool AFileExplorerManager::IsPathValid(const FString &Path) const
{
    return false;
}

UJFolder* AFileExplorerManager::CreateFolder(const FString &FolderName, UJFolder *ParentFolder)
{
    if (ParentFolder)
	{
		UJFolder* newFolder = ParentFolder->AddSubFolderByName(FolderName);
		return newFolder;
	}
	else{
		UJFolder* newFolder = RootFolder->AddSubFolderByName(FolderName);
		return newFolder;
	}
}

UJFile* AFileExplorerManager::CreateFile(const FString &FileName, const FString &Content, UJFolder *ParentFolder)
{
    if(ParentFolder)
	{
		UJFile* newFile = ParentFolder->AddFileByName(FileName, Content);
		return newFile;
	}
	else{
		UJFile* newFile = RootFolder->AddFileByName(FileName, Content);
		return newFile;
	}
}

bool AFileExplorerManager::RemoveFolder(UJFolder *Folder)
{
    if(Folder)
	{
		UJFolder* ParentFolder = Folder->GetParentFolder();
		if(ParentFolder)
		{
			ParentFolder->RemoveSubFolder(Folder);
			return true;
		}
	}
	return false;
}

bool AFileExplorerManager::RemoveFile(UJFile *File)
{
    if(File)
	{
		UJFolder* ParentFolder = File->GetParentFolder();
		if(ParentFolder)
		{
			ParentFolder->RemoveFile(File);
			return true;
		}
	}
	return false;
}



UJFolder *AFileExplorerManager::DuplicateFolder(UJFolder *Folder, UJFolder *DestinationFolder)
{
	if(DestinationFolder)
	{
		UJFolder* NewFolder = NewObject<UJFolder>();
		NewFolder->SetName(Folder->GetName());
		NewFolder->SetPath(Folder->GetPath());
		NewFolder->SetParentFolder(DestinationFolder);
		DestinationFolder->AddSubFolder(NewFolder);
		for(UJFolder* SubFolder : Folder->GetAllSubFolders())
		{
			DuplicateFolder(SubFolder, NewFolder);
		}
		for(UJFile* File : Folder->GetAllFiles())
		{
			DuplicateFile(File, NewFolder);
		}
		return NewFolder;
	}
	else{
		UJFolder* NewFolder = NewObject<UJFolder>();
		NewFolder->SetName(Folder->GetName());
		NewFolder->SetPath(Folder->GetPath());
		UJFolder* ParentFolderDest = Folder->GetParentFolder();
		NewFolder->SetParentFolder(ParentFolderDest);
		ParentFolderDest->AddSubFolder(NewFolder);
		for(UJFolder* SubFolder : Folder->GetAllSubFolders())
		{
			DuplicateFolder(SubFolder, NewFolder);
		}
		for(UJFile* File : Folder->GetAllFiles())
		{
			DuplicateFile(File, NewFolder);
		}
		return NewFolder;
	}
	
}

UJFile *AFileExplorerManager::DuplicateFile(UJFile *File, UJFolder *DestinationFolder)
{
   
	if(DestinationFolder)
	{
		UJFile* NewFile = NewObject<UJFile>();
		NewFile->SetName(File->GetName());
		NewFile->SetContent(File->GetContent());
		NewFile->SetPath(File->GetPath());
		NewFile->Type = File->Type;
		NewFile->SetParentFolder(DestinationFolder);
		DestinationFolder->AddFile(NewFile);
		return NewFile;
	}
	else{
		UJFile* NewFile = NewObject<UJFile>();
		NewFile->SetName(File->GetName());
		NewFile->SetContent(File->GetContent());
		NewFile->SetPath(File->GetPath());
		NewFile->Type = File->Type;
		UJFolder* ParentFolderDest = File->GetParentFolder();
		NewFile->SetParentFolder(ParentFolderDest);
		ParentFolderDest->AddFile(NewFile);
		return NewFile;
	}
}

TArray<FString> AFileExplorerManager::ParsePath(const FString &Path) const
{
    TArray<FString> ParsedPath;
	Path.ParseIntoArray(ParsedPath, TEXT("/"), true);
	return ParsedPath;
}

bool AFileExplorerManager::MoveFile(UJFile *File, UJFolder *DestinationFolder)
{
   if(File && DestinationFolder)
   {
	   DestinationFolder->AddFile(File);
	   UJFolder* ParentFolderDest = File->GetParentFolder();
	   ParentFolderDest->RemoveFile(File);
	   return true;
   }
   return false;
}

bool AFileExplorerManager::MoveFolder(UJFolder *Folder, UJFolder *DestinationFolder)
{
    if(Folder && DestinationFolder)
	{
		DestinationFolder->AddSubFolder(Folder);
		UJFolder* ParentFolderDest = Folder->GetParentFolder();
		ParentFolderDest->RemoveSubFolder(Folder);
		return true;
	}
	return false;
}

void AFileExplorerManager::RecursivelySyncFolder(UJFolder* VirtualFolder, const FString& PhysicalFolderPath, int32 Depth)
{
    // Check recursion depth to prevent infinite recursion
    if (Depth > 100)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Recursion depth limit reached"));
        }
        return;
    }

    // Check if the virtual folder or physical path is valid
    if (!VirtualFolder)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Virtual Folder is null"));
        }
        UE_LOG(LogTemp, Error, TEXT("Virtual Folder is null"));
        return;
    }

	
    IFileManager& FileManager = IFileManager::Get();
	 FString NormalizedPhysicalFolderPath = PhysicalFolderPath;
    FPaths::NormalizeDirectoryName(NormalizedPhysicalFolderPath);
    if (!FPaths::DirectoryExists(NormalizedPhysicalFolderPath))
    {
        
        return;
    }

    
    // Iterate through all files and folders in the current directory
    FileManager.IterateDirectory(*NormalizedPhysicalFolderPath, [&](const TCHAR* FilenameOrDirectory, bool bIsDirectory) -> bool
    {	
		
        
		FString CurrentItem = FString(FilenameOrDirectory);
        FString CleanName = FPaths::GetCleanFilename(CurrentItem);
        FString FullPath = FPaths::Combine(NormalizedPhysicalFolderPath, CleanName);

        if (bIsDirectory)
        {
            // Create a new virtual folder for the subdirectory
            UJFolder* NewFolder = NewObject<UJFolder>();
            NewFolder->SetName(CleanName);
            NewFolder->SetPath(FullPath);
            NewFolder->SetParentFolder(VirtualFolder);

            VirtualFolder->AddSubFolder(NewFolder);

            // Recurse into the subfolder

			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Added Folder for %s"), *CleanName));
			}
			
			
            RecursivelySyncFolder(NewFolder, FullPath, Depth + 1);
			if(GEngine){
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Recursion Finished for Folder %s"), *FullPath));
			}
        }
        else
        {
			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Adding File %s"), *CleanName));
			}
            // Create a new virtual file for the file
            FString FileExtension = FPaths::GetExtension(CurrentItem);
            EFileType FileType = DetermineFileType(FileExtension);

            FString FileContent;
            FFileHelper::LoadFileToString(FileContent, *FullPath);

            UJFile* NewFile = NewObject<UJFile>();
            NewFile->SetName(CleanName);
            NewFile->SetContent(FileContent);
            NewFile->SetPath(FullPath);
            NewFile->Type = FileType;

            VirtualFolder->AddFile(NewFile);
        }

        return true; // Continue iterating
    });

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, FString::Printf(TEXT("Finished Iterating through %s"), *PhysicalFolderPath));
	}
}

EFileType AFileExplorerManager::DetermineFileType(const FString& Extension)
{
    // Determine the file type based on the extension
    if (Extension == TEXT("txt"))
    {
        return EFileType::FT_Text;
    }
	else if( Extension == TEXT("png"))
	{
		 return EFileType::FT_Image;
	}
    else if (Extension == TEXT("mp3"))
    {
        return EFileType::FT_Audio;
    }
    else if (Extension == TEXT("mp4"))
    {
        return EFileType::FT_Video;
    }
    else if (Extension == TEXT("doc"))
    {
        return EFileType::FT_Document;
    }
    return EFileType::FT_Other;
}