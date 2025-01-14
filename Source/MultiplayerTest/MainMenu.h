// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "AnotherGameinstance.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BLUEPRINTREADWRITE, meta = (BindWidget))
	UButton* FinalHost;

	UPROPERTY(BLUEPRINTREADWRITE, meta = (BindWidget))
	UButton* FinalJoin;
/*
	UPROPERTY(BLUEPRINTREADWRITE, meta = (BindWidget))
	UButton* refreshButton;
*/

	UPROPERTY(meta = (BindWidget))
	class UEditableText* ServerNameText;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList;


	void NativeConstruct() override;
	void NativeDestruct() override;

	class UAnotherGameinstance* AnotherGameinstance;
	void SetGameInstance(UAnotherGameinstance* gameInstance);

	int selectedIndex;
	void notifyParent(int32 index);

	void UpdateServerList(TArray<FServerData> ServerNames);

private:
	UFUNCTION()
	void host();

	UFUNCTION()
	void join();

	UFUNCTION()
	void refresh();

	void ButtonEnableFunction();
	void ButtonDisableFunction();
};


USTRUCT()
struct FServerData {
	GENERATED_BODY()

	FString ServerName, PlayerName;
	int32 CurrentPlayerNumber, MaxPlayerNumber;
};
