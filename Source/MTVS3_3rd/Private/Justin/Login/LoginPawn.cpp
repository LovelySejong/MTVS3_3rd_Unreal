﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Login/LoginPawn.h"
#include "Justin/S3GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

// Sets default values
ALoginPawn::ALoginPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALoginPawn::BeginPlay()
{
	Super::BeginPlay();
	GI = GetWorld()->GetGameInstance<US3GameInstance>();

	if (GI && GI->SessionInterface.IsValid())
	{
		GI->SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ALoginPawn::OnCreateSessionCompleted);
		GI->SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ALoginPawn::OnJoinSessionCompleted);
		GI->SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ALoginPawn::OnDestroySessionComplete);
	}
}

void ALoginPawn::StartConnection()
{
	if (GI)
	{
		GI->SessionInterface->OnFindSessionsCompleteDelegates.Clear();
		GI->SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ALoginPawn::OnFindSessionsCompleted);
		GI->FindServer();
	}
}

void ALoginPawn::Test_CreateSession()
{
	GI->CreateServer();
}

void ALoginPawn::Test_JoinSession()
{
	GI->SessionInterface->OnFindSessionsCompleteDelegates.Clear();
	GI->SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ALoginPawn::Test_OnFindSessionsCompleted);
	GI->FindServer();
}

void ALoginPawn::Test_OnFindSessionsCompleted(bool bWasSuccessful)
{
	if (GI)
	{
		if (bWasSuccessful)
		{
			TArray<FOnlineSessionSearchResult> Results = GI->SessionSearch->SearchResults;
			UE_LOG(LogTemp, Warning, TEXT("Sessions %d"), Results.Num());
			if ( Results.Num() )
			{
				auto nice = GetPlayerState()->GetUniqueId();
				UE_LOG(LogTemp , Warning , TEXT("Player UniqueId %s") , *nice.ToString());
				GI->JoinServer(0);
			}
		}
	}
}

void ALoginPawn::OnFindSessionsCompleted(bool bWasSuccessful)
{
	if (GI)
	{
		if (bWasSuccessful)
		{
			TArray<FOnlineSessionSearchResult> Results = GI->SessionSearch->SearchResults;
			UE_LOG(LogTemp, Warning, TEXT("Sessions %d"), Results.Num());
			if (Results.Num() != 0)
			{
				bool bJoiningServer = false;
				for (int i = 0; i < Results.Num(); ++i)
				{
					UE_LOG(LogTemp, Warning, TEXT("OwningUserName %s"), *Results[i].Session.OwningUserName);
					//UE_LOG(LogTemp , Warning , TEXT("OwningUserName %s") , *Results[i].Session.SessionInfo);

					int32 AvailablePublicSpots = Results[i].Session.NumOpenPublicConnections;
					int32 MaxPublicSpots = Results[i].Session.SessionSettings.NumPublicConnections;
					int32 AvailablePrivateSpots = Results[i].Session.NumOpenPrivateConnections;
					int32 MaxPrivateSpots = Results[i].Session.SessionSettings.NumPrivateConnections;
					UE_LOG(LogTemp, Warning, TEXT("NumOpenPublicConnections %d"), AvailablePublicSpots);
					UE_LOG(LogTemp, Warning, TEXT("NumPublicConnections %d"), MaxPublicSpots);
					UE_LOG(LogTemp, Warning, TEXT("NumOpenPrivateConnections %d"), AvailablePrivateSpots);
					UE_LOG(LogTemp, Warning, TEXT("NumPrivateConnections %d"), MaxPrivateSpots);
					UE_LOG(LogTemp, Warning, TEXT("Im inside OnFindSessionCompleted, %s"), *GetPlayerState()->GetUniqueId().ToString());
					UE_LOG(LogTemp, Warning, TEXT("----------------------------"));

					if (AvailablePublicSpots <= 0) continue;

					GI->JoinServer(i);
					bJoiningServer = true;
					break;
				}
				if (!bJoiningServer) GI->CreateServer();
			}
			else GI->CreateServer();
		}
		else UE_LOG(LogTemp, Warning, TEXT("Find Session Unsuccessful"));
	}
}

void ALoginPawn::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Create Session successful"));
		GetWorld()->ServerTravel(TEXT("/Game/Justin/Lobby/LobbyLevel?listen")); //Matchmaking Test Level
		//GetWorld()->ServerTravel(TEXT("/Game/Justin/VoiceChat/Level_VoiceChatTest?listen")); //Voice Chat Test Level
	}
	else UE_LOG(LogTemp, Warning, TEXT("Create Session Unsuccessful"));
}

void ALoginPawn::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete"));

		switch (Result)
		{
		case EOnJoinSessionCompleteResult::Success:
		{
			if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				FString JoinAddress;

				GI->SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
				UE_LOG(LogTemp, Warning, TEXT("Joining Session %s with Address %s"), *SessionName.ToString(), *JoinAddress);
				if (JoinAddress != "") PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
			}
			break;
		}
		case EOnJoinSessionCompleteResult::SessionIsFull:
		{
			UE_LOG(LogTemp, Warning, TEXT("Session is Full"));

			break;
		}
		case EOnJoinSessionCompleteResult::AlreadyInSession:
		{
			UE_LOG(LogTemp, Warning, TEXT("Already in session"));

			break;
		}
		default:
		{
			UE_LOG(LogTemp, Warning, TEXT("Default join result"));
			break;
		}
		}
	}
}

void ALoginPawn::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete Success -- %s"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete Fail"));
	}
}

