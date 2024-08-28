// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/S3GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Kismet/GameplayStatics.h"

US3GameInstance::US3GameInstance()
{
	
}

void US3GameInstance::Init()
{
	if ( IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get() )
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if ( SessionInterface.IsValid() )
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this , &US3GameInstance::OnCreateSessionCompleted);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this , &US3GameInstance::OnFindSessionsCompleted);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this , &US3GameInstance::OnJoinSessionCompleted);
		}
	}

	Super::Init();
}

void US3GameInstance::CreateServer()
{
	FOnlineSessionSettings SessionSettings; 
	SessionSettings.bAllowJoinInProgress = false;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = true; 
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 2;

	SessionInterface->CreateSession(0 , "MySession" , SessionSettings);
}

void US3GameInstance::OnCreateSessionCompleted(FName SessionName , bool bWasSuccessful)
{
	if ( bWasSuccessful )
	{
		GetWorld()->ServerTravel(TEXT("/Game/KHJ/Maps/KHJ_FirstPersonMap?listen"));
	}
	else UE_LOG(LogTemp , Warning , TEXT("Create Session Unsuccessful"));
}

void US3GameInstance::JoinServer()
{
	UE_LOG(LogTemp , Warning , TEXT("Join Session Start "));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE , true , EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void US3GameInstance::OnFindSessionsCompleted(bool bWasSuccessful)
{
	if ( bWasSuccessful )
	{
		UE_LOG(LogTemp , Warning , TEXT("Found Session"));

		TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
		UE_LOG(LogTemp , Warning , TEXT("Sessions %d"), Results.Num());
		if (Results.Num())
		{
			UE_LOG(LogTemp , Warning , TEXT("Joining Server"));
			SessionInterface->JoinSession(0, "MySession", Results[0]);
		}
	}
	else UE_LOG(LogTemp , Warning , TEXT("Find Session Unsuccessful"));
}

void US3GameInstance::OnJoinSessionCompleted(FName SessionName , EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp , Warning , TEXT("OnJoinSessionComplete"));
	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress;
		SessionInterface->GetResolvedConnectString(SessionName , JoinAddress);
		if(JoinAddress != "") PController->ClientTravel(JoinAddress , ETravelType::TRAVEL_Absolute);
	}
}

