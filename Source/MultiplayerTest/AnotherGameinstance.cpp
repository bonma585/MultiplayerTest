#include "AnotherGameInstance.h"
#include "OnlineSubsystem.h"
#include "MainMenu.h"
#include "ServerDisplayRow.h"

UAnotherGameInstance::UAnotherGameInstance()
{
    SetupOnlineSession();

}

void UAnotherGameInstance::Init()
{
    Super::Init();
}

void UAnotherGameInstance::SetupOnlineSession()
{
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        SessionInterface = OnlineSubsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UAnotherGameInstance::OnCreateSessionComplete);
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UAnotherGameInstance::OnDestroySessionComplete);
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UAnotherGameInstance::OnFindSessionsComplete);
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UAnotherGameInstance::OnJoinSessionComplete);
        }
    }
}

void UAnotherGameInstance::Host(const FString& ServerName)
{
    DesiredServerName = ServerName;

    if (SessionInterface.IsValid())
    {
        FOnlineSessionSettings SessionSettings;
        SessionSettings.bIsLANMatch = false;
        SessionSettings.NumPublicConnections = 5;
        SessionSettings.bAllowJoinInProgress = true;
        SessionSettings.bShouldAdvertise = true;
        SessionSettings.bUsesPresence = true;

        SessionInterface->CreateSession(0, FName(TEXT("MySession")), SessionSettings);
    }
}

void UAnotherGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            World->ServerTravel("/Game/Maps/Mansion?listen");
        }
    }
}

void UAnotherGameInstance::FindSessions()
{
    if (SessionInterface.IsValid())
    {
        SessionSearch = MakeShareable(new FOnlineSessionSearch());
        SessionSearch->bIsLanQuery = false;
        SessionSearch->MaxSearchResults = 100;
        SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

        SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
    }
}

void UAnotherGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
    TArray<FServerData> ServerDataArray;

    if (bWasSuccessful && SessionSearch.IsValid())
    {
        for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
        {
            FServerData ServerData;
            ServerData.ServerName = SearchResult.Session.OwningUserName;
            ServerData.HostName = SearchResult.Session.OwningUserName;
            ServerData.CurrentPlayers = SearchResult.Session.SessionSettings.NumPublicConnections -
                SearchResult.Session.NumOpenPublicConnections;
            ServerData.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;

            ServerDataArray.Add(ServerData);
        }
    }

    UpdateMainMenuServerList(ServerDataArray);
}

void UAnotherGameInstance::UpdateMainMenuServerList(const TArray<FServerData>& ServerData)
{
    if (APlayerController* PlayerController = GetFirstLocalPlayerController())
    {
        UMainMenu* MainMenu = Cast<UMainMenu>(PlayerController->GetHUD());
        if (MainMenu)
        {
            MainMenu->SetServerList(ServerData);
        }
    }
}

void UAnotherGameInstance::Join(int32 ServerIndex)
{
    if (SessionInterface.IsValid() && SessionSearch.IsValid() && SessionSearch->SearchResults.IsValidIndex(ServerIndex))
    {
        SessionInterface->JoinSession(0, FName(TEXT("MySession")), SessionSearch->SearchResults[ServerIndex]);
    }
}

void UAnotherGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (APlayerController* PlayerController = GetFirstLocalPlayerController())
    {
        FString ConnectString;
        if (SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
        {
            PlayerController->ClientTravel(ConnectString FTravelType::TRAVEL_Absolute)
        }
    }
}








   