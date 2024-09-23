// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MTVS3_3rdPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MTVS3_3RD_API AMTVS3_3rdPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
#pragma region HJ 
	void SetPlayerNickname(const FString& Nickname);
	FString GetPlayerNickname() const;
	FString PlayerNickname;
	
	// AccessToken을 저장할 변수 추가
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Authentication")
    FString AccessToken;
	
	int32 PlayerNum = 0;
	bool bIsHost;
	void CopyProperties(APlayerState* NewPlayerState);
	void SetHost(bool _bIsHost);

	// AccessToken을 설정하는 함수
    UFUNCTION(BlueprintCallable, Category = "Authentication")
    void SetAccessToken(const FString& InAccessToken);
#pragma endregion

};
