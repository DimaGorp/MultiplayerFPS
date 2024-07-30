#include "GameInstance/FPSGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

UFPSGameInstance::UFPSGameInstance()
{
    Http = &FHttpModule::Get();
    API_URL = "https://d6ca4keedb.execute-api.us-east-1.amazonaws.com/test";
}

void UFPSGameInstance::Init()
{
    Super::Init();
    isLogged = false;
}

void UFPSGameInstance::LogIn(FString usr,FString pass)
{
    UE_LOG(LogTemp, Log, TEXT("Username:%s"),*usr);
    UE_LOG(LogTemp, Log, TEXT("Password:%s"),*pass);
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("username"), *FString::Printf(TEXT("%s"), *usr));
	JsonObject->SetStringField(TEXT("password"), *FString::Printf(TEXT("%s"), *pass));

	FString JsonBody;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonBody);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    TSharedRef<IHttpRequest,ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this,&UFPSGameInstance::OnCompleteLogin);
    Request->SetURL(API_URL + FString("/users/login"));
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
        TSharedPtr<FJsonObject> ResponseMetadata = JsonObject->GetObjectField("ResponseMetadata");
        int32 Status = ResponseMetadata->GetIntegerField("HTTPStatusCode");

        if (Status == 200)
        {
            isLogged = true;
            UE_LOG(LogTemp, Log, TEXT("Login successful"));
            UIDelegate.Broadcast();
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
    UE_LOG(LogTemp, Log, TEXT("Username:%s"),*usr);
    UE_LOG(LogTemp, Log, TEXT("Email:%s"),*mail);
    UE_LOG(LogTemp, Log, TEXT("Password:%s"),*pas);
    username = usr;
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    JsonObject->SetStringField("username",*FString::Printf(TEXT("%s"),*usr)); 
    JsonObject->SetStringField("email",*FString::Printf(TEXT("%s"),*mail)); 
    JsonObject->SetStringField("password",*FString::Printf(TEXT("%s"),*pas));

    FString JsonBody;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonBody);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    TSharedPtr<IHttpRequest,ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this,&UFPSGameInstance::OnCompleteSignUp);
    Request->SetURL(API_URL + FString("/users/sign-up"));
    Request->SetContentAsString(JsonBody);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->ProcessRequest();
    
}

void UFPSGameInstance::OnCompleteSignUp(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful){
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP request failed or response is invalid"));
        return;
    }
    FString ResponseString = Response->GetContentAsString();
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        TSharedPtr<FJsonObject> ResponseMetadata = JsonObject->GetObjectField("ResponseMetadata");
        int32 Status = ResponseMetadata->GetIntegerField("HTTPStatusCode");

        if (Status == 200)
        {
            
            UE_LOG(LogTemp, Log, TEXT("Login successful"));
            UIDelegate.Broadcast();
        }
        else
        {
            username = "";
            FString Message = JsonObject->GetStringField("body");
            UE_LOG(LogTemp, Error, TEXT("Login failed: %s"), *Message);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response"));
    }

}

void UFPSGameInstance::SignUpConfirmed(FString cod)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    JsonObject->SetStringField("username",*FString::Printf(TEXT("%s"),*username)); 
    JsonObject->SetStringField("code",*FString::Printf(TEXT("%s"),*cod));

    FString JsonBody;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonBody);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    TSharedPtr<IHttpRequest,ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this,&UFPSGameInstance::OnCompleteSignUpConfirmed);
    Request->SetURL(API_URL + FString("/users/sign-up-confirmed"));
    Request->SetContentAsString(JsonBody);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->ProcessRequest();
    
}
void UFPSGameInstance::OnCompleteSignUpConfirmed(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP request failed or response is invalid"));
        return;
    }

    FString ResponseString = Response->GetContentAsString();
    UE_LOG(LogTemp, Log, TEXT("Response JSON: %s"), *ResponseString);

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
    
    if (!FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response"));
        return;
    }

    // Handle ResponseMetadata
    if (JsonObject->HasField("ResponseMetadata"))
    {
        TSharedPtr<FJsonObject> ResponseMetadata = JsonObject->GetObjectField("ResponseMetadata");
        if (ResponseMetadata.IsValid())
        {
            int32 Status = ResponseMetadata->GetIntegerField("HTTPStatusCode");

            if (Status == 200)
            {
                UE_LOG(LogTemp, Log, TEXT("Login successful"));
                username = ""; // Reset username or perform necessary actions
                UIDelegate.Broadcast();
            }
            else
            {
                // Since 'body' is not in the response, handle this case accordingly
                UE_LOG(LogTemp, Error, TEXT("Login failed with status code: %d"), Status);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("ResponseMetadata was not found in JSON"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ResponseMetadata field is missing from JSON"));
    }
}

void UFPSGameInstance::LogOut() 
{

}


void UFPSGameInstance::OnCompleteLogout()
{
    
}
