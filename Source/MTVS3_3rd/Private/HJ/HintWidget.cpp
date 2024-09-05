// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/HintWidget.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanel.h"
#include "HJ/MTVS3_3rdGameState.h"
#include "Components/Button.h"

void UHintWidget::SetActiveHintPanel(bool value)
{
	if ( value )
	{
		HintPanel->SetVisibility(ESlateVisibility::Visible);
		SetAllHintsInactive();

		auto* gs = GetWorld()->GetGameState<AMTVS3_3rdGameState>();
		if ( !gs ) return;
		ERoomState CurrentState = gs->GetCurrentRoomState();
		switch ( CurrentState )
		{
		case ERoomState::QUIZ1_ROOM:
			Hint1->SetVisibility(ESlateVisibility::Visible);
			break;
		case ERoomState::QUIZ2_ROOM:
			Hint2->SetVisibility(ESlateVisibility::Visible);
			break;
		case ERoomState::MEETING_ROOM:
			HintMeeting->SetVisibility(ESlateVisibility::Visible);
			break;
		case ERoomState::QUIZ3_ROOM:
			Hint3->SetVisibility(ESlateVisibility::Visible);
			break;
		case ERoomState::QUIZ4_ROOM:
			Hint4->SetVisibility(ESlateVisibility::Visible);
			break;
		default:
			GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::White , TEXT("No hint"));
			break;
		}
		//FString stateText = StaticEnum<ERoomState>()->GetValueAsString(CurrentState);
		//GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Green , FString::Printf(TEXT("%s") , *stateText));
	}
	else
	{
		HintPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHintWidget::SetAllHintsInactive()
{
	Hint1->SetVisibility(ESlateVisibility::Hidden);
	Hint2->SetVisibility(ESlateVisibility::Hidden);
	HintMeeting->SetVisibility(ESlateVisibility::Hidden);
	Hint3->SetVisibility(ESlateVisibility::Hidden);
	Hint4->SetVisibility(ESlateVisibility::Hidden);
}