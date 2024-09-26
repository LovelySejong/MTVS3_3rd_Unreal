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
	//int32 PlayerNum = 0;
	UPROPERTY(Replicated , BlueprintReadOnly , Category = Authentication)
	bool bIsHost;
	void CopyProperties(APlayerState* NewPlayerState);
	void SetHost(bool _bIsHost);

	void SetHostNickname(const FString& hostNickname);
	FString GetHostNickname() const;
	FString HostNickname;

	void SetGuestNickname(const FString& guestNickname);
	FString GetGuestNickname() const;
	FString GuestNickname;

	void SetHostID(const FString& hostID);
	FString GetHostID() const;
	UPROPERTY(ReplicatedUsing = OnRep_HostID , BlueprintReadOnly , Category = Authentication)
	FString HostID;
	UFUNCTION()
	void OnRep_HostID();

	void SetGuestID(const FString& guestID);
	FString GetGuestID() const;
	UPROPERTY(ReplicatedUsing = OnRep_GuestID , BlueprintReadOnly , Category = Authentication)
	FString GuestID;
	UFUNCTION()
	void OnRep_GuestID();

	void SetHostToken(const FString& hostToken);
	FString GetHostToken() const;
	UPROPERTY(Replicated , BlueprintReadOnly , Category = Authentication)
	FString HostToken;

	void SetGuestToken(const FString& guestToken);
	FString GetGuestToken() const;
	UPROPERTY(Replicated , BlueprintReadOnly , Category = Authentication)
	FString GuestToken;
#pragma endregion

# pragma region HJ 멀티플레이
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	// Host 닉네임 RPC
	UFUNCTION(Server , Reliable)
	void ServerRPCSetHostNickname(const FString& hostName);

	// Guest 닉네임 RPC
	UFUNCTION(Server , Reliable)
	void ServerRPCSetGuestNickname(const FString& guestName);

# pragma endregion
};
