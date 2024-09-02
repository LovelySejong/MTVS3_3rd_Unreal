// Copyright Epic Games, Inc. All Rights Reserved.

#include "MTVS3_3rdGameMode.h"
#include "MTVS3_3rdCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMTVS3_3rdGameMode::AMTVS3_3rdGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AMTVS3_3rdGameMode::OnQuiz1Start()
{
	SetState(ERoomState::QUIZ1_ROOM);
}

void AMTVS3_3rdGameMode::OnQuiz2Start()
{
	Q1ClearCount++;
	if ( Q1ClearCount == RequiredCount ) // 멀티플레이에서 Count가 2일 때 문제방1 종료 인정
	{
		SetState(ERoomState::QUIZ2_ROOM);
	}
}

void AMTVS3_3rdGameMode::OnMeetingStart()
{
	Q2ClearCount++;
	if ( Q2ClearCount == RequiredCount ) // 멀티플레이에서 Count가 2일 때 문제방2 종료 인정
	{
		SetState(ERoomState::MEETING_ROOM);
	}

	// 카페트 2개 체크
}

void AMTVS3_3rdGameMode::OnQuiz3Start()
{
	SetState(ERoomState::QUIZ3_ROOM);
}

void AMTVS3_3rdGameMode::OnQuiz4Start()
{
	SetState(ERoomState::QUIZ4_ROOM);
}

void AMTVS3_3rdGameMode::OnGameClear()
{
	SetState(ERoomState::ENDING_ROOM);
}

void AMTVS3_3rdGameMode::SetState(ERoomState NextState)
{
	ERoomState prevState = State;
	State = NextState;

	if ( DebugRoomStateEnable&& GEngine )
	{
		FString stateText = UEnum::GetValueAsString(State);
		GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Green , FString::Printf(TEXT("%s") , *stateText));
	}
}

void AMTVS3_3rdGameMode::CheckCarpet(int num, int value)
{
	if ( num == 1 )
	{
		OnCarpet1Count += value;
		if ( OnCarpet1Count == RequiredCount )
		{
			if ( DebugCarpetSuccessEnable) GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Yellow , FString::Printf(TEXT("Carpet1 ON")));
			// 문 열기 함수 호출
		}
	}
	else if ( num == 2 )
	{
		OnCarpet2Count += value;
		if ( OnCarpet2Count == RequiredCount )
		{
			if ( DebugCarpetSuccessEnable ) GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Yellow , FString::Printf(TEXT("Carpet2 ON")));
			// 장롱 열리는 함수 호출
			// 문제 액자 등장 함수 호출
		}
		else
		{
			if ( DebugCarpetSuccessEnable ) GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Yellow , FString::Printf(TEXT("Carpet2 OFF")));
			// 장롱 닫히는 함수 호출
			// 문제 액자 숨겨지는 함수 호출
		}
	}
}
