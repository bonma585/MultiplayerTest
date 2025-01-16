#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerDisplayRow.generated.h"

UCLASS()
class MULTIPLAYERTEST_API UServerDisplayRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ServerName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PlayerOwner;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CurrentPlayerNumber;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* MaxPlayerNumber;

	UPROPERTY(meta = (BindWidget))
	class UButton* ServerNameButton;

	UPROPERTY(BlueprintReadOnly)
	bool Selected;

	void SetParent(class UMainMenu* Menu);
	void SetUniqueIndex(int32 Index);

	UFUNCTION()
	void NotifyParent();

	void SetSelected(bool InSelected);

private:
	UMainMenu* Parent = nullptr;
	int32 UniqueIndex = -1;


protected:
	virtual bool Initialize() override;
};
