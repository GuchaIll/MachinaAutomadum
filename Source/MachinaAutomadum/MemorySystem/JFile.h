// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JFile.generated.h"

/**
 * 
 */

class UJFolder;

UENUM(BlueprintType)
enum class EFileLoadStatus : uint8
{
	Ok,
	Error,
	FileNotLoaded,
	FileNotFound
};

UENUM(BlueprintType)
enum class EFileType: uint8
{
	FT_Text UMETA(DisplayName = "txt"),
	FT_Image UMETA(DisplayName = "png"),
	FT_Audio UMETA(DisplayName = "mp3"),
	FT_Video UMETA(DisplayName = "mp4"),
	FT_Document UMETA(DisplayName = "doc"),
	FT_Virus UMETA(DisplayName = "virus"),
	FT_Other UMETA(DisplayName = "other")
};


UCLASS(BlueprintType, Blueprintable)
class MACHINAAUTOMADUM_API UJFile : public UObject
{
	GENERATED_BODY()

	private:
		UPROPERTY(BlueprintReadWrite, Category = "File", meta = (AllowPrivateAccess = "true"))
		FString Name;

		UPROPERTY(BlueprintReadWrite, Category = "File", meta = (AllowPrivateAccess = "true"))
		FString Content;

        
		UPROPERTY(BlueprintReadWrite, Category = "File", meta = (AllowPrivateAccess = "true"))
		EFileLoadStatus LoadStatus;

		UPROPERTY(BlueprintReadWrite, Category = "File", meta = (AllowPrivateAccess = "true"))
		FString FilePath;

	
	public:
		UJFile();

		UPROPERTY(BlueprintReadWrite, Category = "File", meta = (AllowPrivateAccess = "true"))
		EFileType Type;

		UPROPERTY(BlueprintReadWrite, Category = "File", meta = (AllowPrivateAccess = "true"))
		UJFolder* ParentFolder;


		UFUNCTION(BlueprintCallable, Category = "File")
		FString GetName() const { return Name; }

		UFUNCTION(BlueprintCallable, Category = "File")
		void SetName(const FString& NewName) { Name = NewName; }

		UFUNCTION(BlueprintCallable, Category = "File")
		FString GetContent() const { return Content; }

		UFUNCTION(BlueprintCallable, Category = "File")
		void SetContent(const FString& NewContent) { Content = NewContent; }

		UFUNCTION(BlueprintCallable, Category = "File")
		bool LoadFile(const FString& Path);

		UFUNCTION(BlueprintCallable, Category = "File")
		bool SaveFile(const FString& Path);

		UFUNCTION(BlueprintCallable, Category = "File")
		FString GetPath() const { return FilePath; }

		UFUNCTION(BlueprintCallable, Category = "File")
		void SetPath(const FString& NewPath) { FilePath = NewPath; }

		UFUNCTION(BlueprintCallable, Category = "File")
		UJFolder* GetParentFolder() const { return ParentFolder; }

		UFUNCTION(BlueprintCallable, Category = "File")
		void SetParentFolder(UJFolder* NewParentFolder) { ParentFolder = NewParentFolder; }

		UFUNCTION(BlueprintCallable, Category = "File")
		FString GetFileTypeAsString() const
		{
			switch (Type)
			{
			case EFileType::FT_Text:
				return TEXT("txt");
			case EFileType::FT_Image:
				return TEXT("png");
			case EFileType::FT_Audio:
				return TEXT("mp3");
			case EFileType::FT_Video:
				return TEXT("mp4");
			case EFileType::FT_Document:
				return TEXT("doc");
			case EFileType::FT_Virus:
				return TEXT("virus");
			default:
				return TEXT("other");
			}
		}

        UFUNCTION(BlueprintCallable, Category = "File")
        void SolveFilePath();

        UFUNCTION(BlueprintCallable, Category = "File")
        void HandleFileType();

		
	
};
