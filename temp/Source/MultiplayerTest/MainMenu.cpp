#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

UCLASS()
class MULTIPLAYERTEST_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	// Initializes the menu with server data
	UFUNCTION(BlueprintCallable)
	void SetServerList(const TArray<FServerData>& ServerData);

	// Selects a specific server by index
	void SelectIndex(int32 Index);

	// Notify function called when a server row is clicked
	void NotifyParent(int32 Index);

protected:
	// Called to bind functionality to UI elements
	virtual bool Initialize() override;

private:
	// UI Bindings
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenuWidget;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenuWidget;

	// Current selected index
	TOptional<int32> SelectedIndex;

	// Utility function to update the server rows
	void UpdateChildren();


	// Button click handlers
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();
};
