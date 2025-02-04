﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/QuizWidget.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UQuizWidget::SetActiveQuizPanel(bool value , int num)
{
	Btn_Black->OnClicked.AddDynamic(this , &UQuizWidget::OnBlackButtonClicked);
	if ( value )
	{
		QuizPanel->SetVisibility(ESlateVisibility::Visible);
		SetAllQuizzesInactive();
		auto* pc = UGameplayStatics::GetPlayerController(this , 0);
		pc->SetInputMode(FInputModeGameAndUI());
		pc->SetShowMouseCursor(true);
		switch ( num )
		{
		case 1:
			Quiz1->SetVisibility(ESlateVisibility::Visible);
			break;
		case 2:
			Quiz2->SetVisibility(ESlateVisibility::Visible);
			break;
		case 3:
			Quiz3->SetVisibility(ESlateVisibility::Visible);
			break;
		case 4:
			Quiz4->SetVisibility(ESlateVisibility::Visible);
			break;
		case 5:
			Quiz5->SetVisibility(ESlateVisibility::Visible);
			break;
		case 6:
			Quiz6->SetVisibility(ESlateVisibility::Visible);
			break;
		case 7:
			Quiz7->SetVisibility(ESlateVisibility::Visible);
			break;
		case 8:
			Quiz8->SetVisibility(ESlateVisibility::Visible);
			break;
		case 9:
			Quiz9->SetVisibility(ESlateVisibility::Visible);
			break;
		default:
			break;
		}
	}
	else
	{
		QuizPanel->SetVisibility(ESlateVisibility::Hidden);
		auto* pc = UGameplayStatics::GetPlayerController(this , 0);
		pc->SetInputMode(FInputModeGameOnly());
		pc->SetShowMouseCursor(false);
	}	
}

void UQuizWidget::SetAllQuizzesInactive()
{
	Quiz1->SetVisibility(ESlateVisibility::Hidden);
	Quiz2->SetVisibility(ESlateVisibility::Hidden);
	Quiz3->SetVisibility(ESlateVisibility::Hidden);
	Quiz4->SetVisibility(ESlateVisibility::Hidden);
	Quiz5->SetVisibility(ESlateVisibility::Hidden);
	Quiz6->SetVisibility(ESlateVisibility::Hidden);
	Quiz7->SetVisibility(ESlateVisibility::Hidden);
	Quiz8->SetVisibility(ESlateVisibility::Hidden);
	Quiz9->SetVisibility(ESlateVisibility::Hidden);
}

void UQuizWidget::OnBlackButtonClicked()
{
	SetActiveQuizPanel(false , 0);
}
