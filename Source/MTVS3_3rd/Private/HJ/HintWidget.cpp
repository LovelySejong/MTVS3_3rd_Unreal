// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/HintWidget.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanel.h"

void UHintWidget::SetActiveHintPanel(bool value)
{
	if ( value ) HintPanel->SetVisibility(ESlateVisibility::Visible);
	else HintPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UHintWidget::OnQuitButtonClicked()
{
	SetActiveHintPanel(false);
}

void UHintWidget::SetActiveHint1(bool value)
{
	SetActiveHintPanel(true);
	if ( value ) Hint1->SetVisibility(ESlateVisibility::Visible);
	else Hint1->SetVisibility(ESlateVisibility::Hidden);
}
