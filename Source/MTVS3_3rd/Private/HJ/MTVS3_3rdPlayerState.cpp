// Fill out your copyright notice in the Description page of Project Settings.


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
		UE_LOG(LogTemp , Warning , TEXT("AMTVS3_3rdPlayerState::CopyProperties"));
		// 기존 PlayerState의 데이터를 새로운 PlayerState로 복사
		MyNewPlayerState->bIsHost = bIsHost;
		MyNewPlayerState->HostID = HostID;
		MyNewPlayerState->GuestID = GuestID;

		UE_LOG(LogTemp , Warning , TEXT("[%s] bIshost: %d, HostId: %s, GuestID: %s"), 
			*GetUniqueId().ToString(),
		bIsHost, *HostID, *GuestID);

		auto GS = Cast<AMTVS3_3rdGameState>(GetWorld()->GetGameState());
		GS->SetHostID(MyNewPlayerState->HostID);
		GS->SetGuestID(MyNewPlayerState->GuestID);
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
	UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdPlayerState::SetHostID(): %s") , *HostID);
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
	UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdPlayerState::SetGuestID(): %s") , *GuestID);
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
	DOREPLIFETIME(AMTVS3_3rdPlayerState , HostID);
	DOREPLIFETIME(AMTVS3_3rdPlayerState , GuestID);
}

void AMTVS3_3rdPlayerState::ServerRPCSetGuestId_Implementation(const FString& guestId)
{
	SetGuestID(guestId);
}

void AMTVS3_3rdPlayerState::ServerRPCSetHostId_Implementation(const FString& hostId)
{
	UE_LOG(LogTemp , Warning , TEXT("HostID Set in Server"));
	SetHostID(hostId);
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