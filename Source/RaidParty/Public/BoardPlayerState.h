// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BoardPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RAIDPARTY_API ABoardPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, SaveGame)
	uint8 Coins {0};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	uint8 TricksterShrinePower {0};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	uint8 TravellerShrinePower {0};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	uint8 TraderShrinePower {0	};

	UPROPERTY(BlueprintReadWrite)
	FColor Color{1,1,1,1};
};
