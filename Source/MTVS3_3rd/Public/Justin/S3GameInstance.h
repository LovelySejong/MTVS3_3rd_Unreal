// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "S3GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MTVS3_3RD_API US3GameInstance : public UGameInstance, public FTickableGameObject	
{
	GENERATED_BODY()
public:
	US3GameInstance();
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;

public:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UFUNCTION(BlueprintCallable)
	void CreateServer();
	UFUNCTION(BlueprintCallable)
	void FindServer();
	UFUNCTION(BlueprintCallable)
	void JoinServer(int32 Index);
	UFUNCTION(BlueprintCallable)
	void StartSession();
	UFUNCTION(BlueprintCallable)
	void DestroyServer();

#pragma region HJ 
	bool bIsHost;
	void SetHost(bool _bIsHost);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Authentication")
    FString PlayerID;
	UFUNCTION(BlueprintCallable, Category = "Authentication")
    void SetPlayerID(const FString& id);
	FString GetPlayerID() const;
	
	void SetPlayerNickname(const FString& Nickname);
	FString GetPlayerNickname() const;
	FString PlayerNickname;
	
	// AccessToken을 저장할 변수 추가
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Authentication")
    FString AccessToken;
	// AccessToken을 설정하는 함수
    UFUNCTION(BlueprintCallable, Category = "Authentication")
    void SetAccessToken(const FString& InAccessToken);
#pragma endregion

protected:
	virtual void Init() override;
	virtual void Shutdown() override;

	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
private:
	float MaxTime = 2.f;
	float TotalTime = 0.f;
};
