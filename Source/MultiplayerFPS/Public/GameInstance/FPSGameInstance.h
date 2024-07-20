// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
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
	void LogIn();
	//LoginDelegate
	void OnCompleteLogin(int32 LocalUsertNum,bool wasSuccessful,const FUniqueNetId& UserId, const FString& Error);
	//Logout
	UFUNCTION(BlueprintCallable)
	void LogOut();
    void OnCompleteLogout(int32 LocalUserNum, bool bWasSuccessful);
protected:
	class IOnlineSubsystem * OnlineSubsystem;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	bool isLogged;

	
};
