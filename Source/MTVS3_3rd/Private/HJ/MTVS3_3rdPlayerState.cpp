﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/MTVS3_3rdPlayerState.h"
#include "Justin/Lobby/S3PCLobby.h"
#include "Net/UnrealNetwork.h"
#include "Justin/Lobby/LobbyWidget.h"
#include "HJ/MTVS3_3rdGameState.h"

#pragma region HJ 
void AMTVS3_3rdPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
	Super::CopyProperties(NewPlayerState);

	if ( AMTVS3_3rdPlayerState* MyNewPlayerState = Cast<AMTVS3_3rdPlayerState>(NewPlayerState) )
	{
		// 기존 PlayerState의 데이터를 새로운 PlayerState로 복사
		MyNewPlayerState->bIsHost = bIsHost;
	}
}

void AMTVS3_3rdPlayerState::SetHost(bool _bIsHost)
{
	bIsHost = _bIsHost;
}

void AMTVS3_3rdPlayerState::SetHostNickname(const FString& hostNickname)
{
	HostNickname = hostNickname;
}

FString AMTVS3_3rdPlayerState::GetHostNickname() const
{
	if ( HostNickname.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("HostNickname is empty, returning default value."));
		return TEXT("Host");
	}
	return HostNickname;
}

void AMTVS3_3rdPlayerState::SetGuestNickname(const FString& guestNickname)
{
	GuestNickname = guestNickname;
}

FString AMTVS3_3rdPlayerState::GetGuestNickname() const
{
	if ( GuestNickname.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("GuestNickname is empty, returning default value."));
		return TEXT("Guest");
	}
	return GuestNickname;
}

void AMTVS3_3rdPlayerState::SetHostID(const FString& hostID)
{
	HostID = hostID;
}

FString AMTVS3_3rdPlayerState::GetHostID() const
{
	if ( HostID.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("HostID is empty, returning default value."));
		return TEXT("HostID");
	}
	return HostID;
}
void AMTVS3_3rdPlayerState::SetGuestID(const FString& guestID)
{
	GuestID = guestID;
}
FString AMTVS3_3rdPlayerState::GetGuestID() const
{
	if ( GuestID.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("GuestID is empty, returning default value."));
		return TEXT("GuestID");
	}
	return GuestID;
}
#pragma endregion

# pragma region HJ 멀티플레이
void AMTVS3_3rdPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMTVS3_3rdPlayerState , bIsHost);
}

// Host 닉네임 RPC
void AMTVS3_3rdPlayerState::ServerRPCSetHostNickname_Implementation(const FString& hostName)
{
	// 서버는 hostName을 정하고
	SetHostNickname(hostName);

	auto GS = Cast<AMTVS3_3rdGameState>(GetWorld()->GetGameState());
	GS->SetHostNickname(GetHostNickname());
}

// Guest 닉네임 RPC
void AMTVS3_3rdPlayerState::ServerRPCSetGuestNickname_Implementation(const FString& guestName)
{
	// 서버는 guestName을 정하고
	SetGuestNickname(guestName);

	auto GS = Cast<AMTVS3_3rdGameState>(GetWorld()->GetGameState());
	GS->SetGuestNickname(GetGuestNickname());
}
#pragma endregion