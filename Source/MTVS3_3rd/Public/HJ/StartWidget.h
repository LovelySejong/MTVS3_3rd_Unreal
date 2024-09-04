// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartWidget.generated.h"

/**
 * 
 */
UCLASS()
class MTVS3_3RD_API UStartWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* StartPanel;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Start;

	UFUNCTION()
	void OnStartButtonClicked();

	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Quit;

	UFUNCTION()
	void OnQuitButtonClicked();

	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* LoginPanel;

	void SetActiveLoginUI(bool value);

	UPROPERTY(meta=(BindWidget))
	class UButton* Button_SignIn;

	UFUNCTION()
	void OnSignInButtonClicked();

	UPROPERTY(meta=(BindWidget))
	class UButton* Button_SignUp;

	UFUNCTION()
	void OnSignUpButtonClicked();

	UPROPERTY(meta=(BindWidget))
	class UButton* Button_X;

	UFUNCTION()
	void OnXButtonClicked();
};
