// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/MTVS3_3rdPlayerState.h"
#include "Justin/Lobby/S3PCLobby.h"
#include "Net/UnrealNetwork.h"
#include "Justin/Lobby/LobbyWidget.h"
#include "HJ/MTVS3_3rdGameState.h"
#include "HJ/HttpActor.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/S3GameInstance.h"

#pragma region HJ 
void AMTVS3_3rdPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
	Super::CopyProperties(NewPlayerState);

	if ( AMTVS3_3rdPlayerState* MyNewPlayerState = Cast<AMTVS3_3rdPlayerState>(NewPlayerState) )
	{
		UE_LOG(LogTemp , Warning , TEXT("AMTVS3_3rdPlayerState::CopyProperties"));
		// 기존 PlayerState의 데이터를 새로운 PlayerState로 복사
		MyNewPlayerState->bIsHost = bIsHost;
		//MyNewPlayerState->HostID = HostID;
		//MyNewPlayerState->GuestID = GuestID;

		//UE_LOG(LogTemp , Warning , TEXT("[%s] bIshost: %d, HostId: %s, GuestID: %s"), 
		//	*GetUniqueId().ToString(),
		//bIsHost, *HostID, *GuestID);

		//auto GS = Cast<AMTVS3_3rdGameState>(GetWorld()->GetGameState());
		//GS->SetHostID(MyNewPlayerState->HostID);
		//GS->SetGuestID(MyNewPlayerState->GuestID);
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

void AMTVS3_3rdPlayerState::SetHostToken(const FString& hostToken)
{
	HostToken = hostToken;
	UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdPlayerState::SetHostToken(): %s") , *HostToken);
}

FString AMTVS3_3rdPlayerState::GetHostToken() const
{
	if ( HostToken.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("HostToken is empty, returning default value."));
		return TEXT("HostToken");
	}
	return HostToken;
}

void AMTVS3_3rdPlayerState::SetGuestToken(const FString& guestToken)
{
	GuestToken = guestToken;
	UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdPlayerState::SetGuestToken(): %s") , *GuestToken);
}

FString AMTVS3_3rdPlayerState::GetGuestToken() const
{
	if ( GuestToken.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("GuestToken is empty, returning default value."));
		return TEXT("GuestToken");
	}
	return GuestToken;
}
#pragma endregion

# pragma region HJ 멀티플레이
void AMTVS3_3rdPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMTVS3_3rdPlayerState , bIsHost);
	DOREPLIFETIME(AMTVS3_3rdPlayerState , HostID);
	DOREPLIFETIME(AMTVS3_3rdPlayerState , GuestID);
	DOREPLIFETIME(AMTVS3_3rdPlayerState , HostToken);
	DOREPLIFETIME(AMTVS3_3rdPlayerState , GuestToken);
}

void AMTVS3_3rdPlayerState::OnResPostMatchState(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bWasSuccessful)
{
	if ( !bWasSuccessful || !Response.IsValid() )
	{
		UE_LOG(LogTemp , Error , TEXT("Request failed."));
		return;
	}

	// 응답 상태 코드 확인
	if ( Response->GetResponseCode() != 200 )
	{
		UE_LOG(LogTemp , Error , TEXT("Request failed with status code: %d") , Response->GetResponseCode());
		return;
	}

	// 응답 본문을 로그에 출력
	UE_LOG(LogTemp , Log , TEXT("Response Code: %d") , Response->GetResponseCode());
	UE_LOG(LogTemp , Log , TEXT("Response Body: %s") , *Response->GetContentAsString());

	// JSON 응답 파싱
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if ( FJsonSerializer::Deserialize(Reader , JsonObject) )
	{
		// 'response' 객체에서 'gameId' 값을 가져옴
		if ( JsonObject->HasTypedField<EJson::Object>(TEXT("response")) )
		{
			TSharedPtr<FJsonObject> ResponseObject = JsonObject->GetObjectField(TEXT("response"));
			if ( ResponseObject->HasTypedField<EJson::Number>(TEXT("gameId")) )
			{
				int32 GameID = ResponseObject->GetIntegerField(TEXT("gameId"));
				UE_LOG(LogTemp , Log , TEXT("gameId received: %d") , GameID);

				// GameInstance에 GameID 저장
				auto GI = GetWorld()->GetGameInstance<US3GameInstance>();
				if ( GI )
				{
					GI->SetGameID(GameID);
				}
				else
				{
					UE_LOG(LogTemp , Error , TEXT("GameInstance is null."));
				}
			}
			else
			{
				UE_LOG(LogTemp , Error , TEXT("Failed to find 'gameId' in 'response' object."));
			}
		}
		else
		{
			UE_LOG(LogTemp , Error , TEXT("Failed to find 'response' object in JSON."));
		}
	}
	else
	{
		UE_LOG(LogTemp , Error , TEXT("Failed to parse JSON response."));
	}
}

// Host 닉네임 RPC
void AMTVS3_3rdPlayerState::ServerRPCSetHostNickname_Implementation(const FString& hostName)
{
	// 서버는 hostName을 정하고

	UE_LOG(LogTemp,Warning,TEXT("PlayerState: ServerRPCSetHostNickname_Implementation"))

	SetHostNickname(hostName);

	auto GS = Cast<AMTVS3_3rdGameState>(GetWorld()->GetGameState());
	GS->SetHostNickname(GetHostNickname());
}

// Guest 닉네임 RPC
void AMTVS3_3rdPlayerState::ServerRPCSetGuestNickname_Implementation(const FString& guestName)
{
	UE_LOG(LogTemp , Warning , TEXT("PlayerState: ServerRPCSetGuestNickname_Implementation"))
		// 서버는 guestName을 정하고
	SetGuestNickname(guestName);

	auto GS = Cast<AMTVS3_3rdGameState>(GetWorld()->GetGameState());
	GS->SetGuestNickname(GetGuestNickname());
}

void AMTVS3_3rdPlayerState::ServerRPCSetHostID_Implementation(const FString& hostId , const FString& AccessToken)
{
	SetHostToken(AccessToken);
	SetHostID(hostId);

	auto GS = GetWorld()->GetGameState<AMTVS3_3rdGameState>();
	if ( GS )
	{
		GS->SetHostToken(AccessToken);
		GS->SetHostID(hostId);
	}

	/*UE_LOG(LogTemp , Warning , TEXT("ServerRPCSetHostID_Implementation 111"));
	for ( FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it )
	{
		UE_LOG(LogTemp , Warning , TEXT("222"));
		if ( auto PC = Cast<AS3PCLobby>(*it) )
		{
			UE_LOG(LogTemp , Warning , TEXT("333"));
			auto PS = PC->GetPlayerState<AMTVS3_3rdPlayerState>();
			if ( PS )
			{
				UE_LOG(LogTemp , Warning , TEXT("HostID Set in Server"));
				PS->SetHostID(hostId);
			}
		}
	}*/
}

void AMTVS3_3rdPlayerState::ServerRPCSetGuestID_Implementation(const FString& guestId , const FString& AccessToken)
{
	SetGuestToken(AccessToken);
	SetGuestID(guestId);

	auto GS = GetWorld()->GetGameState<AMTVS3_3rdGameState>();
	if ( GS )
	{
		GS->SetGuestToken(AccessToken);
		GS->SetGuestID(guestId);
	}

	/*UE_LOG(LogTemp , Warning , TEXT("ServerRPCSetGuestID_Implementation 111"));
	for ( FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it )
	{
		UE_LOG(LogTemp , Warning , TEXT("222"));
		if ( auto PC = Cast<AS3PCLobby>(*it) )
		{
			UE_LOG(LogTemp , Warning , TEXT("333"));
			auto PS = PC->GetPlayerState<AMTVS3_3rdPlayerState>();
			if ( PS )
			{
				UE_LOG(LogTemp , Warning , TEXT("GuestID Set in Server"));
				PS->SetGuestID(guestId);
				if ( PS->GetHostID() == TEXT("HostID") )
				{
					UE_LOG(LogTemp , Warning , TEXT("HostID Set late in Server"));
					PS->SetHostID(GetHostID());
				}
			}
		}
	}*/
}

#pragma endregion