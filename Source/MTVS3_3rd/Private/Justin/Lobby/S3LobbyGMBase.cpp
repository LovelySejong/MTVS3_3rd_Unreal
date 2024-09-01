// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Lobby/S3LobbyGMBase.h"
#include "Justin/Lobby/S3PCLobby.h"
#include "GameFramework/GameSession.h"

AS3LobbyGMBase::AS3LobbyGMBase()
{
	bHasHost = false;
	bIsFull = false;
}

void AS3LobbyGMBase::PreLogin(const FString& Options , const FString& Address , const FUniqueNetIdRepl& UniqueId , FString& ErrorMessage)
{
	UE_LOG(LogTemp , Warning , TEXT("Im inside PreLogin, nice %s"), *UniqueId.ToString());
	
	if ( !bIsFull )
	{
		Super::PreLogin(Options , Address , UniqueId , ErrorMessage);
	}
}

void AS3LobbyGMBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp , Warning , TEXT("PostLogin Host %d") , Cast<AS3PCLobby>(NewPlayer)->GetHost());

	if ( !Cast<AS3PCLobby>(NewPlayer)->GetHost() ) // is Client 
	{
		bIsFull = true;
		UE_LOG(LogTemp , Warning , TEXT("1111"));
		auto PC = Cast<AS3PCLobby>(HostPC);
		if ( PC )
		{
			UE_LOG(LogTemp , Warning , TEXT("2222"));
			PC->AddPlayer();
		}
		else UE_LOG(LogTemp , Warning , TEXT("ERROR"));
	}
	else HostPC = NewPlayer;
}

void AS3LobbyGMBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	auto PC = CastChecked<AS3PCLobby>(NewPlayer);
	if ( PC )
	{
		if ( bHasHost )
		{
			PC->SetHost(false);
		}
		else
		{
			bHasHost = true;
			PC->SetHost(true);
		}
	}
	UE_LOG(LogTemp , Warning , TEXT("HandleStartingNewPlayer Called"));

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}
