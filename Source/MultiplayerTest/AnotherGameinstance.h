
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Engine/GameInstance.h"
#include "AnotherGameinstance.generated.h"

UCLASS()
class MULTIPLAYERTEST_API UAnotherGameinstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UAnotherGameinstance();


    UFUNCTION(BlueprintCallable)
    void ReturnToMainMenu();

    UFUNCTION()
    void TravelToMainMenu();


    IOnlineSessionPtr SessionInterface;

    bool bReturnToMenu = false;

protected:

    TSharedPtr<FOnlineSessionSearch> SessionSearch;


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
