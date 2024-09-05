// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HintWidget.generated.h"

/**
 * 
 */
UCLASS()
class MTVS3_3RD_API UHintWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* HintPanel;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Quit;

	UFUNCTION()
	void OnQuitButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Hint1;

	void SetActiveHintPanel(bool value);

	void SetActiveHint1(bool value);
};
