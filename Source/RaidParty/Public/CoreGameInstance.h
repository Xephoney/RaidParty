// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CoreGameInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnCounterDelegate);


USTRUCT(BlueprintType)
struct FMinigamePlayer
{
	GENERATED_BODY()

	FMinigamePlayer() : PlayerIndex(-1), Score(-20.f)
	{};

	FMinigamePlayer(int32 ControllerIndex, float PlayerScore)
		: PlayerIndex(ControllerIndex), Score(PlayerScore)
	{};

	UPROPERTY(BlueprintReadWrite)
	int32 PlayerIndex {-1};

	UPROPERTY(BlueprintReadWrite)
	float Score{ 0.f };
};

USTRUCT(BlueprintType)
struct FMinigamePlayerResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int32 PlayerIndex {-21};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int32 Ranking {-231};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int32 PriceMoney{-321321452};

	FMinigamePlayerResult() : PlayerIndex(-1), Ranking(-1), PriceMoney(-1){}

	FMinigamePlayerResult(int32 ControllerIndex, int32 Rank, int32 Price)
		: PlayerIndex(ControllerIndex), Ranking(Rank), PriceMoney(Price)
	{

	};
};

UCLASS()
class RAIDPARTY_API UCoreGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite);
	int TurnCounter = 1;

	UPROPERTY(BlueprintReadWrite);
	int TurnLimit = 15;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite);
	int Players = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	int Computers = 0;

	UPROPERTY(BlueprintAssignable);
	FOnTurnCounterDelegate OnTurnCounterChanged;

	UPROPERTY(BlueprintReadWrite)
	FString BoardSaveGameSlot {"DEBUG SAVE" };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString MinigameCompletedLevel { "LVL_MINIGAME_COMPLETE" };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FString> MinigameLevels;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FMinigamePlayerResult> MinigameResult;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bGameStarted{ false };

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool bTutorialCompleted { false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bLoadedFromMinigame{ false };
	
	UFUNCTION(BlueprintCallable)
	void EndOfTurn()
	{
		TurnCounter++;
		OnTurnCounterChanged.Broadcast();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FMinigamePlayerResult> SortResult(TArray<FMinigamePlayer> MinigameParticipants);

};
