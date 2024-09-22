// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/EndingWidget.h"
#include "Components/Button.h"

void UEndingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    Btn_Web->OnClicked.AddDynamic(this , &UEndingWidget::OnWebButtonClicked);
    if ( EndingAnim )
    {
        PlayAnimation(EndingAnim);
    }
}

void UEndingWidget::OnWebButtonClicked()
{
    FString URL = TEXT("http://125.132.216.190:5173/quiz");
    FPlatformProcess::LaunchURL(*URL , nullptr , nullptr);
}
