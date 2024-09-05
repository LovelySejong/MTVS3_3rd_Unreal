// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Lobby/S3LobbyGMBase.h"
#include "Justin/Lobby/S3PCLobby.h"
#include "GameFramework/GameSession.h"
#include "Net/UnrealNetwork.h"
#include "Justin/S3GameInstance.h"

AS3LobbyGMBase::AS3LobbyGMBase()
{
	bHasHost = false;
	bIsFull = false;
	
}

//void AS3LobbyGMBase::PreLogin(const FString& Options , const FString& Address , const FUniqueNetIdRepl& UniqueId , FString& ErrorMessage)
//{
//	if ( !bIsFull )
//	{
//		UE_LOG(LogTemp , Warning , TEXT("Login success %s") , *UniqueId.ToString());
//		Super::PreLogin(Options , Address , UniqueId , ErrorMessage);
//	}
//	else
//	{
//		ErrorMessage = TEXT("Server is full. Try again later.");
//		UE_LOG(LogTemp , Warning , TEXT("Login Failed %s") , *UniqueId.ToString());
//	}
//}

void AS3LobbyGMBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if ( !Cast<AS3PCLobby>(NewPlayer)->IsHost() ) // is Client 
	{
		bIsFull = true;
		auto PC = Cast<AS3PCLobby>(HostPC);
		if ( PC )
		{
			PC->AddPlayer();
			bUseSeamlessTravel = true;
		}
		else UE_LOG(LogTemp , Warning , TEXT("ERROR"));
	}
	else HostPC = NewPlayer;
}

void AS3LobbyGMBase::Logout(AController* Exiting)
{
	auto PC = Cast<AS3PCLobby>(Exiting);
	if ( PC )
	{
		if ( PC->IsHost() )
		{
			auto GI = Cast<US3GameInstance>(GetWorld()->GetGameInstance());
			if ( GI ) GI->DestroyServer();
		}
		else
		{
			PC = Cast<AS3PCLobby>(HostPC);
			PC->RemovePlayer();
			bIsFull = false;
			bUseSeamlessTravel = false;
		}
	}

	Super::Logout(Exiting);
}

bool AS3LobbyGMBase::IsReadyToPlay()
{
	bool bIsReadyToStart = false;

	for ( FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it )
	{
		auto PC = Cast<AS3PCLobby>(*it);
		if ( PC->IsHost() ) continue;
		else bIsReadyToStart = PC->IsReady();
	}
	return bIsReadyToStart;
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

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}