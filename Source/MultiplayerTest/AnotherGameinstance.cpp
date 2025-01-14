// Fill out your copyright notice in the Description page of Project Settings.


#include "AnotherGameinstance.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerTestPlayerController.h"
#include "Online/OnlineSessionNames.h"
#include <Online/OnlineSessionNames.h>
#include "Blueprint/UserWidget.h"


UAnotherGameinstance::UAnotherGameinstance()
{
    // Ensure UserWidget class is properly initialized
    static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClassFinder(TEXT("/Game/TopDown/Maps/WBP_MainMenu"));
    if (MenuBPClassFinder.Succeeded()) {
        MenuBPClass = MenuBPClassFinder.Class;
    }

    // ServerDisplayRowClass
    static ConstructorHelpers::FClassFinder<UUserWidget> ServerDisplayRowClassFinder(TEXT("/Game/TopDown/Maps/WBP_ServerDisplayRow"));
    if (ServerDisplayRowClassFinder.Succeeded()) {
        ServerDisplayRowClass = ServerDisplayRowClassFinder.Class;
    }
}

void UAnotherGameinstance::Init()
{
    Super::Init();

    if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get()) {
        SessionInterface = Subsystem->GetSessionInterface();
        if (SessionInterface.IsValid()) {
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UAnotherGameinstance::OnCreateSessionComplete);
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UAnotherGameinstance::OnFindSessionComplete);
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UAnotherGameinstance::OnJoinSessionComplete);
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UAnotherGameinstance::OnDestroySessionComplete);

        }
    }
}

void UAnotherGameinstance::host(FString sessionName)
{
    UE_LOG(LogTemp, Warning, TEXT("Create Server"));

    if (!SessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("SessionInterface is invalid!"));
        return;
    }

    // Check if a session already exists
    if (SessionInterface->GetNamedSession(SESSION_NAME) != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("A session already exists. Destroying the existing session."));
        SessionInterface->DestroySession(SESSION_NAME);
        return; // The `OnDestroySessionComplete` delegate should handle calling `CreateServer` again
    }

    // Configure session settings
    FOnlineSessionSettings SessionSettings;
    FName SubsystemName = IOnlineSubsystem::Get()->GetSubsystemName();

    if (SubsystemName == "NULL") // LAN configuration
    {
        SessionSettings.bIsLANMatch = true;
        SessionSettings.bShouldAdvertise = true;
        SessionSettings.NumPublicConnections = 5;
        SessionSettings.bAllowJoinInProgress = true;
    }
    else if (SubsystemName == "Steam") // Steam configuration
    {
        SessionSettings.bIsLANMatch = false;
        SessionSettings.bShouldAdvertise = true;
        SessionSettings.bUsesPresence = true;
        SessionSettings.bUseLobbiesIfAvailable = true;
        SessionSettings.NumPublicConnections = 5;
        SessionSettings.bAllowJoinInProgress = true;
        SessionSettings.bAllowJoinViaPresence = true;
    }

    UE_LOG(LogTemp, Warning, TEXT("Logging Session Settings:"));
    for (const auto& Setting : SessionSettings.Settings)
    {
        FString Key = Setting.Key.ToString();
        FString Value;
        Setting.Value.Data.GetValue(Value);
        UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *Key, *Value);
    }

    // Create the session
    if (!SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings))
    {
        UE_LOG(LogTemp, Error, TEXT("CreateSession failed to start!"));
    }
}


void UAnotherGameinstance::join(int32 Index)
{
    if (SessionInterface.IsValid() && SessionSearch.IsValid() &&
        Index >= 0 && Index < SessionSearch->SearchResults.Num()) {
        const FOnlineSessionSearchResult& SearchResult = SessionSearch->SearchResults[Index];
        if (SessionInterface->JoinSession(0, "SessionName", SearchResult)) {
            UE_LOG(LogTemp, Warning, TEXT("Successfully joined session at index %d."), Index);
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Failed to join session at index %d."), Index);
        }
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Invalid session index or session search invalid."));
    }
}


void UAnotherGameinstance::OnCreateSessionComplete(FName SessionName, bool bSucceeded)
{
    if (bSucceeded) {
        FString SessionId = SessionInterface->GetNamedSession(SESSION_NAME)->GetSessionIdStr();
        UE_LOG(LogTemp, Warning, TEXT("Session ID: %s"), *SessionId);

        // Ensure ServerTravel is called correctly for both cases
        GetWorld()->ServerTravel("/Game/TopDown/Maps/Mansion?listen");
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Failed to create session"));
    }
}

void UAnotherGameinstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
        FString JoinAddress = "";
        SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);

        if (JoinAddress != "") {
            PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
        }
    }
}

void UAnotherGameinstance::OnFindSessionComplete(bool bSucceeded)
{
    if (!bSucceeded) {
        return;
    }

    TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

    if (SearchResults.Num()) {
        UE_LOG(LogTemp, Warning, TEXT("LISTING SESSIONS"));
        for (const FOnlineSessionSearchResult& SearchResult : SearchResults) {
            UE_LOG(LogTemp, Warning, TEXT("Owning User Name: %s"), *FString(SearchResult.Session.OwningUserName));
        }

        // Join the first found session
        SessionInterface->JoinSession(0, SESSION_NAME, SearchResults[0]);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("No sessions found"));
    }
}


void UAnotherGameinstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful) {
        // After destroying the session, create a new one
        host("DefaultSessionName");
    }

}

