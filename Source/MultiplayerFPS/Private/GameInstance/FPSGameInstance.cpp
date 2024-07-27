#include "GameInstance/FPSGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

UFPSGameInstance::UFPSGameInstance()
{
    Http = &FHttpModule::Get();
    username = "dIma";
    password = "Dim@20012004";
}

void UFPSGameInstance::Init()
{
    Super::Init();
    isLogged = false;
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
    Request->SetContentAsString(JsonBody);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->ProcessRequest();
}

void UFPSGameInstance::OnCompleteLogin(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        isLogged = false;
        UE_LOG(LogTemp, Error, TEXT("HTTP request failed or response is invalid"));
        return;
    }

    FString ResponseString = Response->GetContentAsString();
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        FString Status = JsonObject->GetStringField("status");

        if (Status == "success")
        {
            isLogged = true;
            UE_LOG(LogTemp, Log, TEXT("Login successful"));
        }
        else
        {
            isLogged = false;
            FString Message = JsonObject->GetStringField("body");
            UE_LOG(LogTemp, Error, TEXT("Login failed: %s"), *Message);
        }
    }
    else
    {
        isLogged = false;
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response"));
    }
}


void UFPSGameInstance::SignUp(FString usr, FString mail, FString pas)
{
    
}

void UFPSGameInstance::LogOut() 
{

}


void UFPSGameInstance::OnCompleteLogout()
{
    
}
