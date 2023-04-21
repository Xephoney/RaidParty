// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "HeadInMouthAIController.generated.h"

/**
 * 
 */
UCLASS()
class RAIDPARTY_API AHeadInMouthAIController : public AAIControllerBase
{
	GENERATED_BODY()
	
public:

	AHeadInMouthAIController();

	// Hand in mouth //
	UPROPERTY(BlueprintReadWrite)
		bool bBiteHappening = false;
	UPROPERTY(BlueprintReadWrite)
		bool bBiteHappened = false;
	UPROPERTY(BlueprintReadWrite)
		bool bDead = false;
	UPROPERTY(BlueprintReadWrite)
		bool bHandInMouth = false;
	UPROPERTY(BlueprintReadWrite)
		float BraveryPercentage = 5.f; // How big of a chance that the AI will keep it's hand in the mouth when a bit might happen.
	UPROPERTY(BlueprintReadOnly)
		float MaxReactionTime = 0.5f; // The maximum amount of time it takes for the AI to decide if it will keep it's hand in the mouth.

	UFUNCTION(BlueprintCallable)
		void BraveryCheck();

};
