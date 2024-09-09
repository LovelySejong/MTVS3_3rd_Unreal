// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */

UENUM()
enum class EButtonType 
{
	NOTREADY, 
	READY,
	START,
	NOTSTART
};

class UTextBlock;
class UButton;
class UImage;
class UCircularThrobber;

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
	//Player 2
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_P2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ReadyIcon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCircularThrobber> NotReadyIcon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Name1Text;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Name2Text;

	void SetNameText(int num , const FString& Name);

	//Buttons
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Quit;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_StartON;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_StartOFF;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ReadyON;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ReadyOFF;

private:
	bool bIsHost;

	UFUNCTION()
	void QuitPressed();

	UFUNCTION()
	void StartONPressed();
	UFUNCTION()
	void StartOFFPressed();
	
	UFUNCTION()
	void ReadyONPressed();
	UFUNCTION()
	void ReadyOFFPressed();

	void DisplayButton(EButtonType Type);
	void DisplayPlayerTwo(bool bDisplay);
	void DisplayP2Status(bool bDisplay);
};
