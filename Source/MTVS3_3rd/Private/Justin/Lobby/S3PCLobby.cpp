// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Lobby/S3PCLobby.h"
#include "Justin/Lobby/LobbyWidget.h"
#include "Blueprint/UserWidget.h"

void AS3PCLobby::SetHost(bool _bIsHost)
{
	bIsHost = _bIsHost;
}

bool AS3PCLobby::GetHost() const
{
	return bIsHost;
}

void AS3PCLobby::RemovePlayer()
{
	if ( bIsHost && LobbyWidget )
	{
		LobbyWidget->RemovePlayer();
	}
}

void AS3PCLobby::AddPlayer()
{
	UE_LOG(LogTemp , Warning , TEXT("bIsHost : %d") , bIsHost);
	UE_LOG(LogTemp , Warning , TEXT("LobbyWidget : %d") , IsValid(LobbyWidget));
	if ( bIsHost && LobbyWidget )
	{
		UE_LOG(LogTemp , Warning , TEXT("AddpLayer Success"));
		LobbyWidget->AddPlayer();
	}
	else UE_LOG(LogTemp , Warning , TEXT("AddpLayer failed"));
}

void AS3PCLobby::HandleButtonPress()
{
	if ( HasAuthority() )
	{

	}
	else
	{

	}
}

void AS3PCLobby::What()
{
	UE_LOG(LogTemp , Warning , TEXT("RemoteRole: :%s") , *UEnum::GetValueAsString(GetRemoteRole()));
}

void AS3PCLobby::BeginPlay()
{
	Super::BeginPlay();
	
	if ( IsLocalPlayerController() )
	{
		LobbyWidget = CreateWidget<ULobbyWidget>(this , LobbyWidgetClass);
		UE_LOG(LogTemp , Warning , TEXT("S3PCLobby BeginPlay Host : %d") , bIsHost);
		if ( ensure(LobbyWidget))
		{
			LobbyWidget->Init(bIsHost);
			LobbyWidget->AddToViewport();
		}
	}
}
