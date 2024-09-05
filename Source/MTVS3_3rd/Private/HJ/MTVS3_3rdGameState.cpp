// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/MTVS3_3rdGameState.h"

AMTVS3_3rdGameState::AMTVS3_3rdGameState()
{
	// 레버 상태 초기화 (키: 1~4, 값: 모두 false로 시작)
	LeverStates.Add(1 , false);
	LeverStates.Add(2 , false);
	LeverStates.Add(3 , false);
	LeverStates.Add(4 , false);

	// 문제방4 상태 초기화 (키: 1~2, 값: 모두 false로 시작)
	Q4GimmickStates.Add(1 , false);
	Q4GimmickStates.Add(2 , false);
}

#pragma region 방 체크
// 체크포인트 액터가 호출하는 함수들
void AMTVS3_3rdGameState::OnQuiz1Start()
{
	SetState(ERoomState::QUIZ1_ROOM);
}

void AMTVS3_3rdGameState::OnQuiz2Start()
{
	Q1ClearCount++;
	if ( Q1ClearCount == RequiredCount ) // 멀티플레이에서 Count가 2일 때 문제방1 종료 인정
	{
		SetState(ERoomState::QUIZ2_ROOM);
	}
}

void AMTVS3_3rdGameState::OnMeetingStart()
{
	Q2ClearCount++;
	if ( Q2ClearCount == RequiredCount ) // 멀티플레이에서 Count가 2일 때 문제방2 종료 인정
	{
		SetState(ERoomState::MEETING_ROOM);
	}
}

void AMTVS3_3rdGameState::OnQuiz3Start()
{
	SetState(ERoomState::QUIZ3_ROOM);
}

void AMTVS3_3rdGameState::OnQuiz4Start()
{
	SetState(ERoomState::QUIZ4_ROOM);
}

void AMTVS3_3rdGameState::OnGameClear()
{
	SetState(ERoomState::ENDING_ROOM);
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
			// 문제 액자 등장 함수 호출
		}
		else
		{
			if ( DebugGimmickEnable ) GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Yellow , FString::Printf(TEXT("Carpet2 OFF")));
			// 장롱 닫히는 함수 호출
			// 문제 액자 숨겨지는 함수 호출
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