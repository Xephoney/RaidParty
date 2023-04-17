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

};

