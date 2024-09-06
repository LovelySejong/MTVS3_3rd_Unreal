// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/HttpActor.h"
#include "HJ/StartWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHttpActor::AHttpActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHttpActor::BeginPlay()
{
	Super::BeginPlay();

	// 현재 레벨 이름 가져오기
	FString CurrentLevelName = GetWorld()->GetMapName();
	// 레벨 이름에서 경로 부분을 제거 (게임 실행 시 레벨 이름은 "파일경로:레벨이름" 형태일 수 있음 )
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	// 레벨 이름이 "StartLevel"인 경우에만 InitStartUI 호출
	if ( CurrentLevelName == "StartLevel" )
	{
		InitStartUI();
	}
}

// Called every frame
void AHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHttpActor::InitStartUI()
{
	StartUI = CastChecked<UStartWidget>(CreateWidget(GetWorld() , StartUIFactory));
	if ( StartUI )
	{
		StartUI->AddToViewport();
	}

	/*StartPanel->SetVisibility(ESlateVisibility::Hidden);
	FName LevelName = FName(TEXT("MatchingLevel"));
	UGameplayStatics::OpenLevel(this , LevelName);
	auto* pc = GetWorld()->GetFirstPlayerController();
	pc->SetInputMode(FInputModeGameOnly());*/

	auto* pc = UGameplayStatics::GetPlayerController(this , 0);
	if ( !pc ) return;
	pc->SetInputMode(FInputModeGameOnly());
	pc->SetShowMouseCursor(true);

	// 뷰포트 크기 가져오기
	int32 ViewportSizeX , ViewportSizeY;
	pc->GetViewportSize(ViewportSizeX , ViewportSizeY);

	// 뷰포트 크기의 30% 위치 계산
	int32 DesiredMouseX = FMath::TruncToInt(ViewportSizeX * 0.3f);
	int32 DesiredMouseY = FMath::TruncToInt(ViewportSizeY * 0.3f);

	// 마우스 위치 설정
	pc->SetMouseLocation(DesiredMouseX , DesiredMouseY);
}

void AHttpActor::ReqPostLogin(const FText& ID , const FText& Password)
{
	// HTTP 모듈을 가져오기
	FHttpModule* Http = &FHttpModule::Get();
	if ( !Http ) { return; }

	// HTTP 요청 생성
	TSharedRef<IHttpRequest , ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this , &AHttpActor::OnResPostLogin);

	// 서버 URL을 설정
	Request->SetURL(TEXT("https://"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type") , TEXT("application/json"));

	// JSON 형식으로 데이터를 생성
	FString ContentString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ContentString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("ID") , ID.ToString());
	Writer->WriteValue(TEXT("Password") , Password.ToString());
	Writer->WriteObjectEnd();
	Writer->Close();

	// 요청 본문에 JSON 데이터를 설정
	Request->SetContentAsString(ContentString);

	// 요청 실행
	Request->ProcessRequest();
}

void AHttpActor::OnResPostLogin(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully && Response.IsValid() )
	{
		// 응답 본문을 로그에 출력
		UE_LOG(LogTemp , Log , TEXT("Response Code: %d") , Response->GetResponseCode());
		UE_LOG(LogTemp , Log , TEXT("Response Body: %s") , *Response->GetContentAsString());

		if ( Response->GetResponseCode() == 200 ) // 로그인 성공 시 응답 코드가 200(OK)인 경우
		{
			// 응답 본문에서 토큰을 추출
			FString ResponseBody = Response->GetContentAsString();
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);

			if ( FJsonSerializer::Deserialize(Reader , JsonObject) && JsonObject.IsValid() )
			{
				FString Token = JsonObject->GetStringField("token"); // 예: "token" 필드에서 토큰 추출

				if ( !Token.IsEmpty() )
				{
					UE_LOG(LogTemp , Log , TEXT("Received Token: %s") , *Token);
					StartUI->OnLoginSuccess();
				}
				else
				{
					UE_LOG(LogTemp , Warning , TEXT("Token is missing in the response."));
					StartUI->OnLoginFail(1); // 토큰이 없는 경우 로그인 실패 처리
				}
			}
		}
		else
		{
			// 로그인 실패 시 처리 (예: 잘못된 자격 증명)
			UE_LOG(LogTemp , Warning , TEXT("Login failed with response code: %d") , Response->GetResponseCode());
			StartUI->OnLoginFail(1); // 로그인 실패 처리
		}
	}
	else
	{
		// 실패 처리
		UE_LOG(LogTemp , Error , TEXT("Request failed or response invalid"));
		StartUI->OnLoginFail(2);
	}
}

