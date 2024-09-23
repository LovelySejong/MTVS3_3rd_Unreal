// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/EndingWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UEndingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    Btn_Web->OnClicked.AddDynamic(this , &UEndingWidget::OnWebButtonClicked);
    //Btn_Quit->OnClicked.AddDynamic(this , &UEndingWidget::OnQuitButtonClicked);

    if ( EndingAnim )
    {
        PlayAnimation(EndingAnim);
    }
}

void UEndingWidget::OnWebButtonClicked()
{
    FString URL = TEXT("http://125.132.216.190:7979/quiz");
    FPlatformProcess::LaunchURL(*URL , nullptr , nullptr);
}

//void UEndingWidget::OnQuitButtonClicked()
//{
//    if ( APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld() , 0) )
//    {
//        PC->ClientTravel("/Game/LovelySejong/StartLevel.StartLevel" , ETravelType::TRAVEL_Absolute);
//    }
//}
