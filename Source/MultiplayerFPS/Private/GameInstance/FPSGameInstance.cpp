#include "GameInstance/FPSGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/KismetSystemLibrary.h"

UFPSGameInstance::UFPSGameInstance()
    : OnlineSubsystem(nullptr), isLogged(false)
{
}

void UFPSGameInstance::Init()
{
    Super::Init();
    OnlineSubsystem = IOnlineSubsystem::Get();
    if (!OnlineSubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Online Subsystem not available!"));
    }
}

void UFPSGameInstance::LogIn()
{
    if (OnlineSubsystem)
    {
        IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
        if (Identity.IsValid())
        {
            FOnlineAccountCredentials Credentials;
            Credentials.Id = FString();  // Add actual user ID if needed
            Credentials.Token = FString();  // Add actual token if needed
            Credentials.Type = FString("accountportal");

            Identity->OnLoginCompleteDelegates->AddUObject(this, &UFPSGameInstance::OnCompleteLogin);
            UE_LOG(LogTemp, Warning, TEXT("Attempting to login..."));
            Identity->Login(0, Credentials);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Online Identity Interface is not valid!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Online Subsystem is not initialized!"));
    }
}

void UFPSGameInstance::OnCompleteLogin(int32 LocalUserNum, bool wasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
    if (wasSuccessful)
    {
        isLogged = true;
        UE_LOG(LogTemp, Warning, TEXT("Login successful!"));
    }
    else
    {
        isLogged = false;
        UE_LOG(LogTemp, Error, TEXT("Login failed: %s"), *Error);
        if (Error.Contains(TEXT("EOS_Canceled")))
        {
            UE_LOG(LogTemp, Warning, TEXT("Login process was canceled. Please check your credentials and try again."));
        }
    }

    // Clean up the delegate
    if (OnlineSubsystem)
    {
        IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
        if (Identity.IsValid())
        {
            Identity->ClearOnLoginCompleteDelegates(0, this);
        }
    }
}

void UFPSGameInstance::LogOut()
{
    if (OnlineSubsystem)
    {
        IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
        if (Identity.IsValid())
        {
            Identity->OnLogoutCompleteDelegates->AddUObject(this, &UFPSGameInstance::OnCompleteLogout);
            Identity->Logout(0);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Online Identity Interface is not valid!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Online Subsystem is not initialized!"));
    }
}

void UFPSGameInstance::OnCompleteLogout(int32 LocalUserNum, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        isLogged = false;
        UE_LOG(LogTemp, Warning, TEXT("Logout successful!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Logout failed!"));
    }

    // Clean up the delegate
    if (OnlineSubsystem)
    {
        IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
        if (Identity.IsValid())
        {
            Identity->ClearOnLogoutCompleteDelegates(0, this);
        }
    }
}
