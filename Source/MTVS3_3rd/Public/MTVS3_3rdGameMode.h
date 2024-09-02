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

	UPROPERTY(VisibleAnywhere, Category = "Default|Game")
	int Q1ClearCount = 0;
	UPROPERTY(VisibleAnywhere, Category = "Default|Game")
	int Q2ClearCount = 0;
	UPROPERTY(EditAnywhere, Category = "Default|Game");
	int RequiredCount = 2;

	ERoomState State = ERoomState::TUTORIAL_ROOM;
	void SetState(ERoomState NextState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Debug")
	bool DebugRoomStateEnable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Debug")
	bool DebugCarpetSuccessEnable;
	
	void CheckCarpet(int num, int value);
	UPROPERTY(VisibleAnywhere, Category = "Default|Game")
	int OnCarpet1Count = 0;
	UPROPERTY(VisibleAnywhere, Category = "Default|Game")
	int OnCarpet2Count = 0;
};



