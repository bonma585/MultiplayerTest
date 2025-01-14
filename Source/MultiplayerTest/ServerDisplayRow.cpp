// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerDisplayRow.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MainMenu.h"
#include "Components/ScrollBox.h"

void UServerDisplayRow::setParent(UMainMenu* Menu)
{
	Parent = Menu;
}

void UServerDisplayRow::setUniqueIndex(int32 Index)
{
	UniqueIndex = Index;
}

void UServerDisplayRow::notifyParent()
{
	UE_LOG(LogTemp, Warning, TEXT("Server row has been clicked."));
	Parent->notifyParent(UniqueIndex);
}

void UServerDisplayRow::setSelected(bool isSelected)
{
	Selected = isSelected;
	ServerNameButton->ColorAndOpacity = isSelected ? FLinearColor(1.0f, 1.0f, 0.8f, 1.0f) : FLinearColor::White; //visualize selected row
}

bool UServerDisplayRow::Initialize()
{
	if (!Super::Initialize()) { return false; };
	if (!ensure(ServerNameButton != nullptr)) { return false; };
	ServerNameButton->OnClicked.AddDynamic(this, &UServerDisplayRow::notifyParent);
	UE_LOG(LogTemp, Warning, TEXT("Server row functionality added"));
	return true;
}
