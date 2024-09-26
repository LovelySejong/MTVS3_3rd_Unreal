// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/HttpActor.h"
#include "HJ/StartWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/S3GameInstance.h"
#include "HJ/MTVS3_3rdPlayerState.h"

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

	//// 뷰포트 크기 가져오기
	//int32 ViewportSizeX , ViewportSizeY;
	//pc->GetViewportSize(ViewportSizeX , ViewportSizeY);

	//// 뷰포트 크기의 30% 위치 계산
	//int32 DesiredMouseX = FMath::TruncToInt(ViewportSizeX * 0.3f);
	//int32 DesiredMouseY = FMath::TruncToInt(ViewportSizeY * 0.3f);

	//// 마우스 위치 설정
	//pc->SetMouseLocation(DesiredMouseX , DesiredMouseY);
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
	Request->SetURL(TEXT("http://125.132.216.190:7878/api/auth/login"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type") , TEXT("application/json"));

	// JSON 형식으로 데이터를 생성
	FString ContentString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ContentString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("email") , ID.ToString());
	Writer->WriteValue(TEXT("password") , Password.ToString());
	Writer->WriteObjectEnd();
	Writer->Close();

	// 요청 본문에 JSON 데이터를 설정
	Request->SetContentAsString(ContentString);

	// 요청 실행
	Request->ProcessRequest();
}

void AHttpActor::OnResPostLogin(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bWasSuccessful)
{
	if ( bWasSuccessful && Response.IsValid() )
	{
		// 응답 본문을 로그에 출력
		UE_LOG(LogTemp , Log , TEXT("Response Code: %d") , Response->GetResponseCode());
		UE_LOG(LogTemp , Log , TEXT("Response Body: %s") , *Response->GetContentAsString());

		if ( Response->GetResponseCode() == 200 ) // 응답 코드가 200(OK)인지 확인
		{
			// JSON 응답 파싱
			FString ResponseBody = Response->GetContentAsString();
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);

			if ( FJsonSerializer::Deserialize(Reader , JsonObject) && JsonObject.IsValid() )
			{
				// "response" 객체에 접근
				TSharedPtr<FJsonObject> ResponseObject = JsonObject->GetObjectField("response");

				if ( ResponseObject.IsValid() )
				{
					// "accessToken"과 "nickname" 추출
					FString AccessToken = ResponseObject->GetStringField("accessToken");
					FString Nickname = ResponseObject->GetStringField("nickname");

					US3GameInstance* GI = GetWorld()->GetGameInstance<US3GameInstance>();
					if ( !GI ) return;
					GI->SetPlayerNickname(Nickname);

					if ( !AccessToken.IsEmpty() )
					{
						UE_LOG(LogTemp , Log , TEXT("Received Access Token: %s") , *AccessToken);
						UE_LOG(LogTemp , Log , TEXT("Received Nickname: %s") , *Nickname);

						// 로그인 성공 시 처리
						GI = GetWorld()->GetGameInstance<US3GameInstance>();
						if ( !GI ) return;
						GI->SetAccessToken(AccessToken);
						StartUI->OnLoginSuccess();
					}
					else
					{
						UE_LOG(LogTemp , Warning , TEXT("Access Token is missing in the response."));
						StartUI->OnLoginFail(1); // 로그인 실패 처리
					}
				}
				else
				{
					UE_LOG(LogTemp , Warning , TEXT("'response' 객체를 찾을 수 없습니다."));
					StartUI->OnLoginFail(1); // 로그인 실패 처리
				}
			}
			else
			{
				UE_LOG(LogTemp , Warning , TEXT("JSON 응답 파싱 실패."));
				StartUI->OnLoginFail(1); // 로그인 실패 처리
			}
		}
		else
		{
			UE_LOG(LogTemp , Warning , TEXT("로그인 실패, 응답 코드: %d") , Response->GetResponseCode());
			StartUI->OnLoginFail(1); // 로그인 실패 처리
		}
	}
	else
	{
		UE_LOG(LogTemp , Error , TEXT("요청 실패 또는 응답이 유효하지 않음"));
		StartUI->OnLoginFail(2); // 네트워크 오류 처리
	}
}

void AHttpActor::ReqPostMatchState(const FString& AccessToken , const FString& PartnerID , AMTVS3_3rdPlayerState* PS)
{
	// AccessToken과 PartnerID를 로그로 출력
	UE_LOG(LogTemp , Warning , TEXT("AHttpActor::ReqPostMatchState AccessToken: %s, PartnerID: %s") , *AccessToken , *PartnerID);

	// HTTP 모듈 가져오기
	FHttpModule* Http = &FHttpModule::Get();
	if ( !Http ) { return; }

	// HTTP 요청 생성
	TSharedRef<IHttpRequest , ESPMode::ThreadSafe> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(PS , &AMTVS3_3rdPlayerState::OnResPostMatchState);

	//Request->OnProcessRequestComplete().BindUObject(this , &AHttpActor::OnResPostMatchState);

	Request->SetURL(TEXT("http://125.132.216.190:7878/api/game")); // 매칭 상태 URL
	Request->SetVerb(TEXT("POST"));

	// AccessToken을 Authorization 헤더에 추가
	Request->SetHeader(TEXT("Content-Type") , TEXT("application/json"));
	Request->SetHeader(TEXT("Authorization") , FString::Printf(TEXT("Bearer %s") , *AccessToken));

	// Body에 PartnerID를 JSON으로 포함
	FString ContentString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ContentString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("partnerID") , PartnerID); // PartnerID를 JSON에 포함
	Writer->WriteObjectEnd();
	Writer->Close();

	Request->SetContentAsString(ContentString);

	// HTTP 요청 전송
	Request->ProcessRequest();
}

void AHttpActor::ReqPostRoomState(const FString& AccessToken , const int32& GameID , int32 RoomNumber)
{
	// AccessToken과 RoomNumber를 로그로 출력
	UE_LOG(LogTemp , Warning , TEXT("AccessToken: %s, gameId: %d, RoomNumber: %d") , *AccessToken , GameID , RoomNumber);

	// HTTP module
	FHttpModule* Http = &FHttpModule::Get();
	if ( !Http ) { return; }

	// Create HTTP request
	TSharedRef<IHttpRequest , ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this , &AHttpActor::OnResPostRoomState);

	Request->SetURL(TEXT("http://125.132.216.190:7878/api/log/clear"));
	Request->SetVerb(TEXT("POST"));

	Request->SetHeader(TEXT("Content-Type") , TEXT("application/json"));
	Request->SetHeader(TEXT("Authorization") , FString::Printf(TEXT("Bearer %s") , *AccessToken));

	// Body에 PartnerID와 RoomNumber를 JSON 형식으로 설정
	FString ContentString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ContentString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("gameId") , GameID);
	Writer->WriteValue(TEXT("roomNumber") , RoomNumber);
	Writer->WriteObjectEnd();
	Writer->Close();

	Request->SetContentAsString(ContentString);

	Request->ProcessRequest();

	//Request->SetHeader(TEXT("Content-Type") , TEXT("application/json"));

	//// Create JSON content
	//FString ContentString;
	//TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ContentString);
	//Writer->WriteObjectStart();
	//Writer->WriteValue(TEXT("AccessToken") , AccessToken);
	//Writer->WriteValue(TEXT("RoomNumber") , RoomNumber);
	//Writer->WriteObjectEnd();
	//Writer->Close();

	//// Set request content
	//Request->SetContentAsString(ContentString);

	//// Execute request
	//Request->ProcessRequest();
}

void AHttpActor::OnResPostRoomState(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bWasSuccessful)
{
	if ( bWasSuccessful && Response.IsValid() )
	{
		UE_LOG(LogTemp , Log , TEXT("Request Successful!"));
		UE_LOG(LogTemp , Log , TEXT("Response Code: %d") , Response->GetResponseCode());
		UE_LOG(LogTemp , Log , TEXT("Response Body: %s") , *Response->GetContentAsString());
	}
	else
	{
		// 요청이 실패하거나 응답이 유효하지 않은 경우
		UE_LOG(LogTemp , Error , TEXT("Request Failed or Invalid Response"));

		if ( Response.IsValid() )
		{
			// 유효하지 않은 응답 코드 출력
			UE_LOG(LogTemp , Error , TEXT("Response Code: %d") , Response->GetResponseCode());
			UE_LOG(LogTemp , Error , TEXT("Response Body: %s") , *Response->GetContentAsString());
		}
		else
		{
			// 응답 자체가 유효하지 않은 경우
			UE_LOG(LogTemp , Error , TEXT("No Response or Response is Null"));
		}
	}
}
