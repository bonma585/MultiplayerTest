// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerDisplayRow.generated.h"

/**
 *
 */
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

	void setParent(class UMainMenu* Menu);
	void setUniqueIndex(int32 Index);

	UFUNCTION()
	void notifyParent();

	void setSelected(bool inSelected);

private:

	UMainMenu* Parent;
	int32 UniqueIndex;



protected:
	virtual bool Initialize();
};
