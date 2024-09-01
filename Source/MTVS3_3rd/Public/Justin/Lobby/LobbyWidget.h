// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */

class UTextBlock;
class UButton;
class UImage;

UCLASS()
class MTVS3_3RD_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(bool bHost);
	void RemovePlayer();
	void AddPlayer();
	
	void SetReady(bool bReady);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PlayerOne_Image;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PlayerTwo_Image;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> PlayerTwoReady_Image;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Ready_Text;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StartGame_Text;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button;

private:
	bool bIsHost;
};
