
#include "ServerDisplayRow.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MainMenu.h"
#include "Components/ScrollBox.h"

void UServerDisplayRow::SetParent(UMainMenu* Menu)
{
	Parent = Menu;
}

void UServerDisplayRow::SetUniqueIndex(int32 Index)
{
	UniqueIndex = Index;
}

void UServerDisplayRow::NotifyParent()
{
	UE_LOG(LogTemp, Warning, TEXT("Server row clicked: Index = %d"), UniqueIndex);
	if (Parent)
	{
		Parent->NotifyParent(UniqueIndex);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Parent is null in ServerDisplayRow."));
	}
}

void UServerDisplayRow::SetSelected(bool InSelected)
{
	Selected = InSelected;
	ServerNameButton->ColorAndOpacity = Selected ? FLinearColor(1.0f, 1.0f, 0.8f, 1.0f) : FLinearColor::White;
}

bool UServerDisplayRow::Initialize()
{
	if (!Super::Initialize()) { return false; }
	if (!ensure(ServerNameButton != nullptr)) { return false; }

	ServerNameButton->OnClicked.AddDynamic(this, &UServerDisplayRow::NotifyParent);
	return true;
}
