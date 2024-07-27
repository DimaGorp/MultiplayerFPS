// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "FPSGameInstance.generated.h"
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

	//Sign-in
	UFUNCTION(BlueprintCallable)
	void SignUp(FString username,FString email,FString password);
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
	
	

private:
	FHttpModule * Http;

	
};
