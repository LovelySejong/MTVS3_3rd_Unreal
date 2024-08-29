// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MTVS3_3rdGameMode.generated.h"

UENUM(BlueprintType)
enum class ERoomState : uint8
{
	TUTORIAL_ROOM = 0,	// 튜토리얼 방
	PUZZLE_ROOM_1 = 1,	// 문제방1. 문제방1부터 시간 측정
	PUZZLE_ROOM_2 = 2,	// 문제방2
	MEETING_ROOM = 3,	// 팀원을 만나는 방
	PUZZLE_ROOM_3 = 4,	// 문제방3
	PUZZLE_ROOM_4 = 5,	// 문제방4. 문제방4까지 시간 측정
	ENDING_ROOM = 6,	// 엔딩방
};

UCLASS(minimalapi)
class AMTVS3_3rdGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMTVS3_3rdGameMode();
};



