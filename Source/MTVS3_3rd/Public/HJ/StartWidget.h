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

	// 시작
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

	// 로그인
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* LoginPanel;

	void SetActiveLoginUI(bool value);

	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditableText_ID;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditableText_PW;

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

	UPROPERTY(meta = (BindWidget))
	class UOverlay* LoginFailUI;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FailText;

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* LoginFailAnim;

	UFUNCTION()
	void OnLoginFail(int num);

	void OnLoginSuccess();

	// 로딩
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* RoadingPanel;

	void SetActiveRoadingUI(bool value);
	
	// 연결 실패
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* ConnectFailPanel;

	void SetActiveConnectFailUI(bool value);

	UPROPERTY(meta=(BindWidget))
	class UButton* Button_FailQuit;

	UFUNCTION()
	void OnFailQuitButtonClicked();
};
