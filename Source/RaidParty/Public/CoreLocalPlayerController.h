// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "CoreLocalPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class RAIDPARTY_API ACoreLocalPlayerController : public APlayerController
{
	GENERATED_BODY()
	ACoreLocalPlayerController();
public:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	class ABoardPlayerState* State{ nullptr };

	int32 PlayerIndex{ -1 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UInputMappingContext* InputMapping{ nullptr };

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite)
	bool bIsMyTurn {false};

	UPROPERTY(BlueprintReadWrite)
	bool bSelectingPaths{ false };

	UPROPERTY(BlueprintReadWrite)
	bool bWaitingForConfirmation{ false };

};
