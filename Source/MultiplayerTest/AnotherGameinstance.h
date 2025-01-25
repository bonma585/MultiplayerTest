/*
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Engine/GameInstance.h"
#include "AnotherGameinstance.generated.h"

/**
 * 
 
UCLASS()
class MULTIPLAYERTEST_API UAnotherGameinstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UAnotherGameinstance();

protected:

    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    IOnlineSessionPtr SessionInterface;

    FName SESSION_NAME = FName("My Game");

    FName testKey = FName("ServerKey");
    FName testValue = FName("ServerName");



    void Init() override;

    UFUNCTION(BlueprintCallable)
    void CreateServer();
    UFUNCTION(BlueprintCallable)
    void JoinServer();

    //Delegates
    void OnCreateSessionComplete(FName SessionName, bool wasSuccessful);
    void OnFindSessionComplete(bool Succeeded);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
    void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);




};
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
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
    void FindSessions();

    UFUNCTION(BlueprintCallable)
    void Join(int32 ServerIndex);

    // Callbacks
    UFUNCTION()
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

    UFUNCTION()
    void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

    UFUNCTION()
    void OnFindSessionsComplete(bool bWasSuccessful);

    UFUNCTION()
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
    void SetupOnlineSession();
    void UpdateMainMenuServerList(const TArray<FServerData>& ServerData);

    IOnlineSessionPtr SessionInterface;
    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    // Name of server we intend to create
    FString DesiredServerName;
};
