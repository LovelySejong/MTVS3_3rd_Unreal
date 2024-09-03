// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MTVS3_3rdGameMode.generated.h"

UENUM(BlueprintType)
enum class ERoomState : uint8
{
	TUTORIAL_ROOM = 0,	// 튜토리얼 방
	QUIZ1_ROOM = 1,	// 문제방1. 문제방1부터 시간 측정
	QUIZ2_ROOM = 2,	// 문제방2
	MEETING_ROOM = 3,	// 팀원을 만나는 방
	QUIZ3_ROOM = 4,	// 문제방3
	QUIZ4_ROOM = 5,	// 문제방4. 문제방4까지 시간 측정
	ENDING_ROOM = 6,	// 엔딩방
};

UCLASS(minimalapi)
class AMTVS3_3rdGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMTVS3_3rdGameMode();

#pragma region Default
	// 디버그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Debug")
	bool DebugRoomStateEnable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Debug")
	bool DebugGimmickEnable;

	// 에디터 설정
	
	UPROPERTY(EditAnywhere, Category = "Default|Game");
	int RequiredCount = 2;
#pragma endregion

#pragma region 방 체크
	// 튜토리얼 끝, 문제방1 진입
	void OnQuiz1Start();
	// 문제방1 끝, 문제방2 진입
	void OnQuiz2Start();
	// 문제방2 끝, 상봉방 진입
	void OnMeetingStart();
	// 상봉방 끝, 문제방3 진입
	void OnQuiz3Start();
	// 문제방3 끝, 문제방4 진입
	void OnQuiz4Start();
	// 문제방4 끝, 게임 클리어
	void OnGameClear();

	ERoomState State = ERoomState::TUTORIAL_ROOM;
	void SetState(ERoomState NextState);

	// 개인 진행되는 문제방1,2 클리어 카운트
	// RequiredCount와 같은 값이 되면 클리어 인정
	UPROPERTY(VisibleAnywhere, Category = "Default|Game")
	int Q1ClearCount = 0;
	UPROPERTY(VisibleAnywhere, Category = "Default|Game")
	int Q2ClearCount = 0;
#pragma endregion
	
#pragma region 카페트 기믹
	UPROPERTY(VisibleAnywhere, Category = "Default|Game")
	int OnCarpet1Count = 0; // MEETING_ROOM의 Carpet1에 올라간 플레이어 카운트
	UPROPERTY(VisibleAnywhere, Category = "Default|Game")
	int OnCarpet2Count = 0; // QUIZ3_ROOM의 Carpet2에 올라간 플레이어 카운트

	// num에 해당하는 카페트의 value 처리
	// 카페트에 접촉하면 value=1, 카페트를 나가면 value=-1과 호출됨
	void CheckCarpet(int num , int value);
#pragma endregion

#pragma region 레버 기믹(문제방3)
	TMap<int , bool> LeverStates;  // 각 레버의 키와 상태를 저장하는 TMap
	UFUNCTION(BlueprintCallable)
	void CheckLever(int num, bool bIsCorrect);

	UFUNCTION(BlueprintImplementableEvent) // 카페트를 들어올리는 이벤트
	void MovingUpCarpet();
	UFUNCTION(BlueprintImplementableEvent) // 카페트를 아래로 내리는 이벤트
	void MovingDownCarpet();
#pragma endregion

#pragma region 문제방4
	TMap<int , bool> Q4GimmickStates;  // 문제방4 기믹의 키와 상태를 저장하는 TMap
	void CheckQuiz4(int num , bool bIsCorrect);
#pragma endregion

};



