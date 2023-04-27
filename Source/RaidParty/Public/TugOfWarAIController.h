// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "TugOfWarAIController.generated.h"

/**
 * 
 */
UCLASS()
class RAIDPARTY_API ATugOfWarAIController : public AAIControllerBase
{
	GENERATED_BODY()
	
public:
	ATugOfWarAIController();

	// Tug-of-War //
	UPROPERTY(BlueprintReadOnly)
		float MaxInputDelay = 0.5f;
	UPROPERTY(BlueprintReadOnly)
		int NumberOfPresses = 0;

	UFUNCTION(BlueprintCallable)
		void SimulateInput()
			{ NumberOfPresses++; Points++; }
	UFUNCTION(BlueprintCallable)
		void ResetNumPresses()
			{ NumberOfPresses=0; }

};
