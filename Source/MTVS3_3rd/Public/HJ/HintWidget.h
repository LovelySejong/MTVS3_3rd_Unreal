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
	class UOverlay* Hint1;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Hint2;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* HintMeeting;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Hint3;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Hint4;

	UFUNCTION(BlueprintCallable)
	void SetActiveHintPanel(bool value);

	void SetAllHintsInactive();
};
