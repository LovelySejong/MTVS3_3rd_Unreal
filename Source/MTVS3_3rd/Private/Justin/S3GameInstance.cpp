// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/S3GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include <HJ/MTVS3_3rdGameState.h>


US3GameInstance::US3GameInstance()
{

}

TStatId US3GameInstance::GetStatId() const
{
	return TStatId();
}

void US3GameInstance::Tick(float DeltaTime)
{
	TotalTime += DeltaTime;
	if ( TotalTime > MaxTime )
	{
		TotalTime = 0.f;
		if ( SessionInterface )
		{
			FString Str = EOnlineSessionState::ToString(SessionInterface->GetSessionState("Justin's Session"));
			UE_LOG(LogTemp , Warning , TEXT("Session State: %s") , *Str);
		}
	}
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
	SessionSettings.Set("sPRsaUKm" , FString("f2WT04QT") , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//SessionSettings.BuildUniqueId = 123;

	ULocalPlayer* Local = GetWorld()->GetFirstLocalPlayerFromController();
	FString Test = GetWorld()->GetFirstPlayerController()->PlayerState->GetUniqueId().ToString();
	UE_LOG(LogTemp , Warning , TEXT("LocalPlayer: %s , PlayerState: %s") , *Local->GetPreferredUniqueNetId().ToString() , *Test); //Same Unique Id
	SessionInterface->CreateSession(*Local->GetPreferredUniqueNetId() , "Justin's Session" , SessionSettings);
	//SessionInterface->CreateSession(0 , "Justin's Session" , SessionSettings);
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

void US3GameInstance::StartSession()
{
	if ( SessionInterface )
	{
		SessionInterface->OnStartSessionCompleteDelegates.AddUObject(this , &US3GameInstance::OnStartSessionComplete);
		SessionInterface->StartSession("Justin's Session");
	}
}

void US3GameInstance::OnStartSessionComplete(FName SessionName , bool bWasSuccessful)
{
	if ( bWasSuccessful )
	{
		UE_LOG(LogTemp , Warning , TEXT("Start Session Success! SessionName %s") , *SessionName.ToString());

		//auto GM = GetWorld()->GetAuthGameMode();

		//if ( GM )
		//{
		//	auto GS = GM->GetGameState<AMTVS3_3rdGameState>();
		//	if ( GS )
		//	{
		//		UE_LOG(LogTemp , Warning , TEXT("bTest :%d") , GS->bTest);
		//		GS->bTest = true;
		//	}
		//}

		//GetWorld()->ServerTravel(FString::Printf(TEXT("/Game/LovelySejong/PlayLevel?listen?IsHost=%s") , bIsHost));
		
		GetWorld()->ServerTravel("/Game/LovelySejong/PlayLevel?listen");
	}
}

void US3GameInstance::DestroyServer()
{
	SessionInterface->DestroySession("Justin's Session");
}

#pragma region HJ
void US3GameInstance::SetHost(bool _bIsHost)
{
	bIsHost = _bIsHost;
	//PlayerNum += PlayerCount;
	//bIsHost = PlayerNum == 2 ? 1 : 0;
	//UE_LOG(LogTemp , Warning , TEXT("===== Player Num is %d"), PlayerNum);
}

void US3GameInstance::SetAccessToken(const FString& InAccessToken)
{
	AccessToken = InAccessToken;
}

void US3GameInstance::SetPlayerNickname(const FString& Nickname)
{
	if ( Nickname.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("Nickname is empty, setting to default value."));
		PlayerNickname = TEXT("Player")	;
	}
	else
	{
		PlayerNickname = Nickname;
	}
}

FString US3GameInstance::GetPlayerNickname() const
{
	if ( PlayerNickname.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("PlayerNickname is empty, returning default value."));
		return TEXT("Player");
	}
	return PlayerNickname;
}
#pragma endregion