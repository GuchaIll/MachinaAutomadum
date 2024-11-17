// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JFile.h"
#include "JFolder.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class MACHINAAUTOMADUM_API UJFolder : public UObject
{
	GENERATED_BODY()

	private:
		UPROPERTY(BlueprintReadWrite, Category = "Folder", meta = (AllowPrivateAccess = "true"))
		FString Name;

		UPROPERTY(BlueprintReadWrite, Category = "Folder", meta = (AllowPrivateAccess = "true"))
		FString Path;

		UPROPERTY(BlueprintReadWrite, Category = "Folder", meta = (AllowPrivateAccess = "true"))
		TArray<UJFolder*> SubFolders;

		UPROPERTY(BlueprintReadWrite, Category = "Folder", meta = (AllowPrivateAccess =	"true"))
		TArray<UJFile*> Files;

		UPROPERTY(BlueprintReadWrite, Category = "Folder", meta = (AllowPrivateAccess = "true"))
		UJFolder* ParentFolder;

	public:
		UJFolder();

		UFUNCTION(BlueprintCallable, Category = "Folder")
		UJFolder* AddSubFolder(UJFolder* Folder);

		UFUNCTION(BlueprintCallable, Category = "Folder")
		UJFolder* AddSubFolderByName(const FString& FolderName);

		UFUNCTION(BlueprintCallable, Category = "Folder")
		UJFile* AddFile(UJFile* File);

		UFUNCTION(BlueprintCallable, Category = "Folder")
		UJFile* AddFileByName(const FString& FileName, const FString& Content);

		UFUNCTION(BlueprintCallable, Category = "Folder")
		void RemoveSubFolder(UJFolder* Folder);

		UFUNCTION(BlueprintCallable, Category = "Folder")
		void RemoveSubFolderByName(FString& Folder);

		UFUNCTION(BlueprintCallable, Category = "Folder")
		void RemoveFile(UJFile* File);

		UFUNCTION(BlueprintCallable, Category = "Folder")
		void RemoveFileByName( FString& FileName);

		UFUNCTION(BlueprintCallable, Category = "Folder")
		void SetName(const FString& NewName){ Name = NewName; };

		UFUNCTION(BlueprintCallable, Category = "Folder")
		FString GetName() const{ return Name; };

		UFUNCTION(BlueprintCallable, Category = "Folder")
		FString GetPath() const{ return Path; };

		UFUNCTION(BlueprintCallable, Category = "Folder")
		void SetPath(const FString& NewPath){ Path = NewPath; };

		UFUNCTION(BlueprintCallable, Category = "Folder")
		TArray<UJFolder*> GetAllSubFolders() const { return SubFolders; };

		UFUNCTION(BlueprintCallable, Category = "Folder")
		TArray<UJFile*> GetAllFiles() const { return Files; };

		UFUNCTION(BlueprintCallable, Category = "Folder")
		FString ListContentByName() const;

		UFUNCTION(BlueprintCallable, Category = "Folder")
		UJFile* FindFile(const FString& FileName) const;

		UFUNCTION(BlueprintCallable, Category = "Folder")
		UJFolder* FindFolder(const FString& FolderName) const;

		UFUNCTION(BlueprintCallable, Category = "Folder")
		UJFolder* GetParentFolder() const { return ParentFolder; };

		UFUNCTION(BlueprintCallable, Category = "Folder")
		void SetParentFolder(UJFolder* NewParentFolder) { ParentFolder = NewParentFolder; };

		UFUNCTION(BlueprintCallable, Category = "Folder")
		bool MoveFile(UJFile* File, UJFolder* DestinationFolder);

		UFUNCTION(BlueprintCallable, Category = "Folder")
		bool MoveSubFolder(UJFolder* Folder, UJFolder* DestinationFolder);	
	
};
