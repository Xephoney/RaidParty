// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KristianToolkit.h"
#include "AIControllerBase.generated.h"

UCLASS()
class RAIDPARTY_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	// General		//
	UPROPERTY(BlueprintReadOnly)
		int Points=0;
	UPROPERTY(BlueprintReadOnly)
		int PlayerNum;

	AAIControllerBase();
public:
	// Drinking Game //
	UPROPERTY(BluePrintReadWrite)
		TArray<EInputEnum>TargetInputQueue;
	UPROPERTY(BluePrintReadOnly)
		EInputEnum DirectionalInput=EInputEnum::IE_Up; // Same default as players
	UPROPERTY(BlueprintReadWrite)
		int InputIterator=0;
	UPROPERTY(BlueprintReadOnly)
		bool bLastInput = false;

	UFUNCTION(BlueprintCallable)
		void InputNextTarget();
	UFUNCTION(BlueprintCallable)
		void SetTargetInputQueue(TArray<EInputEnum> inArray) 
			{ TargetInputQueue = inArray; }

public:
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
		float MaxReactionTime = 1.f; // The maximum amount of time it takes for the AI to decide if it will keep it's hand in the mouth.

	UFUNCTION(BlueprintCallable)
		void BraveryCheck();
};
