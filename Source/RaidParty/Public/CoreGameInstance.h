// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CoreGameInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnCounterDelegate);


UCLASS()
class RAIDPARTY_API UCoreGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite);
	int TurnCounter = 1;

	UPROPERTY(BlueprintReadWrite);
	int TurnLimit = 15;

	UPROPERTY(BlueprintReadWrite);
	int Players = 4;

	UPROPERTY(BlueprintReadWrite);
	int Computers = 0;

	UPROPERTY(BlueprintAssignable);
	FOnTurnCounterDelegate OnTurnCounterChanged;

	UPROPERTY(BlueprintReadWrite)
	FString BoardSaveGameSlot {"DEBUG SAVE" };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString MinigameCompletedLevel { "LVL_MINIGAME_COMPLETE" };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FString> MinigameLevels;

	UPROPERTY(BlueprintReadWrite)
	bool bGameStarted{ false };

	UFUNCTION(BlueprintCallable)
	void EndOfTurn()
	{
		TurnCounter++;
		OnTurnCounterChanged.Broadcast();
	}
};
