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
 */
UCLASS()
class MULTIPLAYERTEST_API UAnotherGameinstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UAnotherGameinstance();

    TSubclassOf<UUserWidget> ServerDisplayRowClass;

    UFUNCTION(EXEC)
    void host(FString sessionName);

    UFUNCTION(EXEC)
    void join(int32 Index);




    //Delegates
    void OnFindSessionComplete(bool Succeeded);
    void OnCreateSessionComplete(FName SessionName, bool wasSuccessful);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
    void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

protected:

    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    IOnlineSessionPtr SessionInterface;

    FName SESSION_NAME = FName("My Game");

    FName testKey = FName("ServerKey");
    FName testValue = FName("ServerName");



    void Init() override;

    
private:

    TSubclassOf<UUserWidget> MenuBPClass;


};
