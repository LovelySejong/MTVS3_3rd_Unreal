// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/MTVS3_3rdGameState.h"
#include "Kismet/GameplayStatics.h"
#include "CSW/InteractionActor.h"
#include "HJ/HttpActor.h"
#include "Justin/S3GameInstance.h"
#include "MTVS3_3rdGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Justin/Lobby/LobbyWidget.h"
#include "Justin/Lobby/S3PCLobby.h"
#include "HJ/MTVS3_3rdPlayerState.h"
#include "MTVS3_3rdPlayerController.h"


AMTVS3_3rdGameState::AMTVS3_3rdGameState()
{
	// 레버 상태 초기화 (키: 1~4, 값: 모두 false로 시작)
	LeverStates.Add(1 , false);
	LeverStates.Add(2 , false);
	LeverStates.Add(3 , false);
	LeverStates.Add(4 , false);

	// 문제방4 상태 초기화 (키: 1~2, 값: 모두 false로 시작) / 주석 처리
	//Q4GimmickStates.Add(1 , false);
	//Q4GimmickStates.Add(2 , false);
}

void AMTVS3_3rdGameState::BeginPlay()
{
	Super::BeginPlay();
	auto PC = Cast<AS3PCLobby>(GetWorld()->GetFirstPlayerController());
	if ( !PC ) return;
	auto PS = PC->GetPlayerState<AMTVS3_3rdPlayerState>();
	if ( PS )
	{
		UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdGameState::BeginPlay() bIsHost: %d") , PS->bIsHost);
		//UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdGameState::BeginPlay() HostID: %s") , *PS->HostID);
		//UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdGameState::BeginPlay() GuestID: %s") , *PS->GuestID);
	}
}

#pragma region 매칭
void AMTVS3_3rdGameState::SetHostNickname(const FString& hostNickname)
{
	HostNickname = hostNickname;
	OnRep_HostNickname();
}

FString AMTVS3_3rdGameState::GetHostNickname() const
{
	if ( HostNickname.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("HostNickname is empty, returning default value."));
		return TEXT("Host");
	}
	return HostNickname;
}

void AMTVS3_3rdGameState::SetGuestNickname(const FString& guestNickname)
{
	GuestNickname = guestNickname;
	UE_LOG(LogTemp , Log , TEXT("SetGuestNickname: %s") , *GuestNickname);
	OnRep_GuestNickname();
}

FString AMTVS3_3rdGameState::GetGuestNickname() const
{
	if ( GuestNickname.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("GuestNickname is empty, returning default value."));
		return TEXT("Guest");
	}
	return GuestNickname;
}
void AMTVS3_3rdGameState::SetHostID(const FString& hostID)
{
	HostID = hostID;
	//auto PC = Cast<AS3PCLobby>(GetWorld()->GetFirstPlayerController());
	//auto PS = PC->GetPlayerState<AMTVS3_3rdPlayerState>();
	//if ( !PC || !PS ) return;
	//if ( !PS->HostID.IsEmpty() ) HostID = PS->GetHostID();
	UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdGameState::SetHostID(): %s") , *HostID);
	//OnRep_HostID();
}

FString AMTVS3_3rdGameState::GetHostID() const
{
	if ( HostID.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("HostID is empty, returning default value."));
		return TEXT("HostID");
	}
	return HostID;
}

void AMTVS3_3rdGameState::SetGuestID(const FString& guestID)
{
	GuestID = guestID;
	//auto PC = Cast<AS3PCLobby>(GetWorld()->GetFirstPlayerController());
	//auto PS = PC->GetPlayerState<AMTVS3_3rdPlayerState>();
	//if ( !PC || !PS ) return;
	//if ( !PS->GuestID.IsEmpty() ) GuestID = PS->GetHostID();
	UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdGameState::SetGuestID: %s") , *GuestID);
}

FString AMTVS3_3rdGameState::GetGuestID() const
{
	if ( GuestID.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("GuestID is empty, returning default value."));
		return TEXT("GuestID");
	}
	return GuestID;
}
void AMTVS3_3rdGameState::SetHostToken(const FString& hostToken)
{
	HostToken = hostToken;
	UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdPlayerState::SetHostToken(): %s") , *HostToken);
}
FString AMTVS3_3rdGameState::GetHostToken() const
{
	if ( HostToken.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("HostToken is empty, returning default value."));
		return TEXT("HostToken");
	}
	return HostToken;
}
void AMTVS3_3rdGameState::SetGuestToken(const FString& guestToken)
{
	GuestToken = guestToken;
	UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdPlayerState::SetGuestToken(): %s") , *GuestToken);
	OnRep_GuestToken();
}
FString AMTVS3_3rdGameState::GetGuestToken() const
{
	if ( GuestToken.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("GuestToken is empty, returning default value."));
		return TEXT("GuestToken");
	}
	return GuestToken;
}
#pragma endregion

#pragma region 멀티플레이
void AMTVS3_3rdGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMTVS3_3rdGameState , HostNickname);
	DOREPLIFETIME(AMTVS3_3rdGameState , GuestNickname);
	DOREPLIFETIME(AMTVS3_3rdGameState , HostID);
	DOREPLIFETIME(AMTVS3_3rdGameState , GuestID);
	DOREPLIFETIME(AMTVS3_3rdGameState , HostToken);
	DOREPLIFETIME(AMTVS3_3rdGameState , GuestToken);
}

void AMTVS3_3rdGameState::OnRep_HostNickname()
{
	// 클라이언트는 OnRep_HostNickname에 따라 UI 업데이트
	UpdateHostUI(GetHostNickname());
}

void AMTVS3_3rdGameState::UpdateHostUI(const FString& hostName)
{
	auto PC = Cast<AS3PCLobby>(GetWorld()->GetFirstPlayerController());
	if ( !PC )
	{
		UE_LOG(LogTemp , Warning , TEXT("PC is null in UpdateHostUI"));
		return;
	}

	auto LobbyUI = Cast<ULobbyWidget>(PC->LobbyWidget);
	if ( !LobbyUI )
	{
		UE_LOG(LogTemp , Warning , TEXT("LobbyUI is null in UpdateHostUI"));
		return;
	}

	LobbyUI->SetNameText(1 , hostName);
}

void AMTVS3_3rdGameState::OnRep_GuestNickname()
{
	UpdateGuestUI(GetGuestNickname());
	UpdateHostUI(GetHostNickname());
}

void AMTVS3_3rdGameState::UpdateGuestUI(const FString& guestName)
{
	auto PC = Cast<AS3PCLobby>(GetWorld()->GetFirstPlayerController());
	auto LobbyUI = Cast<ULobbyWidget>(PC->LobbyWidget);
	if ( PC && LobbyUI )
	{
		LobbyUI->SetNameText(2 , guestName);
	}
}

//void AMTVS3_3rdGameState::OnRep_HostID()
//{
//	AHttpActor* HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld() , AHttpActor::StaticClass()));
//	if ( !HttpActor ) return;
//
//	US3GameInstance* GI = Cast<US3GameInstance>(UGameplayStatics::GetGameInstance(this));
//	if ( !GI ) return;
//
//	HttpActor->ReqPostMatchState(GI->GetAccessToken() , GetHostID());
//}

void AMTVS3_3rdGameState::OnRep_GuestToken()
{
	if ( HasAuthority() )
	{
		MatchingState();
	}
}

void AMTVS3_3rdGameState::MatchingState()
{
	US3GameInstance* GI = Cast<US3GameInstance>(UGameplayStatics::GetGameInstance(this));
	if ( !GI ) return;

	FString AccessToken = GI->AccessToken;
	AHttpActor* HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld() , AHttpActor::StaticClass()));
	if ( !HttpActor ) return;

	for ( auto PlayerState : PlayerArray )
	{
		auto PS = Cast<AMTVS3_3rdPlayerState>(PlayerState);
		if ( !PS ) return;

		UE_LOG(LogTemp , Warning , TEXT("[%s]PlayerState bIsHost: %d") ,
			*PS->GetUniqueId().ToString() ,
			PS->bIsHost);
		if ( GetHostToken().IsEmpty() || GetGuestToken().IsEmpty() || GetGuestID().IsEmpty() || GetHostID().IsEmpty() ) return;
		if ( PS->bIsHost )
		{
			HttpActor->ReqPostMatchState(GetHostToken() , GetGuestID());
		}
		else HttpActor->ReqPostMatchState(GetGuestToken() , GetHostID());
	}
}
#pragma endregion

#pragma region 방 체크
// 체크포인트 액터가 호출하는 함수들
void AMTVS3_3rdGameState::OnQuiz1Start()
{
	SetState(ERoomState::QUIZ1_ROOM);

	US3GameInstance* GI = Cast<US3GameInstance>(UGameplayStatics::GetGameInstance(this));
	if ( !GI ) return;

	FString AccessToken = GI->AccessToken;
	AHttpActor* HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld() , AHttpActor::StaticClass()));
	if ( !HttpActor ) return;
	HttpActor->ReqPostRoomState(AccessToken , GI->GetGameID() , 1);

	//for ( auto PlayerState : PlayerArray )
	//{
	//	auto PS = Cast<AMTVS3_3rdPlayerState>(PlayerState);
	//	if ( !PS ) return;

	//	UE_LOG(LogTemp , Warning , TEXT("[%s]PlayerState bIsHost: %d") ,
	//		*PS->GetUniqueId().ToString() ,
	//		PS->bIsHost);
	//	if ( PS->bIsHost )
	//	{
	//		HttpActor->ReqPostRoomState(AccessToken , GI->GetGameID() , 1);
	//	}
	//	else HttpActor->ReqPostRoomState(AccessToken , GI->GetGameID() , 1);
	//}
}

void AMTVS3_3rdGameState::OnQuiz2Start()
{
	Q1ClearCount++;
	if ( Q1ClearCount == RequiredCount ) // 멀티플레이에서 Count가 2일 때 문제방1 종료 인정
	{
		SetState(ERoomState::QUIZ2_ROOM);
	}

	US3GameInstance* GI = Cast<US3GameInstance>(UGameplayStatics::GetGameInstance(this));
	if ( !GI ) return;

	FString AccessToken = GI->AccessToken;
	AHttpActor* HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld() , AHttpActor::StaticClass()));
	if ( !HttpActor ) return;
	HttpActor->ReqPostRoomState(AccessToken , GI->GetGameID() , 2);

	//for ( auto PlayerState : PlayerArray )
	//{
	//	auto PS = Cast<AMTVS3_3rdPlayerState>(PlayerState);
	//	if ( !PS ) return;

	//	UE_LOG(LogTemp , Warning , TEXT("[%s]PlayerState bIsHost: %d") ,
	//		*PS->GetUniqueId().ToString() ,
	//		PS->bIsHost);
	//	if ( PS->bIsHost )
	//	{
	//		HttpActor->ReqPostRoomState(GetHostToken() , GI->GetGameID() , 2);
	//	}
	//	else HttpActor->ReqPostRoomState(GetGuestToken() , GI->GetGameID() , 2);
	//}
}

void AMTVS3_3rdGameState::OnMeetingStart()
{
	Q2ClearCount++;
	if ( Q2ClearCount == RequiredCount ) // 멀티플레이에서 Count가 2일 때 문제방2 종료 인정
	{
		SetState(ERoomState::MEETING_ROOM);
	}

	US3GameInstance* GI = Cast<US3GameInstance>(UGameplayStatics::GetGameInstance(this));
	if ( !GI ) return;

	FString AccessToken = GI->AccessToken;
	AHttpActor* HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld() , AHttpActor::StaticClass()));
	if ( !HttpActor ) return;
	HttpActor->ReqPostRoomState(AccessToken , GI->GetGameID() , 3);

	//for ( auto PlayerState : PlayerArray )
	//{
	//	auto PS = Cast<AMTVS3_3rdPlayerState>(PlayerState);
	//	if ( !PS ) return;

	//	UE_LOG(LogTemp , Warning , TEXT("[%s]PlayerState bIsHost: %d") ,
	//		*PS->GetUniqueId().ToString() ,
	//		PS->bIsHost);
	//	if ( PS->bIsHost )
	//	{
	//		HttpActor->ReqPostRoomState(GetHostToken() , GI->GetGameID() , 3);
	//	}
	//	else HttpActor->ReqPostRoomState(GetGuestToken() , GI->GetGameID() , 3);
	//}
}

void AMTVS3_3rdGameState::OnQuiz3Start()
{
	SetState(ERoomState::QUIZ3_ROOM);

	US3GameInstance* GI = Cast<US3GameInstance>(UGameplayStatics::GetGameInstance(this));
	if ( !GI ) return;

	FString AccessToken = GI->AccessToken;
	AHttpActor* HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld() , AHttpActor::StaticClass()));
	if ( !HttpActor ) return;
	HttpActor->ReqPostRoomState(AccessToken , GI->GetGameID() , 4);

	//for ( auto PlayerState : PlayerArray )
	//{
	//	auto PS = Cast<AMTVS3_3rdPlayerState>(PlayerState);
	//	if ( !PS ) return;

	//	UE_LOG(LogTemp , Warning , TEXT("[%s]PlayerState bIsHost: %d") ,
	//		*PS->GetUniqueId().ToString() ,
	//		PS->bIsHost);
	//	if ( PS->bIsHost )
	//	{
	//		HttpActor->ReqPostRoomState(GetHostToken() , GI->GetGameID() , 4);
	//	}
	//	else HttpActor->ReqPostRoomState(GetGuestToken() , GI->GetGameID() , 4);
	//}
}

void AMTVS3_3rdGameState::OnQuiz4Start()
{
	SetState(ERoomState::QUIZ4_ROOM);

	US3GameInstance* GI = Cast<US3GameInstance>(UGameplayStatics::GetGameInstance(this));
	if ( !GI ) return;

	FString AccessToken = GI->AccessToken;
	AHttpActor* HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld() , AHttpActor::StaticClass()));
	if ( !HttpActor ) return;
	HttpActor->ReqPostRoomState(AccessToken , GI->GetGameID() , 5);

	//for ( auto PlayerState : PlayerArray )
	//{
	//	auto PS = Cast<AMTVS3_3rdPlayerState>(PlayerState);
	//	if ( !PS ) return;

	//	UE_LOG(LogTemp , Warning , TEXT("[%s]PlayerState bIsHost: %d") ,
	//		*PS->GetUniqueId().ToString() ,
	//		PS->bIsHost);
	//	if ( PS->bIsHost )
	//	{
	//		HttpActor->ReqPostRoomState(GetHostToken() , GI->GetGameID() , 5);
	//	}
	//	else HttpActor->ReqPostRoomState(GetGuestToken() , GI->GetGameID() , 5);
	//}
}

void AMTVS3_3rdGameState::OnGameClear()
{
	SetState(ERoomState::ENDING_ROOM);

	US3GameInstance* GI = Cast<US3GameInstance>(UGameplayStatics::GetGameInstance(this));
	if ( !GI ) return;

	FString AccessToken = GI->AccessToken;
	AHttpActor* HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld() , AHttpActor::StaticClass()));
	if ( !HttpActor ) return;
	HttpActor->ReqPostRoomState(AccessToken , GI->GetGameID() , 6);

	//for ( auto PlayerState : PlayerArray )
	//{
	//	auto PS = Cast<AMTVS3_3rdPlayerState>(PlayerState);
	//	if ( !PS ) return;

	//	UE_LOG(LogTemp , Warning , TEXT("[%s]PlayerState bIsHost: %d") ,
	//		*PS->GetUniqueId().ToString() ,
	//		PS->bIsHost);
	//	if ( PS->bIsHost )
	//	{
	//		HttpActor->ReqPostRoomState(GetHostToken() , GI->GetGameID() , 6);
	//	}
	//	else HttpActor->ReqPostRoomState(GetGuestToken() , GI->GetGameID() , 6);
	//}

	auto GM = Cast<AMTVS3_3rdGameMode>(GetWorld()->GetAuthGameMode());

	if ( GM )
	{
		GM->OnGameClear();
	}
}

void AMTVS3_3rdGameState::SetState(ERoomState NextState)
{
	ERoomState prevState = State;
	State = NextState;

	if ( DebugRoomStateEnable && GEngine )
	{
		FString stateText = UEnum::GetValueAsString(State);
		GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Green , FString::Printf(TEXT("%s") , *stateText));
	}
}
#pragma endregion

#pragma region 카페트 기믹
void AMTVS3_3rdGameState::CheckCarpet(int num , int value)
{
	if ( num == 1 ) // MEETING_ROOM의 Carpet1일 때
	{
		OnCarpet1Count += value;
		if ( OnCarpet1Count == RequiredCount )
		{
			if ( DebugGimmickEnable ) GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Yellow , FString::Printf(TEXT("Carpet1 ON")));
			// 문 열기 함수 호출
			TArray<AActor*> OutputActor;
			UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld() , AActor::StaticClass() , TEXT("DoorMeeting") , OutputActor);
			if ( !OutputActor.IsEmpty() )
			{
				auto tempActor = Cast<AInteractionActor>(OutputActor.Top());
				tempActor->ForceOpen();
			}
		}
		else
		{
			if ( DebugGimmickEnable ) GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Yellow , FString::Printf(TEXT("Carpet1 OFF")));
		}
	}
	else if ( num == 2 ) // QUIZ3_ROOM의 Carpet2일 때
	{
		OnCarpet2Count += value;
		if ( OnCarpet2Count == RequiredCount )
		{
			if ( DebugGimmickEnable ) GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Yellow , FString::Printf(TEXT("Carpet2 ON")));
			// 장롱 열리는 함수 호출
			TArray<AActor*> OutputActor;
			UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld() , AActor::StaticClass() , TEXT("ClosetDoor") , OutputActor);
			if ( !OutputActor.IsEmpty() )
			{
				auto tempActor = Cast<AInteractionActor>(OutputActor.Top());
				tempActor->ForceOpen();
			}
			// 문제 액자 등장 함수 호출
			UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld() , AActor::StaticClass() , TEXT("Frame1") , OutputActor);
			if ( !OutputActor.IsEmpty() )
			{
				auto tempActor = Cast<AInteractionActor>(OutputActor.Top());
				tempActor->ForceAppear();
			}
			UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld() , AActor::StaticClass() , TEXT("Frame2") , OutputActor);
			if ( !OutputActor.IsEmpty() )
			{
				auto tempActor = Cast<AInteractionActor>(OutputActor.Top());
				tempActor->ForceAppear();
			}
		}
		else
		{
			if ( DebugGimmickEnable ) GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Yellow , FString::Printf(TEXT("Carpet2 OFF")));
			TArray<AActor*> OutputActor;
			// 장롱 닫히는 함수 호출
			UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld() , AActor::StaticClass() , TEXT("ClosetDoor") , OutputActor);
			if ( !OutputActor.IsEmpty() )
			{
				auto tempActor = Cast<AInteractionActor>(OutputActor.Top());
				tempActor->ForceClose();
			}
			// 문제 액자 숨겨지는 함수 호출
			UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld() , AActor::StaticClass() , TEXT("Frame1") , OutputActor);
			if ( !OutputActor.IsEmpty() )
			{
				auto tempActor = Cast<AInteractionActor>(OutputActor.Top());
				tempActor->ForceDisapper();
			}
			UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld() , AActor::StaticClass() , TEXT("Frame2") , OutputActor);
			if ( !OutputActor.IsEmpty() )
			{
				auto tempActor = Cast<AInteractionActor>(OutputActor.Top());
				tempActor->ForceDisapper();
			}
		}
	}
}
#pragma endregion

#pragma region 레버 기믹(문제방3)
void AMTVS3_3rdGameState::CheckLever(int num , bool bIsCorrect)
{
	// 레버 키가 1~4인지 확인하고 업데이트
	if ( LeverStates.Contains(num) )
	{
		LeverStates[num] = bIsCorrect;  // 해당 레버 키의 상태 업데이트
	}
	else
	{
		UE_LOG(LogTemp , Warning , TEXT("Invalid lever key: %d") , num);
		return;
	}

	// 모든 레버가 정답인지 확인
	bool bAllCorrect = true;
	for ( const TPair<int , bool>& Pair : LeverStates )
	{
		if ( !Pair.Value )
		{
			bAllCorrect = false;
			break;
		}
	}

	if ( bAllCorrect )
	{
		// P2 카페트 올라오기
		if ( DebugGimmickEnable ) GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Yellow , FString::Printf(TEXT("All levers are correct. Carpet rises.")));
		MovingUpCarpet();
	}
	else
	{
		// P2 카페트 내려가기
		if ( DebugGimmickEnable ) GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Yellow , FString::Printf(TEXT("Some levers are incorrect. Carpet hides.")));
		MovingDownCarpet();
	}
}
#pragma endregion

#pragma region 문제방4(주석 처리)
//void AMTVS3_3rdGameState::CheckQuiz4(int num , bool bIsCorrect)
//{
//	// 모든 기믹이 정답인지 확인
//	bool bAllCorrect = true;
//	for ( const TPair<int , bool>& Pair : Q4GimmickStates )
//	{
//		if ( !Pair.Value )
//		{
//			bAllCorrect = false;
//			break;
//		}
//	}
//
//	if ( bAllCorrect )
//	{
//		UE_LOG(LogTemp , Log , TEXT("Game Clear"));
//		// 엔딩방 문 열기
//	}
//}
#pragma endregion