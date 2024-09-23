// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndingWidget.generated.h"

/**
 *
 */
UCLASS()
class MTVS3_3RD_API UEndingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Web;

	UFUNCTION()
	void OnWebButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Quit;

	//UFUNCTION()
	//void OnQuitButtonClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* EndingAnim;
};
