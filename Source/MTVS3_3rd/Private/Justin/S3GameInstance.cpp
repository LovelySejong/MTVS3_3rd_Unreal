// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/S3GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Kismet/GameplayStatics.h"

US3GameInstance::US3GameInstance()
{

}

TStatId US3GameInstance::GetStatId() const
{
	return TStatId();
}

void US3GameInstance::Tick(float DeltaTime)
{
	//if ( SessionInterface )
	//{		
	//	FString Str = EOnlineSessionState::ToString(SessionInterface->GetSessionState("Justin's Session"));
	//	UE_LOG(LogTemp , Warning , TEXT("Session State: %s") , *Str);
	//}
}

void US3GameInstance::Init()
{
	if ( IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get() )
	{
		SessionInterface = SubSystem->GetSessionInterface();
	}
	Super::Init();
}

void US3GameInstance::Shutdown()
{
	DestroyServer();

	Super::Shutdown();
}

void US3GameInstance::CreateServer()
{
	UE_LOG(LogTemp , Warning , TEXT("Creating session Start "));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = false;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = true;

	/*if ( IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" )
	else
		SessionSettings.bIsLANMatch = false;*/

	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 2;

	SessionInterface->CreateSession(0 , "Justin's Session" , SessionSettings);
}

void US3GameInstance::FindServer()
{
	UE_LOG(LogTemp , Warning , TEXT("Find Sessions Start "));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true;
	/*if ( IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" )
		SessionSearch->bIsLanQuery = true;
	else
		SessionSearch->bIsLanQuery = false;*/

	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE , true , EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0 , SessionSearch.ToSharedRef());
}

void US3GameInstance::JoinServer(int32 Index)
{
	TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
	UE_LOG(LogTemp , Warning , TEXT("Joining Server %s") , *Results[Index].GetSessionIdStr());
	SessionInterface->JoinSession(0 , "Justin's Session" , Results[Index]);
}

void US3GameInstance::DestroyServer()
{
	SessionInterface->DestroySession("Justin's Session");
}

