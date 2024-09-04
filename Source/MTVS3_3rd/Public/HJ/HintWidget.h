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
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_BG1;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Hint1;
};
