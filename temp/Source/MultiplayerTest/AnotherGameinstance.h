#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "OnlineSubsystem.h" 
#include "AnotherGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FServerData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString ServerName;

    UPROPERTY(BlueprintReadWrite)
    FString HostName;

    UPROPERTY(BlueprintReadWrite)
    int32 CurrentPlayers;

    UPROPERTY(BlueprintReadWrite)
    int32 MaxPlayers;
};



UCLASS()
class MULTIPLAYERTEST_API UAnotherGameInstance : public UGameInstance
{
    GENERATED_BODY()


public:
    UAnotherGameInstance();

    virtual void Init() override;

    UFUNCTION(BlueprintCallable)
    void Host(const FString& ServerName);

    UFUNCTION(BlueprintCallable)
    void Join(int32 ServerIndex);

    UFUNCTION(BlueprintCallable)
    void FindSessions();

    UFUNCTION()
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

    UFUNCTION()
    void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

    UFUNCTION()
    void OnFindSessionsComplete(bool bWasSuccessful);

    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    
private:
    void SetupOnlineSession();

    IOnlineSessionPtr SessionInterface;
    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    FString DesiredServerName;

    // Updates the server list in MainMenu
    void UpdateMainMenuServerList(const TArray<FServerData>& ServerData);
};
