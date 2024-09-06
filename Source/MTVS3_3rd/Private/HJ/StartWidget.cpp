// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/StartWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/Login/LoginPawn.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "HJ/HttpActor.h"

void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetActiveLoginUI(false);
	SetActiveRoadingUI(false);
	SetActiveConnectFailUI(false);
	LoginFailUI->SetVisibility(ESlateVisibility::Hidden);

	Button_Start->OnClicked.AddDynamic(this , &UStartWidget::OnStartButtonClicked);
	Button_Quit->OnClicked.AddDynamic(this , &UStartWidget::OnQuitButtonClicked);
	Button_SignIn->OnClicked.AddDynamic(this , &UStartWidget::OnSignInButtonClicked);
	Button_SignUp->OnClicked.AddDynamic(this , &UStartWidget::OnSignUpButtonClicked);
	Button_X->OnClicked.AddDynamic(this , &UStartWidget::OnXButtonClicked);
	Button_FailQuit->OnClicked.AddDynamic(this , &UStartWidget::OnFailQuitButtonClicked);
	Button_Guest->OnClicked.AddDynamic(this , &UStartWidget::OnGuestButtonClicked);
}

void UStartWidget::OnStartButtonClicked()
{
	SetActiveLoginUI(true);
}

void UStartWidget::OnQuitButtonClicked()
{
	auto* pc = UGameplayStatics::GetPlayerController(this , 0);
	UKismetSystemLibrary::QuitGame(GetWorld() , pc , EQuitPreference::Quit , false);
}

void UStartWidget::OnGuestButtonClicked()
{
	OnLoginSuccess();
}

void UStartWidget::SetActiveLoginUI(bool value)
{
	if (value ) LoginPanel->SetVisibility(ESlateVisibility::Visible);
	else LoginPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UStartWidget::OnSignInButtonClicked()
{
	/*StartPanel->SetVisibility(ESlateVisibility::Hidden);
	FName LevelName = FName(TEXT("MatchingLevel"));
	UGameplayStatics::OpenLevel(this , LevelName);
	auto* pc = GetWorld()->GetFirstPlayerController();
	auto* pc = UGameplayStatics::GetPlayerController(this , 0);
	pc->SetInputMode(FInputModeGameOnly());*/

	if ( EditableText_ID && EditableText_PW )
	{
		// 현재 텍스트를 가져와 로그에 출력하거나 필요한 작업 수행
		FText EnteredText_ID = EditableText_ID->GetText();
		UE_LOG(LogTemp , Log , TEXT("Entered Text_ID: %s") , *EnteredText_ID.ToString());

		FText EnteredText_PW = EditableText_PW->GetText();
		UE_LOG(LogTemp , Log , TEXT("Entered Text_PW: %s") , *EnteredText_PW.ToString());

		if ( !EnteredText_ID.IsEmpty() && !EnteredText_PW.IsEmpty() )
		{
			AHttpActor* HttpActor = CastChecked< AHttpActor>(UGameplayStatics::GetActorOfClass(this , AHttpActor::StaticClass()));
			if ( HttpActor )
			{
				HttpActor->ReqPostLogin(EnteredText_ID , EnteredText_PW);
			}
		}
		else
		{
			OnLoginFail(0); // 아이디 또는 비번 입력 안 함
		}
	}
	else
	{
		UE_LOG(LogTemp , Error , TEXT("EditableText 없음"));
	}
}

void UStartWidget::OnSignUpButtonClicked()
{
	FString URL = TEXT("https://descriptive-manchego-543.notion.site/In-Sink-aa4edfa5ebde4cf29cd7dc89c1ec014b?pvs=4");
	FPlatformProcess::LaunchURL(*URL , nullptr , nullptr);
}

void UStartWidget::OnXButtonClicked()
{
	SetActiveLoginUI(false);
}

void UStartWidget::OnLoginFail(int num)
{	
	switch ( num )
	{
	case 0: // OnLoginFail(0) 호출 시 아이디, 비번 입력 안 함
		FailText->SetText(FText::FromString(TEXT("아이디와 비밀번호를 모두 입력해주세요.")));
		break;
	case 1: // 회원 정보 없음
		FailText->SetText(FText::FromString(TEXT("회원 정보가 없습니다. 다시 확인해주세요.")));
		break;
	case 2: // 네트워크 연결 실패
		FailText->SetText(FText::FromString(TEXT("네트워크 연결을 확인해주세요.")));
		break;
	default:
		FailText->SetText(FText::FromString(TEXT("회원 정보가 없습니다. 다시 확인해주세요.")));
		break;
	}

	LoginFailUI->SetVisibility(ESlateVisibility::Visible);
	if ( LoginFailAnim )
	{
		PlayAnimation(LoginFailAnim);
	}
}

void UStartWidget::OnLoginSuccess()
{
	// ** 로그인 성공 시 연결
	//check if User successfully logged in.
	//if yes, allow player to connect
	auto Pawn = GetOwningPlayerPawn<ALoginPawn>();
	if ( Pawn )
	{
		Pawn->StartConnection(this);
	}
	//else ask user to sign in	
	// ** 로그인 성공 시 연결
}

void UStartWidget::SetActiveRoadingUI(bool value)
{
	if ( value ) RoadingPanel->SetVisibility(ESlateVisibility::Visible);
	else RoadingPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UStartWidget::SetActiveConnectFailUI(bool value)
{
	if ( value ) ConnectFailPanel->SetVisibility(ESlateVisibility::Visible);
	else ConnectFailPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UStartWidget::OnFailQuitButtonClicked()
{
	FString mapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld() , FName(*mapName));
}

