// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "DrinkGameAIController.generated.h"

/**
 * 
 */
UCLASS()
class RAIDPARTY_API ADrinkGameAIController : public AAIControllerBase
{
	GENERATED_BODY()

public:

	ADrinkGameAIController();

	// Drinking Game //
	UPROPERTY(BluePrintReadWrite)
		TArray<EInputEnum>TargetInputQueue;
	UPROPERTY(BluePrintReadOnly)
		EInputEnum DirectionalInput = EInputEnum::IE_Up; // Same default as players
	UPROPERTY(BlueprintReadWrite)
		int InputIterator = 0;
	UPROPERTY(BlueprintReadOnly)
		bool bLastInput = false;

	UFUNCTION(BlueprintCallable)
		void InputNextTarget();
	UFUNCTION(BlueprintCallable)
		void SetTargetInputQueue(TArray<EInputEnum> inArray)
	{
		TargetInputQueue = inArray;
	}
};
