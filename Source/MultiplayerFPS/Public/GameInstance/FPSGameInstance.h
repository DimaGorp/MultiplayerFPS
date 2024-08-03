// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "FPSGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRequestComplete);

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UFPSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UFPSGameInstance();
protected:
	virtual void Init() override;
	//Login
	UFUNCTION(BlueprintCallable)
	void LogIn(FString usr,FString pass);
	//LoginDelegate
	void OnCompleteLogin(FHttpRequestPtr Request,FHttpResponsePtr Responce,bool bWasSuccessful);

	//Sign-un
	UFUNCTION(BlueprintCallable)
	void SignUp(FString usr, FString mail, FString pas);
	//SignUp Delegate
	void OnCompleteSignUp(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//Sign-un-confirmed
	UFUNCTION(BlueprintCallable)
	void SignUpConfirmed(FString cod);
	//SignUp Delegate
	void OnCompleteSignUpConfirmed(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//Logout
	UFUNCTION(BlueprintCallable)
	void LogOut();
    void OnCompleteLogout();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isLogged;
	UPROPERTY(BlueprintReadWrite)
	FString username;
	UPROPERTY(BlueprintReadWrite)
	FString password;
	UPROPERTY(BlueprintReadWrite)
	FString email;
	UPROPERTY(BlueprintReadWrite)
	FString code;

	
	
	//Delegates
	UPROPERTY(BlueprintAssignable)
	FOnRequestComplete UIDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnRequestComplete UIValidEmailDelegate;

	//Responce Messages

	FString* LoginMessage;
	FString* SignUpMessage;
	FString* ConfirmMessage;
	FString* PasswordMessage;
private:
	FHttpModule * Http;
	FString API_URL;

	
};
