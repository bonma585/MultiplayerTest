// Fill out your copyright notice in the Description page of Project Settings.



#include "MainMenu.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "ServerDisplayRow.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"



void UMainMenu::NativeConstruct() {
	UE_LOG(LogTemp, Warning, TEXT("UMainMenu::NativeConstruct() called"));


	APlayerController* playerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	FInputModeUIOnly inputMode = FInputModeUIOnly();
	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = true;


	ButtonEnableFunction();
}

void UMainMenu::NativeDestruct() {
	APlayerController* playerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (playerController == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("playerController is a nullptr"));
		return;
	}

	FInputModeGameAndUI inputMode;
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputMode.SetHideCursorDuringCapture(false);

	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = true;
}




void UMainMenu::host() {
	UE_LOG(LogTemp, Warning, TEXT("UMainMenu::Host() called"));

	if (AnotherGameinstance == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("AnotherGameinstance is a nullptr"));
		return;
	}

	AnotherGameinstance->host(ServerNameText->GetText().ToString());
}


void UMainMenu::join() {
	UE_LOG(LogTemp, Warning, TEXT("UMainMenu::Join() called"));

	if (AnotherGameinstance == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("AnotherGameinstance is a nullptr"));
		return;
	}

	AnotherGameinstance->join(selectedIndex);

	ButtonDisableFunction();
}

void UMainMenu::refresh() {
	UE_LOG(LogTemp, Warning, TEXT("Refreshing sessions"));

	if (AnotherGameinstance == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("AnotherGameinstance is a nullptr"));
		return;
	}
	AnotherGameinstance->OnFindSessionComplete(true);
}

void UMainMenu::SetGameInstance(UAnotherGameinstance* gameInstance) {
	AnotherGameinstance = gameInstance;
}

void UMainMenu::notifyParent(int32 index)
{
	selectedIndex = index;
	UE_LOG(LogTemp, Warning, TEXT("Target Session is Session with Index '%d'"), index);
	for (int Index = 0; Index < ServerList->GetChildrenCount(); Index++) {
		if (UServerDisplayRow* ServerRow = Cast<UServerDisplayRow>(ServerList->GetChildAt(Index))) {
			ServerRow->setSelected(Index == selectedIndex);
		}
	}

}

void UMainMenu::UpdateServerList(TArray<FServerData> ServerNames)
{
	int Index = 0;
	for (FServerData ServerData : ServerNames) {
		UServerDisplayRow* ServerDisplayRow = CreateWidget<UServerDisplayRow>(this, AnotherGameinstance->ServerDisplayRowClass);
		if (ServerDisplayRow != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Session hello."));
			ServerDisplayRow->setUniqueIndex(ServerList->GetChildrenCount());
			ServerDisplayRow->setUniqueIndex(Index++);
			ServerDisplayRow->setParent(this);
			ServerDisplayRow->ServerName->SetText(FText::FromString(ServerData.ServerName));
			ServerDisplayRow->PlayerOwner->SetText(FText::FromString(ServerData.PlayerName));
			ServerDisplayRow->CurrentPlayerNumber->SetText(FText::FromString(FString::FromInt(ServerData.CurrentPlayerNumber)));
			ServerDisplayRow->MaxPlayerNumber->SetText(FText::FromString(FString::FromInt(ServerData.MaxPlayerNumber)));
			ServerList->AddChild(ServerDisplayRow);
		}
	}
}

void UMainMenu::ButtonEnableFunction() {
	FinalHost->OnClicked.AddDynamic(this, &UMainMenu::host);
	FinalJoin->OnClicked.AddDynamic(this, &UMainMenu::join);
//	refreshButton->OnClicked.AddDynamic(this, &UMainMenu::refresh);
}

void UMainMenu::ButtonDisableFunction() {
	FinalHost->OnClicked.RemoveDynamic(this, &UMainMenu::host);
	FinalJoin->OnClicked.RemoveDynamic(this, &UMainMenu::join);
//	refreshButton->OnClicked.RemoveDynamic(this, &UMainMenu::refresh);
}