// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuizWidget.generated.h"

/**
 * 
 */
UCLASS()
class MTVS3_3RD_API UQuizWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* QuizPanel;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Black;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Quiz1;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Quiz2;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Quiz3;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Quiz4;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Quiz5;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Quiz6;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Quiz7;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Quiz8;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Quiz9;

	UFUNCTION(BlueprintCallable)
	void SetActiveQuizPanel(bool value, int num);

	void SetAllQuizzesInactive();

	UFUNCTION()
	void OnBlackButtonClicked();
};
