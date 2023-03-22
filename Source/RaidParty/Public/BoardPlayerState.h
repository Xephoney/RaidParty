// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BoardPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoinsChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTricksterPowerChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeepsChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTravellerPowerChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTraderPowerChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRankChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FColorChangedDelegate);


/**
 * 
 */
UCLASS()
class RAIDPARTY_API ABoardPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateState();

	

	UPROPERTY(BlueprintReadWrite, SaveGame)
	uint8 Coins {10};
	uint8 oldCoins = 0;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	uint8 Keeps{ 0 };
	uint8 oldKeeps = 5;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	uint8 TricksterShrinePower {0};
	uint8 oldTrickster = 5;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	uint8 TravellerShrinePower {0};
	uint8 oldTraveller = 5;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	uint8 TraderShrinePower {0	};
	uint8 oldTrader = 5;

	UPROPERTY(BlueprintReadWrite)
	FColor Color{1,1,1,1};
	FColor oldColor = FColor(0, 0, 0, 0);

	UPROPERTY(BlueprintReadWrite, SaveGame)
	uint8 Ranking{ 1 };
	uint8 oldRanking = 5;

	UPROPERTY(BlueprintAssignable)
	FOnCoinsChangedDelegate OnCoinsChanged;

	UPROPERTY(BlueprintAssignable)
	FOnKeepsChangedDelegate OnKeepsChanged;

	UPROPERTY(BlueprintAssignable)
	FTricksterPowerChangedDelegate OnTricksterChanged;

	UPROPERTY(BlueprintAssignable)
	FTravellerPowerChangedDelegate OnTravellerChanged;

	UPROPERTY(BlueprintAssignable)
	FTraderPowerChangedDelegate OnTraderChanged;

	UPROPERTY(BlueprintAssignable)
	FColorChangedDelegate OnColorChanged;

	UPROPERTY(BlueprintAssignable)
	FRankChangedDelegate OnRankingChanged;


	/*FORCEINLINE bool operator>(const ABoardPlayerState& Other)
	{

	}*/

};
