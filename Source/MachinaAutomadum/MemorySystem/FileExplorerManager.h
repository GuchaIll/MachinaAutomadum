// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JFolder.h"
#include "FileExplorerManager.generated.h"


UCLASS(BlueprintType, Blueprintable)
class MACHINAAUTOMADUM_API AFileExplorerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFileExplorerManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	void SyncwithRootFolder(const FString &RootPath);

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	UJFolder* GetFolderByPath(const FString& Path);

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	UJFile* GetFileByPath(const FString& FileName);

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	bool IsPathValid(const FString& Path) const;

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	UJFolder* CreateFolder(const FString& FolderName, UJFolder* ParentFolder);

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	UJFile* CreateFile(const FString& FileName, const FString& Content, UJFolder* ParentFolder);

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	bool MoveFolder(UJFolder* Folder, UJFolder* DestinationFolder);

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	bool MoveFile(UJFile* File, UJFolder* DestinationFolder);

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	bool RemoveFolder(UJFolder* Folder);

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	bool RemoveFile(UJFile* File);

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	UJFolder* GetRootFolder() const { return RootFolder; }

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	UJFolder* DuplicateFolder(UJFolder* Folder, UJFolder* DestinationFolder = nullptr);

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	UJFile* DuplicateFile(UJFile* File, UJFolder* DestinationFolder);

	UFUNCTION(BlueprintCallable, Category = "File Explorer")
	TArray<FString> ParsePath(const FString& Path) const;

	UPROPERTY(BlueprintReadOnly, Category = "File Explorer", meta = (AllowPrivateAccess = "true"))
	FString DiskRootPath;

private:
	UPROPERTY(BlueprintReadWrite, Category = "File Explorer", meta = (AllowPrivateAccess = "true"))
	UJFolder* RootFolder;

	EFileType DetermineFileType(const FString& Extension);

	void RecursivelySyncFolder(UJFolder* VirtualFolder, const FString& PhysicalFolderPath, int32 Depth = 0);







};
