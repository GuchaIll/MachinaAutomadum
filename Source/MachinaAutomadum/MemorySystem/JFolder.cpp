// Fill out your copyright notice in the Description page of Project Settings.


#include "JFolder.h"


UJFolder::UJFolder()
{
    ParentFolder = nullptr;
}

FString UJFolder::ListContentByName() const
{
    FString ContentList;
    ContentList += "Folders:\n";
    for(const auto& Folder : SubFolders)
    {
        ContentList += Folder->GetName() + "\n";
    }

    for(const auto& File : Files)
    {
        ContentList += File->GetName() + "\n";
    }

    return ContentList;
}

UJFile *UJFolder::FindFile(const FString &FileName) const
{
    for(const auto& File : Files)
    {
        if(File->GetName() == FileName)
        {
            return File;
        }
    }
    return nullptr;
}

UJFolder *UJFolder::FindFolder(const FString &FolderName) const
{
    for(const auto& Folder : SubFolders)
    {
        if(Folder->GetName() == FolderName)
        {
            return Folder;
        }
    }
    return nullptr;
}

bool UJFolder::MoveFile(UJFile *File, UJFolder *DestinationFolder)
{
    if(File && DestinationFolder)
    {
        File->ParentFolder = DestinationFolder;
        File->SetPath(DestinationFolder->GetPath() + "/" + File->GetName() + "." + File->GetFileTypeAsString());
        DestinationFolder->AddFile(File);
        Files.Remove(File);
        return true;
    }
    return false;
}

bool UJFolder::MoveSubFolder(UJFolder *Folder, UJFolder *DestinationFolder)
{
   if(Folder && DestinationFolder)
   {
         Folder->ParentFolder = DestinationFolder;
            Folder->SetPath(DestinationFolder->GetPath() + "/" + Folder->GetName());
       DestinationFolder->AddSubFolder(Folder);
       SubFolders.Remove(Folder);
       return true;
   }
    return false;
}

UJFolder* UJFolder::AddSubFolder(UJFolder *Folder)
{
    if(Folder)
    {
        Folder->ParentFolder = this;
        Folder->SetPath(Path + "/" + Folder->GetName());
        SubFolders.Add(Folder);
        return Folder;
    }
    return Folder;
}

UJFolder* UJFolder::AddSubFolderByName(const FString &FolderName)
{
    UJFolder* NewFolder = NewObject<UJFolder>();
    NewFolder->SetName(FolderName);
    NewFolder->ParentFolder = this;
    NewFolder->SetPath(Path + "/" + FolderName);
    AddSubFolder(NewFolder);
    return NewFolder;
}

UJFile* UJFolder::AddFile(UJFile *File)
{
    if(File)
    {
        Files.Add(File);
        File->ParentFolder = this;
        File->SetPath(Path + "/" + File->GetName() + "." + File->GetFileTypeAsString());
    }
    return File;
}

UJFile* UJFolder::AddFileByName(const FString &FileName, const FString &Content)
{
    if(FileName.IsEmpty())
    {
        return nullptr;
    }

    UJFile* NewFile = NewObject<UJFile>();
    NewFile->SetName(FileName);
    NewFile->SetContent(Content);
    NewFile->ParentFolder = this;
    NewFile->SetPath(Path + "/" + FileName + "." + NewFile->GetFileTypeAsString());
    Files.Add(NewFile);
    return NewFile;
}

void UJFolder::RemoveSubFolder(UJFolder *Folder)
{

    if(Folder)
    {
        for(int32 i = 0; i < Folder->SubFolders.Num(); i++)
        {
            Folder->RemoveSubFolder(Folder->SubFolders[i]);
        }

        Folder->Files.Empty();
        Folder->ParentFolder = nullptr;
        SubFolders.Remove(Folder);
    }
}

void UJFolder::RemoveSubFolderByName(FString &Folder)
{
    for(int32 i = 0; i < SubFolders.Num(); i++)
    {
        if(SubFolders[i]->GetName() == Folder)
        {
            RemoveSubFolder(SubFolders[i]);
            break;
        }
    }

}

void UJFolder::RemoveFile(UJFile *File)
{
    if(File)
    {
        Files.Remove(File);
    }
}

void UJFolder::RemoveFileByName(FString &FileName)
{
    for(int32 i = 0; i < Files.Num(); i++)
    {
        if(Files[i]->GetName() == FileName)
        {
            Files.RemoveAt(i);
            break;
        }
    }
}
