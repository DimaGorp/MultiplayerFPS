#include "GameInstance/FPSGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FPSGameInstance.h"

UFPSGameInstance::UFPSGameInstance()
{
    Http = &FHttpModule::Get();
}

void UFPSGameInstance::Init()
{
    Super::Init();
    if(!isLogged)
        LogIn(username,password);
    
}

void UFPSGameInstance::LogIn(FString usr,FString pass)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("username"), *FString::Printf(TEXT("%s"), *usr));
	JsonObject->SetStringField(TEXT("password"), *FString::Printf(TEXT("%s"), *pass));

	FString JsonBody;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonBody);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    TSharedRef<IHttpRequest,ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this,&UFPSGameInstance::OnCompleteLogin);
    Request->SetURL("https://d6ca4keedb.execute-api.us-east-1.amazonaws.com/test" + FString("/login"));
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->ProcessRequest();
}

void UFPSGameInstance::OnCompleteLogin(FHttpRequestPtr Request, FHttpResponsePtr Responce, bool bWasSuccessful)
{
    isLogged = bWasSuccessful;
}

void UFPSGameInstance::SignUp(FString username, FString email, FString password)
{
    
}

void UFPSGameInstance::LogOut() 
{

}


void UFPSGameInstance::OnCompleteLogout()
{
    
}
